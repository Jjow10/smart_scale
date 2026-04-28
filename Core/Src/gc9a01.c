#include "gc9a01.h"
#include "stm32f4xx_hal_spi.h"

static SPI_HandleTypeDef *_hspi;

static inline void CS_LOW(void){
    HAL_GPIO_WritePin(GC9A01_CS_PORT, GC9A01_CS_PIN, GPIO_PIN_RESET);
}

static inline void CS_HIGH(void){
    HAL_GPIO_WritePin(GC9A01_CS_PORT, GC9A01_CS_PIN, GPIO_PIN_SET);
}

static inline void DC_LOW(void){
    HAL_GPIO_WritePin(GC9A01_DC_PORT, GC9A01_DC_PIN, GPIO_PIN_RESET);
}

static inline void DC_HIGH(void){
    HAL_GPIO_WritePin(GC9A01_DC_PORT, GC9A01_DC_PIN, GPIO_PIN_SET);
}

static inline void RESET_LOW(void){
    HAL_GPIO_WritePin(GC9A01_RESET_PORT, GC9A01_RESET_PIN, GPIO_PIN_RESET);
}

static inline void RESET_HIGH(void){
    HAL_GPIO_WritePin(GC9A01_RESET_PORT, GC9A01_RESET_PIN, GPIO_PIN_SET);
}

static void GC9A01_SendCmd(uint8_t cmd)
{
    DC_LOW();
    CS_LOW();
    HAL_SPI_Transmit(_hspi, &cmd, 1, HAL_MAX_DELAY);
    CS_HIGH();
}
 
/*
 * Send a single data byte.
 * DC must be HIGH for data bytes.
 */
static void GC9A01_SendData8(uint8_t data)
{
    DC_HIGH();
    CS_LOW();
    HAL_SPI_Transmit(_hspi, &data, 1, HAL_MAX_DELAY);
    CS_HIGH();
}
 
/*
 * Send a 16-bit value as two bytes (big-endian).
 * Used for pixel colour data (RGB565).
 */
static void GC9A01_SendData16(uint16_t data)
{
    uint8_t buf[2];
    buf[0] = (data >> 8) & 0xFF;    // High byte first
    buf[1] = data & 0xFF;
    DC_HIGH();
    CS_LOW();
    HAL_SPI_Transmit(_hspi, buf, 2, HAL_MAX_DELAY);
    CS_HIGH();
}
 
/*
 * Hardware reset sequence.
 * RST low for 10ms, then high and wait 120ms for the controller to settle.
 */
static void GC9A01_HardReset(void)
{
    RESET_HIGH();
    HAL_Delay(10);
    RESET_LOW();
    HAL_Delay(10);
    RESET_HIGH();
    HAL_Delay(120);
}

