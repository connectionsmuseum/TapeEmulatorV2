

#include <atmel_start.h>
#include <stdio.h>
#include "block_transfer.h"
#include "fatFS/ff.h"

#define BLOCK_SIZE 2000

static struct spi_m_dma_descriptor * _SPI;

struct transfer_buffer;

struct transfer_buffer {
    uint8_t block[BLOCK_SIZE];
    int track;
    int block_id;
    unsigned int length;
    bool locked;
};


static struct transfer_buffer bufferA;
static struct transfer_buffer bufferB;

void _initialize_buffer(struct transfer_buffer *buf);
int _load_block_into_buffer(int track, int block_id, struct transfer_buffer *buffer);
static void tx_complete_cb_bufferA(struct _dma_resource *resource);
static void tx_complete_cb_bufferB(struct _dma_resource *resource);

static bool _dma_running;

void init_block_buffer(struct spi_m_dma_descriptor *SPI) {
    _SPI  = SPI;
    _initialize_buffer(&bufferA);
    _initialize_buffer(&bufferB);
}

void _initialize_buffer(struct transfer_buffer *buf) {
    buf->block_id = -1;
    buf->track = -1;
    buf->length = 0;
    buf->locked = false;
}

/*
 * Ensure that the target block is available in memory;
 * load from SD card if not already loaded.
 *
 */
void load_next_block(int track, int current_block_id) {

    int next_block_id = current_block_id + 1;

    if((current_block_id != bufferA.block_id) &&
       (current_block_id != bufferB.block_id)) {

        // Need to load the current block; preempts loading
        // the next block
        if((bufferA.block_id >= 0) && !bufferA.locked) {
            _load_block_into_buffer(track, current_block_id, &bufferA);
        } else if(!bufferB.locked) {
            // Write into this buffer even if it's occupied, but
            // not if it's locked.
            _load_block_into_buffer(track, current_block_id, &bufferB);
        }

        // We don't want to load two blocks in the same function call.
        return;
    }

    // Check if we need to load the next block
    if((next_block_id == bufferA.block_id) ||
       (next_block_id == bufferB.block_id)) {
        return;
    }

    // Write the new block into whichever buffer
    // *doesn't* have the current block.
    if(current_block_id == bufferA.block_id) {
        // Write into B
        if(!bufferB.locked) {
            _load_block_into_buffer(track, next_block_id, &bufferB);
        }
    } else if (current_block_id == bufferB.block_id) {
        // Write into A
        if(!bufferA.locked) {
            _load_block_into_buffer(track, next_block_id, &bufferA);
        }
    }

}

/*
 * Does the work of loading a single block into the supplied
 * buffer.
 *
 * This takes about 20 ms to load a block.
 */
int _load_block_into_buffer(int track, int block_id, struct transfer_buffer *buffer) {

    FIL fp;
    FRESULT result;
    char filename[50];
    uint8_t tmp_a;

    // For timing
    // gpio_set_pin_level(D10, true);

    _initialize_buffer(buffer);

    // By setting these early, we ensuring the buffering code sees this block
    // even if the reads fail and we don't have any data for it.
    // For example, track 1 block 0000 doesn't exist; we don't want to send data, but we
    // also don't want to loop forever trying to read it from the SD card.
    buffer->track = track;
    buffer->block_id = block_id;

    /*
     *
     * DEBUG DEBUG DEBUG
     *
     *
     */
    snprintf(filename, 49, "%i/%04i.bin", track, block_id);
    // snprintf(filename, 49, "%i/%04i.bin", 1, 1);
    result = f_open(&fp, filename, FA_READ);
    if(result != FR_OK) {
        // Error handle
        return 0;
    }

    result = f_read(&fp, buffer->block, BLOCK_SIZE, &buffer->length);
    if(result != FR_OK) {
        // Error handle
        f_close(&fp);
        return 0;
    }

    /*
     *  Byte-swap and invert bits to get output polarity right.
     *  SD-1C904 says "LOW=LOGICAL ONE"
     *
     */
    for(int i = 0; i < (buffer->length - 1) ; i += 2) {
        tmp_a = buffer->block[i + 1];
        buffer->block[i + 1] = ~buffer->block[i];
        buffer->block[i] = ~tmp_a;
    }

    f_close(&fp);

    // For timing
    // gpio_set_pin_level(D10, false);

    return buffer->length;
}


void send_block(int track, int block_id) {
    struct io_descriptor *io;
    struct transfer_buffer *buffer_to_send;

    if((block_id == bufferA.block_id) && (track == bufferA.track)) {
        buffer_to_send = &bufferA;
        spi_m_dma_register_callback(_SPI, SPI_M_DMA_CB_TX_DONE, tx_complete_cb_bufferA);
    } else if ((block_id == bufferB.block_id) &&
               (track == bufferB.track)) {
        buffer_to_send = &bufferB;
        spi_m_dma_register_callback(_SPI, SPI_M_DMA_CB_TX_DONE, tx_complete_cb_bufferB);
    } else {
        return;
    }

    // If the block is empty, we don't want to lock the buffer and
    // set DMA running, or we won't be able to clear it.
    if(buffer_to_send->length == 0) {
        return;
    }

    buffer_to_send->locked = true;
    spi_m_dma_get_io_descriptor(_SPI, &io);

    // Doing some things to try and clear the send buffer, but
    // it doesn't seem to work.
    //spi_m_dma_disable(_SPI);
    //spi_m_dma_deinit(_SPI);
    //

    // Active low
    // The CTTC is sensitive to this timing, have to set this before the io_write.
    CRITICAL_SECTION_ENTER();
    gpio_set_pin_level(DATDET0, false);

    // spi_m_dma_init(_SPI, SERCOM1);
    io_write(io, buffer_to_send->block, buffer_to_send->length);
    spi_m_dma_enable(_SPI); 
    CRITICAL_SECTION_LEAVE();


    _dma_running = true;

}

void cancel_transfer() {
    // When interrupting the transfer this way, re-enabling DMA later seemed to continue
    // sending the previous block that was interrupted. Not sure how to stop that from
    // happening; easier just to transmit the entire block into the void.
    // spi_m_dma_disable(_SPI);
    //  spi_m_dma_deinit(_SPI);
    _dma_running = false;
    bufferA.locked = false;
    bufferB.locked = false;
}

bool dma_running() {
    return _dma_running;
}

static void tx_complete_cb_bufferA(struct _dma_resource *resource) {
    // 95 was right after clocking ended
    // delay_us(300);
    // gpio_set_pin_level(DATDET0, true);
    _dma_running = false;
    _initialize_buffer(&bufferA);
}

static void tx_complete_cb_bufferB(struct _dma_resource *resource) {
    // 95 was right after clocking ended
    // delay_us(300);
    // gpio_set_pin_level(DATDET0, true);
    _dma_running = false;
    _initialize_buffer(&bufferB);
}

