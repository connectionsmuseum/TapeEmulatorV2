#include <atmel_start.h>
#include "usb_start.h"
#include "sd_mmc.h"
#include "fatFS/ff.h"

static void tx_complete(struct _dma_resource *resource) {
    // gpio_set_pin_level(PB04GREEN, true);
    return;
}

static const uint8_t debug_buf[] = "some nonsense text";

static sd_mmc_err_t sd_err;
static sd_mmc_err_t sd_err_init;

FATFS FatFs;

bool single_block_read() {

    uint8_t sd_mmc_block[512];
    char usb_printbuf[200];

    if (sd_mmc_get_type(0) & (CARD_TYPE_SD | CARD_TYPE_MMC)) {
        /* Read card block 0 */
        sd_err_init = sd_mmc_init_read_blocks(0, 0, 1);
        sd_err = sd_mmc_start_read_blocks(sd_mmc_block, 1);
        sd_err = sd_mmc_wait_end_of_read_blocks(false);

        snprintf(usb_printbuf, 99, "First bytes 0x%02x 0x%02x 0x%02x 0x%02x init %d \n\r",
                sd_mmc_block[0], sd_mmc_block[1], sd_mmc_block[2], sd_mmc_block[4], (int) sd_err_init);
        cdcdf_acm_write((uint8_t *)usb_printbuf, strlen(usb_printbuf));


    }

}

int main(void)
{
    struct io_descriptor *io;
    char usb_printbuf[200];
    uint32_t sd_cap;
    FRESULT fatfs_result;
    FILINFO file_info;
    DIR dir_obj;

    /* Initializes MCU, drivers and middleware */
    atmel_start_init();

    // spi_m_dma_register_callback(&SPI_0, SPI_M_DMA_CB_TX_DONE, tx_complete);
    spi_m_dma_get_io_descriptor(&SPI_0, &io);
    spi_m_dma_enable(&SPI_0);

        if (cdcdf_acm_is_enabled()) {
            snprintf(usb_printbuf, 99, "*** Initializing *** \n\r");
            cdcdf_acm_write((uint8_t *)usb_printbuf, strlen(usb_printbuf));
        }

    fatfs_result = f_mount(&FatFs, "", 0);

    while (1) {
        if (cdcdf_acm_is_enabled()) {
            snprintf(usb_printbuf, 99, "Loop \n\r");
            cdcdf_acm_write((uint8_t *)usb_printbuf, strlen(usb_printbuf));

        }

        if (SD_MMC_OK == sd_mmc_check(0)) {
            gpio_set_pin_level(PB04GREEN, true);
            fatfs_result = f_opendir(&dir_obj, "1");
            fatfs_result = f_readdir(&dir_obj, &file_info);

            snprintf(usb_printbuf, 99, "filename %s \n\r", file_info.fname);
            cdcdf_acm_write((uint8_t *)usb_printbuf, strlen(usb_printbuf));
            // single_block_read();

        }

        gpio_set_pin_level(PB05RED, true);
        gpio_set_pin_level(TTBOTA0, true);
        // io_write(io, debug_buf, 8);

        delay_ms(300);

        gpio_set_pin_level(PB05RED, false);
        gpio_set_pin_level(TTBOTA0, false);
        gpio_set_pin_level(PB04GREEN, false);
        // gpio_set_pin_level(PB04GREEN, false);
        delay_ms(300);
    }
}