void GC9A01_Init(SPI_HandleTypeDef *hspi)
{
    _hspi = hspi;
 
    /* 1. Hardware reset */
    GC9A01_HardReset();
 
    /* 2. Software reset */
    GC9A01_SendCmd(GC9A01_CMD_SWRESET);
    HAL_Delay(150);
 
    /* 3. Manufacturer / power-on registers
     *    These values come from the GC9A01 recommended init sequence.
     *    They configure internal power rails, oscillator, and gamma.
     *    You don't need to fully understand each one — treat them as a
     *    required "magic sequence" to wake the controller up properly.
     */
    GC9A01_SendCmd(0xEF);
 
    GC9A01_SendCmd(0xEB);
    GC9A01_SendData8(0x14);
 
    GC9A01_SendCmd(0xFE);           // Inter register enable 1
    GC9A01_SendCmd(0xEF);           // Inter register enable 2
 
    GC9A01_SendCmd(0xEB);
    GC9A01_SendData8(0x14);
 
    GC9A01_SendCmd(0x84);
    GC9A01_SendData8(0x40);
 
    GC9A01_SendCmd(0x85);
    GC9A01_SendData8(0xFF);
 
    GC9A01_SendCmd(0x86);
    GC9A01_SendData8(0xFF);
 
    GC9A01_SendCmd(0x87);
    GC9A01_SendData8(0xFF);
 
    GC9A01_SendCmd(0x88);
    GC9A01_SendData8(0x0A);
 
    GC9A01_SendCmd(0x89);
    GC9A01_SendData8(0x21);
 
    GC9A01_SendCmd(0x8A);
    GC9A01_SendData8(0x00);
 
    GC9A01_SendCmd(0x8B);
    GC9A01_SendData8(0x80);
 
    GC9A01_SendCmd(0x8C);
    GC9A01_SendData8(0x01);
 
    GC9A01_SendCmd(0x8D);
    GC9A01_SendData8(0x01);
 
    GC9A01_SendCmd(0x8E);
    GC9A01_SendData8(0xFF);
 
    GC9A01_SendCmd(0x8F);
    GC9A01_SendData8(0xFF);
 
    GC9A01_SendCmd(0xB6);           // Display Function Control
    GC9A01_SendData8(0x00);
    GC9A01_SendData8(0x00);
 
    /* 4. Pixel format: 16-bit RGB565 (0x55) */
    GC9A01_SendCmd(GC9A01_CMD_COLMOD);
    GC9A01_SendData8(0x55);
    HAL_Delay(10);
 
    GC9A01_SendCmd(0x90);
    GC9A01_SendData8(0x08);
    GC9A01_SendData8(0x08);
    GC9A01_SendData8(0x08);
    GC9A01_SendData8(0x08);
 
    GC9A01_SendCmd(0xBD);
    GC9A01_SendData8(0x06);
 
    GC9A01_SendCmd(0xBC);
    GC9A01_SendData8(0x00);
 
    GC9A01_SendCmd(0xFF);
    GC9A01_SendData8(0x60);
    GC9A01_SendData8(0x01);
    GC9A01_SendData8(0x04);
 
    GC9A01_SendCmd(0xC3);           // Power Control 1
    GC9A01_SendData8(0x13);
 
    GC9A01_SendCmd(0xC4);           // Power Control 2
    GC9A01_SendData8(0x13);
 
    GC9A01_SendCmd(0xC9);           // Power Control 3
    GC9A01_SendData8(0x22);
 
    GC9A01_SendCmd(0xBE);
    GC9A01_SendData8(0x11);
 
    GC9A01_SendCmd(0xE1);
    GC9A01_SendData8(0x10);
    GC9A01_SendData8(0x0E);
 
    GC9A01_SendCmd(0xDF);
    GC9A01_SendData8(0x21);
    GC9A01_SendData8(0x0C);
    GC9A01_SendData8(0x02);
 
    /* Gamma settings */
    GC9A01_SendCmd(0xF0);           // SET_GAMMA1
    GC9A01_SendData8(0x45);
    GC9A01_SendData8(0x09);
    GC9A01_SendData8(0x08);
    GC9A01_SendData8(0x08);
    GC9A01_SendData8(0x26);
    GC9A01_SendData8(0x2A);
 
    GC9A01_SendCmd(0xF1);           // SET_GAMMA2
    GC9A01_SendData8(0x43);
    GC9A01_SendData8(0x70);
    GC9A01_SendData8(0x72);
    GC9A01_SendData8(0x36);
    GC9A01_SendData8(0x37);
    GC9A01_SendData8(0x6F);
 
    GC9A01_SendCmd(0xF2);           // SET_GAMMA3
    GC9A01_SendData8(0x45);
    GC9A01_SendData8(0x09);
    GC9A01_SendData8(0x08);
    GC9A01_SendData8(0x08);
    GC9A01_SendData8(0x26);
    GC9A01_SendData8(0x2A);
 
    GC9A01_SendCmd(0xF3);           // SET_GAMMA4
    GC9A01_SendData8(0x43);
    GC9A01_SendData8(0x70);
    GC9A01_SendData8(0x72);
    GC9A01_SendData8(0x36);
    GC9A01_SendData8(0x37);
    GC9A01_SendData8(0x6F);
 
    GC9A01_SendCmd(0xED);
    GC9A01_SendData8(0x1B);
    GC9A01_SendData8(0x0B);
 
    GC9A01_SendCmd(0xAE);
    GC9A01_SendData8(0x77);
 
    GC9A01_SendCmd(0xCD);
    GC9A01_SendData8(0x63);
 
    GC9A01_SendCmd(0x70);
    GC9A01_SendData8(0x07);
    GC9A01_SendData8(0x07);
    GC9A01_SendData8(0x04);
    GC9A01_SendData8(0x0E);
    GC9A01_SendData8(0x0F);
    GC9A01_SendData8(0x09);
    GC9A01_SendData8(0x07);
    GC9A01_SendData8(0x08);
    GC9A01_SendData8(0x03);
 
    GC9A01_SendCmd(0xE8);
    GC9A01_SendData8(0x34);
 
    GC9A01_SendCmd(0x62);
    GC9A01_SendData8(0x18);
    GC9A01_SendData8(0x0D);
    GC9A01_SendData8(0x71);
    GC9A01_SendData8(0xED);
    GC9A01_SendData8(0x70);
    GC9A01_SendData8(0x70);
    GC9A01_SendData8(0x18);
    GC9A01_SendData8(0x0F);
    GC9A01_SendData8(0x71);
    GC9A01_SendData8(0xEF);
    GC9A01_SendData8(0x70);
    GC9A01_SendData8(0x70);
 
    GC9A01_SendCmd(0x63);
    GC9A01_SendData8(0x18);
    GC9A01_SendData8(0x11);
    GC9A01_SendData8(0x71);
    GC9A01_SendData8(0xF1);
    GC9A01_SendData8(0x70);
    GC9A01_SendData8(0x70);
    GC9A01_SendData8(0x18);
    GC9A01_SendData8(0x13);
    GC9A01_SendData8(0x71);
    GC9A01_SendData8(0xF3);
    GC9A01_SendData8(0x70);
    GC9A01_SendData8(0x70);
 
    GC9A01_SendCmd(0x64);
    GC9A01_SendData8(0x28);
    GC9A01_SendData8(0x29);
    GC9A01_SendData8(0xF1);
    GC9A01_SendData8(0x01);
    GC9A01_SendData8(0xF1);
    GC9A01_SendData8(0x00);
    GC9A01_SendData8(0x07);
 
    GC9A01_SendCmd(0x66);
    GC9A01_SendData8(0x3C);
    GC9A01_SendData8(0x00);
    GC9A01_SendData8(0xCD);
    GC9A01_SendData8(0x67);
    GC9A01_SendData8(0x45);
    GC9A01_SendData8(0x45);
    GC9A01_SendData8(0x10);
    GC9A01_SendData8(0x00);
    GC9A01_SendData8(0x00);
    GC9A01_SendData8(0x00);
 
    GC9A01_SendCmd(0x67);
    GC9A01_SendData8(0x00);
    GC9A01_SendData8(0x3C);
    GC9A01_SendData8(0x00);
    GC9A01_SendData8(0x00);
    GC9A01_SendData8(0x00);
    GC9A01_SendData8(0x01);
    GC9A01_SendData8(0x54);
    GC9A01_SendData8(0x10);
    GC9A01_SendData8(0x32);
    GC9A01_SendData8(0x98);
 
    GC9A01_SendCmd(0x74);
    GC9A01_SendData8(0x10);
    GC9A01_SendData8(0x85);
    GC9A01_SendData8(0x80);
    GC9A01_SendData8(0x00);
    GC9A01_SendData8(0x00);
    GC9A01_SendData8(0x4E);
    GC9A01_SendData8(0x00);
 
    GC9A01_SendCmd(0x98);
    GC9A01_SendData8(0x3E);
    GC9A01_SendData8(0x07);
 
    GC9A01_SendCmd(GC9A01_CMD_INVON);   // Display inversion on (needed for correct colours)
    GC9A01_SendCmd(GC9A01_CMD_NORON);   // Normal display mode on
 
    /* 5. MADCTL: default orientation, RGB order
     *    Change MADCTL_BGR to MADCTL_BGR if colours appear inverted
     */
    GC9A01_SendCmd(GC9A01_CMD_MADCTL);
    GC9A01_SendData8(0x00);             // 0x00 = 0° rotation, RGB
 
    /* 6. Sleep Out — must wait 120ms after this */
    GC9A01_SendCmd(GC9A01_CMD_SLPOUT);
    HAL_Delay(120);
 
    /* 7. Display On */
    GC9A01_SendCmd(GC9A01_CMD_DISPON);
    HAL_Delay(20);
 
}

