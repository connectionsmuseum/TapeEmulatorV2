#include <atmel_start.h>
#include "usb_start.h"
#include "sd_mmc.h"

static void tx_complete(struct _dma_resource *resource) {
    // gpio_set_pin_level(PB04GREEN, true);
    return;
}

static const uint8_t debug_buf[] = "some nonsense text";

int main(void)
{
    struct io_descriptor *io;
    char usb_printbuf[200];
    uint32_t sd_cap;

    /* Initializes MCU, drivers and middleware */
    atmel_start_init();

    // spi_m_dma_register_callback(&SPI_0, SPI_M_DMA_CB_TX_DONE, tx_complete);
    spi_m_dma_get_io_descriptor(&SPI_0, &io);
    spi_m_dma_enable(&SPI_0);

        if (cdcdf_acm_is_enabled()) {
            snprintf(usb_printbuf, 99, "*** Initializing *** \n\r");
            cdcdf_acm_write((uint8_t *)usb_printbuf, strlen(usb_printbuf));
        }

    while (1) {
        if (cdcdf_acm_is_enabled()) {
            snprintf(usb_printbuf, 99, "Loop \n\r");
            cdcdf_acm_write((uint8_t *)usb_printbuf, strlen(usb_printbuf));

        }

        if (SD_MMC_OK == sd_mmc_check(0)) {
            gpio_set_pin_level(PB04GREEN, true);
            sd_cap = sd_mmc_get_capacity(0);
            snprintf(usb_printbuf, 99, "SD Cap %lu \n\r", sd_cap);
            cdcdf_acm_write((uint8_t *)usb_printbuf, strlen(usb_printbuf));
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
