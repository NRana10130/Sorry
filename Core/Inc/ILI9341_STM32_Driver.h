#ifndef ILI9341_STM32_DRIVER_H
#define ILI9341_STM32_DRIVER_H

#include "stm32l1xx_hal.h"
#include "fonts.h"

extern SPI_HandleTypeDef hspi1;

#define ILI9341_SCREEN_HEIGHT 	240
#define ILI9341_SCREEN_WIDTH 	320

/* PIN Configuration */
#define HSPI_INSTANCE			&hspi1
#define LCD_CS_PORT				GPIOB
#define LCD_CS_PIN				GPIO_PIN_10
#define LCD_DC_PORT				GPIOB
#define LCD_DC_PIN				GPIO_PIN_11
#define	LCD_RST_PORT			GPIOB
#define	LCD_RST_PIN				GPIO_PIN_12

#define BURST_MAX_SIZE 			500
#define BLACK      				0x0000
#define NAVY        			0x000F
#define DARKGREEN   			0x03E0
#define DARKCYAN    			0x03EF
#define MAROON      			0x7800
#define PURPLE      			0x780F
#define OLIVE       			0x7BE0
#define LIGHTGREY   			0xC618
#define DARKGREY    			0x7BEF
#define BLUE        			0x001F
#define GREEN       			0x07E0
#define CYAN        			0x07FF
#define RED         			0xF800
#define MAGENTA     			0xF81F
#define YELLOW      			0xFFE0
#define WHITE       			0xFFFF
#define ORANGE      			0xFD20
#define GREENYELLOW 			0xAFE5
#define PINK        			0xF81F

#define SCREEN_VERTICAL_1		0
#define SCREEN_HORIZONTAL_1		1
#define SCREEN_VERTICAL_2		2
#define SCREEN_HORIZONTAL_2		3

#define HORIZONTAL_IMAGE	0
#define VERTICAL_IMAGE		1

void ILI9341_WriteCommand(uint8_t cmd);
void ILI9341_WriteData(uint8_t data);
void ILI9341_WriteBuffer(uint8_t *buffer, uint16_t len);
void ILI9341_Reset(void);
void ILI9341_Enable(void);
void ILI9341_Init(void);
void ILI9341_SetRotation(uint8_t rotation);
void ILI9341_SetAddress(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void ILI9341_DrawColorBurst(uint16_t color, uint32_t size);
void ILI9341_FillScreen(uint16_t color);
void ILI9341_DrawPixel(uint16_t x,uint16_t y,uint16_t color);
void ILI9341_DrawRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color);
void ILI9341_DrawChar(char ch, const uint8_t font[], uint16_t X, uint16_t Y, uint16_t color, uint16_t bgcolor);
void ILI9341_DrawText(const char* str, const uint8_t font[], uint16_t X, uint16_t Y, uint16_t color, uint16_t bgcolor);

//===========================================================================
// C Picture data structure.
//===========================================================================
typedef const struct {
    unsigned int   width;
    unsigned int   height;
    unsigned int   bytes_per_pixel; // 2:RGB16, 3:RGB, 4:RGBA
    unsigned char  pixel_data[0]; // variable length array
} Picture;

void ILI9341_DrawPicture(uint16_t x0, uint16_t y0, const Picture *pic);

#endif