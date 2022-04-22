#ifndef LCD_BUTTON_H
#define LCD_BUTTON_H

#include "stm32l1xx_hal.h"
#include "ILI9341_STM32_Driver.h"
#include "fatfs.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void Button_Wait(uint16_t color, uint16_t bgcolor);
void Display_Card(char string[]);
int Generate_Random(void);
void Increment_Count(void);
int change_det(int array[64]);

extern FATFS fs;
extern FIL fil;
//extern FRESULT fres;
extern int count;
extern int led_status;

#endif
