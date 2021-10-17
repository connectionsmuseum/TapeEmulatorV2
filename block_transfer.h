
#include <hal_spi_m_dma.h>

void init_block_buffer(struct spi_m_dma_descriptor *SPI);
void load_next_block(int track, int block_id);
void send_block(int track, int block_id, int end_position);
void cancel_transfer();
bool dma_running();


