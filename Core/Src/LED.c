#include "LED.h"

uint8_t LED_data[NUM_LED*3]; //I have strip with 60 LEDs and need 3 bytes/LED
uint32_t LED_data1[NUM_LED];
uint32_t ttt[NUM_LED*24 + 4];
uint32_t off[NUM_LED*24 + 4];
uint32_t turn[2][4] = {
						{LED_RED, LED_YELLOW, LED_GREEN, LED_BLUE},
						{RED, YELLOW, GREEN, BLUE}
					  };

uint16_t pwmData[24];
uint16_t pwmData1[24];
uint8_t LED_Data[MAX_LED][4];
uint8_t LED_Mod[MAX_LED][4]; //brightness
int datasentflag = 0;
uint16_t pos;
uint8_t mask = 0b10000000;
uint8_t lastbit;
long double period;
uint16_t low_CCR1, low_ARR, high_CCR1, high_ARR, treset_ARR;

int led_board[64] =    { 6, 8, 9,10,11,12,13,14,
		                15,16,21,22,24,25,26,27,
						28,29,30,31,32,37,38,40,
						41,42,43,44,45,46,47,48,
						53,54,56,57,58,59,60,61,
						62,63, 4, 5, 7,23,39,55,
						 3, 2, 1, 0,17,18,19,20,
						33,34,35,36,49,50,51,52};

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim) {
	HAL_TIM_PWM_Stop_DMA(&htim2, TIM_CHANNEL_2);
	datasentflag = 1;
}

void Set_LED_Color(int LEDnum, uint32_t colorData, int brightness) {
	LEDnum = led_board[LEDnum];
	uint32_t newColor = colorData;
	#if USE_BRIGHTNESS
		int green = Set_Brightness((colorData & LED_GREEN) >> 16, brightness);
		int red = Set_Brightness((colorData & LED_RED) >> 8, brightness);
		int blue = Set_Brightness((colorData & LED_BLUE), brightness);

		newColor = (green << 16) | (red << 8) | blue;
    #endif

	for (int i = 23; i >= 0; i--) {
		if (newColor&(1<<i)) ttt[LEDnum*24 + (23-i) + 2] = 26;
		else ttt[LEDnum*24 + (23-i) + 2] = 13;
	}
}

void clear_off(void) {
	for (int i = 0; i < NUM_LED*24 + 4; i++) {
		off[i] = 13;
	}

	off[0] = 0;
	off[1] = 0;
	off[NUM_LED*24+4-1] = 0;
    off[NUM_LED*24+4-2] = 0;
}

void Set_LED_Off(int LEDnum, uint32_t colorData, int brightness) {
	LEDnum = led_board[LEDnum];
	uint32_t newColor = colorData;
	#if USE_BRIGHTNESS
		int green = Set_Brightness((colorData & LED_GREEN) >> 16, brightness);
		int red = Set_Brightness((colorData & LED_RED) >> 8, brightness);
		int blue = Set_Brightness((colorData & LED_BLUE), brightness);
		newColor = (green << 16) | (red << 8) | blue;
    #endif

	for (int i = 23; i >= 0; i--) {
		if (newColor&(1<<i)) off[LEDnum*24 + (23-i) + 2] = 26;
		else off[LEDnum*24 + (23-i) + 2] = 13;
	}
}

uint8_t Set_Brightness(uint8_t value, int brightness) { //0-45
	//255 -> 100%, 127 -> 50%, 63 ->  25%
#if USE_BRIGHTNESS
	if (brightness > 45) brightness = 45;
	//for (int i = 0; i < NUM_LED*24+4; i++) {
			float angle = 90 - brightness; //in degrees
			angle = angle * PI / 180; //in radian
			return (value/(tan(angle)));
			//ttt[i] = (LED_Data[i][j])/(tan(angle));
	//}
#endif
  return 0;
}

void Send_LED(void) {
	//for (int i = 0; i < 50; i++) {
		  //Send_LED();
		  //HAL_Delay(50);
		  HAL_TIM_PWM_Start_DMA(&htim2, TIM_CHANNEL_2, (uint32_t*)&ttt, NUM_LED*24+4);
	//}
}

void Send_LED_OFF(void) {
	//for (int i = 0; i < 50; i++) {
		  //Send_LED();
		  //HAL_Delay(50);
		  HAL_TIM_PWM_Start_DMA(&htim2, TIM_CHANNEL_2, (uint32_t*)&off, NUM_LED*24+4);
	//}
}

void LED_Init(void) {
	  for (int i = 2; i < NUM_LED*24-2; i++) {
	  	  ttt[i] = 13;
	  	  off[i] = 13;
	  }
	  ttt[0] = 0;
	  ttt[1] = 0;
	  ttt[NUM_LED*24+4-1] = 0;
	  ttt[NUM_LED*24+4-2] = 0;

	  off[0] = 0;
	  off[1] = 0;
	  off[NUM_LED*24+4-1] = 0;
	  off[NUM_LED*24+4-2] = 0;
}