/* ─── Drawing Functions ─────────────────────────────────────────────────── */
 
/*
 * Set the active drawing window.
 * All subsequent RAMWR pixel data will fill this rectangle.
 *
 * The GC9A01 requires:
 *   CASET (0x2A): 4 bytes → start_col_hi, start_col_lo, end_col_hi, end_col_lo
 *   RASET (0x2B): 4 bytes → start_row_hi, start_row_lo, end_row_hi, end_row_lo
 *   RAMWR (0x2C): followed by pixel data
 */
void GC9A01_SetWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    // Column Address Set
    GC9A01_SendCmd(GC9A01_CMD_CASET);
    GC9A01_SendData8(x0 >> 8);
    GC9A01_SendData8(x0 & 0xFF);
    GC9A01_SendData8(x1 >> 8);
    GC9A01_SendData8(x1 & 0xFF);
 
    // Row Address Set
    GC9A01_SendCmd(GC9A01_CMD_RASET);
    GC9A01_SendData8(y0 >> 8);
    GC9A01_SendData8(y0 & 0xFF);
    GC9A01_SendData8(y1 >> 8);
    GC9A01_SendData8(y1 & 0xFF);
 
    // Memory Write — pixel data follows
    GC9A01_SendCmd(GC9A01_CMD_RAMWR);
}

