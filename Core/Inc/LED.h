#ifndef LED_H
#define LED_H

#include <string.h>
#include <math.h>
#include <stdio.h>
#include "LCD_Button.h"

#define MAX_LED 64
#define USE_BRIGHTNESS 1
#define PI 3.14159265

#define timer_freq 32.0  //timer clock freq in MHz
#define T0H 0.4  //each different clone can have their own timings
#define T1H 0.8  //timing here are in us
#define T0L 0.85
#define T1L 0.45
#define Treset 50
#define NUM_LED 64
#define LED_RED 0x0000FF00
#define LED_BLUE 0x000000FF
#define LED_GREEN 0x00FF0000
#define LED_YELLOW 0x00FFFF00
#define LED_OFF 0x00000000

extern TIM_HandleTypeDef htim2;

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim);
void Set_LED_Color(int LEDnum, uint32_t colorData, int brightness);
void Set_LED_Off(int LEDnum, uint32_t colorData, int brightness);
uint8_t Set_Brightness(uint8_t value, int brightness);
void Send_LED(void);
void Send_LED_OFF(void);
void LED_Init(void);
void clear_off(void);

#endif
