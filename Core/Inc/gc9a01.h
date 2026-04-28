#ifndef INC_GC9A01_H_
#define INC_GC9A01_H_

#include "main.h"
#include "stm32f4xx_hal_spi.h"
#include <stdio.h>
#include <stdint.h>

// Pin definitions
#define GC9A01_SCK_PORT GPIOA
#define GC9A01_SCK_PIN GPIO_PIN_5

#define GC9A01_MOSI_PORT GPIOA
#define GC9A01_MOSI_PIN GPIO_PIN_7

#define GC9A01_CS_PORT GPIOA
#define GC9A01_CS_PIN GPIO_PIN_6

#define GC9A01_DC_PORT GPIOA
#define GC9A01_DC_PIN GPIO_PIN_9

#define GC9A01_RESET_PORT GPIOC
#define GC9A01_RESET_PIN GPIO_PIN_7

// Resolution of the display
#define GC9A01_WIDTH 240
#define GC9A01_HEIGHT 240

// GC9A01 command codes
#define GC9A01_CMD_SWRESET  0x01    // Software Reset
#define GC9A01_CMD_SLPOUT   0x11    // Sleep Out
#define GC9A01_CMD_NORON    0x13    // Normal Display Mode On
#define GC9A01_CMD_INVOFF   0x20    // Display Inversion Off
#define GC9A01_CMD_INVON    0x21    // Display Inversion On
#define GC9A01_CMD_DISPOFF  0x28    // Display Off
#define GC9A01_CMD_DISPON   0x29    // Display On
#define GC9A01_CMD_CASET    0x2A    // Column Address Set
#define GC9A01_CMD_RASET    0x2B    // Row Address Set
#define GC9A01_CMD_RAMWR    0x2C    // Memory Write
#define GC9A01_CMD_MADCTL   0x36    // Memory Access Control (rotation/BGR)
#define GC9A01_CMD_COLMOD   0x3A    // Interface Pixel Format

// MADCTL bits for rotation and colour order
#define MADCTL_MY   0x80    // Row Address Order
#define MADCTL_MX   0x40    // Column Address Order
#define MADCTL_MV   0x20    // Row/Column Exchange
#define MADCTL_ML   0x10    // Vertical Refresh Order
#define MADCTL_BGR  0x08    // BGR colour order (use if colours look wrong)
#define MADCTL_MH   0x04    // Horizontal Refresh Order

#define RGB565(r, g, b) ((uint16_t)(((r & 0x1F) << 11) | ((g & 0x3F) << 5) | (b & 0x1F)))
 
// Common colours
#define COLOR_BLACK     0x0000
#define COLOR_WHITE     0xFFFF
#define COLOR_RED       0xF800
#define COLOR_GREEN     0x07E0
#define COLOR_BLUE      0x001F
#define COLOR_CYAN      0x07FF
#define COLOR_MAGENTA   0xF81F
#define COLOR_YELLOW    0xFFE0

void GC9A01_Init(SPI_HandleTypeDef *hspi);
void GC9A01_SetWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
void GC9A01_FillScreen(uint16_t colour);
void GC9A01_FillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t colour);
void GC9A01_DrawPixel(uint16_t x, uint16_t y, uint16_t colour);

SPI_HandleTypeDef* GC9A01_GetSPI(void);
void GC9A01_Flush(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t *data, uint32_t len);

#endif