void GC9A01_FillScreen(uint16_t colour)
{
    GC9A01_FillRect(0, 0, GC9A01_WIDTH, GC9A01_HEIGHT, colour);
}

void GC9A01_FillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t colour)
{
    if ((x >= GC9A01_WIDTH) || (y >= GC9A01_HEIGHT)) return;
    if ((x + w) > GC9A01_WIDTH)  w = GC9A01_WIDTH  - x;
    if ((y + h) > GC9A01_HEIGHT) h = GC9A01_HEIGHT - y;

    GC9A01_SetWindow(x, y, x + w - 1, y + h - 1);

    // Pre-fill a row buffer with the colour (big-endian RGB565)
    uint8_t buf[GC9A01_WIDTH * 2];
    for (uint16_t i = 0; i < w; i++) {
        buf[i * 2]     = colour >> 8;
        buf[i * 2 + 1] = colour & 0xFF;
    }

    // Send all rows — CS stays LOW for the entire pixel burst
    DC_HIGH();
    CS_LOW();
    for (uint16_t row = 0; row < h; row++) {
        HAL_SPI_Transmit(_hspi, buf, w * 2, HAL_MAX_DELAY);
    }
    CS_HIGH();
}
 
/*
 * Draw a single pixel at (x, y).
 * Useful for testing but slow for large areas — use FillRect instead.
 */
void GC9A01_DrawPixel(uint16_t x, uint16_t y, uint16_t colour)
{
    if (x >= GC9A01_WIDTH || y >= GC9A01_HEIGHT) return;
    GC9A01_SetWindow(x, y, x, y);
    GC9A01_SendData16(colour);
}