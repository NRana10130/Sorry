#include "LCD_Button.h"

//board 1: 44-7
//board 2: 45-18
//board 3: 46-29
//board 4: 47-40
int sensor_board[64] = {44, 0, 1, 2, 3, 4, 5, 6,
		                55,54,53,52,10, 9, 8, 7,
						45,11,12,13,14,15,16,17,
						59,58,57,56,21,20,19,18,
						46,22,23,24,25,26,27,28,
						63,62,61,60,32,31,30,29,
						47,33,34,35,36,37,38,39,
						51,50,49,48,43,42,41,40};

void Button_Wait(uint16_t color, uint16_t bgcolor) {
	ILI9341_DrawText("PRESS BUTTON", FONT4, 90, 175, color, bgcolor);
	led_status = 1;
	while (1) {
		if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2) == GPIO_PIN_SET) {
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
			led_status = 0;
			break;
		}
/*
	  if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2) == GPIO_PIN_SET) {
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
		break;
	  } else if (toggle) {
		Increment_Count();
	    ILI9341_DrawText("PRESS BUTTON", FONT4, 90, 175, color, bgcolor);
	  } else {
	    ILI9341_DrawRectangle(90, 175, 140, 19, bgcolor);
	  }

	  HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_1);
	  toggle = ~toggle;

	  HAL_Delay(250);
*/
	}
}

void Increment_Count(void) {
	  if (count == 10) {
		  count = 0;
	  } else {
		  count++;
	  }
}

int Generate_Random(void) {
	if (count == 7)
		return 1;
	//else if (count == 4 || count == 10)
	//	return 2;
	return count;
}

void Display_Card(char string[]) {
	Increment_Count();
	FRESULT fres;
	ILI9341_SetRotation(SCREEN_HORIZONTAL_2);
	ILI9341_FillScreen(WHITE);
    f_mount(&fs, "", 0);
    fres = f_open(&fil, string, FA_READ);
	fres = f_lseek(&fil, 0x0A);
	uint32_t offset;
	UINT offread;
	f_read(&fil, &offset, 4, &offread);
	fres = f_lseek(&fil, offset);
	for (int y=239; y>=0; y--){
	  for (int x=0; x<320; x++) {
		uint16_t b0;
		UINT bytesread;
		f_read(&fil, &b0, 2, &bytesread);
		if (b0 != 0xFFFF) {
          ILI9341_DrawPixel(x,y, b0);
		}
	   }
	}
    f_close(&fil);
}

//Sensors
int change_det(int array[64]) // Function that detects and returns change in sensor_state array
{
  int change;              // Flag signal - 1 signifies a change, 0 signifies no change
  change = 0;
  int change_sensor = 0;   // Change Sensor refers to sensor whose value changed
  while (change == 0) {
    for(uint8_t count = 0; count < 64; count++) {

	  // Assigns value to select lines of cascaded multiplexer
      HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6, ((1 << 3) & count)>>3);
	  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7, ((1 << 4) & count)>>4);
	  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8, ((1 << 5) & count)>>5);
	  HAL_Delay(10);

	  // Assigns value to the rest of the multiplexers
	  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,((1 << 0) & count)>>0);
	  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,((1 << 1) & count)>>1);
	  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,((1 << 2) & count)>>2);
	  //  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,0);
	  HAL_Delay(1);

	  // Checks for change
	  if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9)) {
	    if (array[count] == 0) {
		  change = 1;
		  change_sensor = count;
	    }
		array[count] = 1;
	   } else {
	     if (array[count] == 1) {
		   change = 1;
		   change_sensor = count;
	     }
	     array[count] = 0;
	   }
	  }
	}
      return sensor_board[change_sensor];         // Returns sensors whose value changed wrt board
}
