/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fatfs.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
//LCD and Button
#include "ILI9341_STM32_Driver.h"
#include "LCD_Button.h"
//Audio
#include "audio.h"
//LEDs
#include "LED.h"
//Game
#include "game.h"
//Standard Libraries
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
 DAC_HandleTypeDef hdac;
DMA_HandleTypeDef hdma_dac_ch1;

SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;
DMA_HandleTypeDef hdma_spi1_tx;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim6;
TIM_HandleTypeDef htim7;
DMA_HandleTypeDef hdma_tim2_ch2_ch4;

/* USER CODE BEGIN PV */
//Button
int count = 0;
int card = 0;
//LCD
extern const Picture *sorry_logo;
char *cards[] = {"sorry_0_16.bmp", "sorry_1_16.bmp", "sorry_2_16.bmp", "sorry_3_16.bmp", "sorry_4_16.bmp",
		            "sorry_5_16.bmp", "sorry_6_16.bmp", "sorry_7_16.bmp", "sorry_8_16.bmp", "sorry_9_16.bmp",
					"sorry_10_16.bmp"};
//SD Card
FATFS fs;
FIL fil;
FRESULT fres;
//Audio
char *audio_cards[] = {"sorry0audio.wav", "sorry1audio.wav", "sorry2audio.wav", "sorry3audio.wav", "sorry4audio.wav",
		"sorry5audio.wav", "sorry6audio.wav", "sorry7audio.wav", "sorry8audio.wav", "sorry9audio.wav", "sorry10audio.wav"};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_SPI1_Init(void);
static void MX_SPI2_Init(void);
static void MX_DAC_Init(void);
static void MX_TIM6_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM7_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint16_t wav440Hz[] = {
		  0x07ff, 0x083f, 0x087e, 0x08be, 0x08fc, 0x093a, 0x0976, 0x09b1, 0x09ea, 0x0a22, 0x0a57, 0x0a89, 0x0aba, 0x0ae7, 0x0b11, 0x0b39,
		  0x0b5d, 0x0b7e, 0x0b9b, 0x0bb5, 0x0bcb, 0x0bdd, 0x0beb, 0x0bf5, 0x0bfb, 0x0bfd, 0x0bfc, 0x0bf6, 0x0bec, 0x0bde, 0x0bcd, 0x0bb7,
		  0x0b9e, 0x0b81, 0x0b61, 0x0b3d, 0x0b16, 0x0aec, 0x0abf, 0x0a8f, 0x0a5d, 0x0a28, 0x09f1, 0x09b8, 0x097d, 0x0941, 0x0903, 0x08c5,
		  0x0886, 0x0846, 0x0806, 0x07c7, 0x0787, 0x0747, 0x0709, 0x06cb, 0x068f, 0x0653, 0x061a, 0x05e3, 0x05ad, 0x057a, 0x054a, 0x051c,
		  0x04f1, 0x04c9, 0x04a5, 0x0484, 0x0466, 0x044c, 0x0436, 0x0423, 0x0415, 0x040a, 0x0403, 0x0401, 0x0402, 0x0407, 0x0411, 0x041e,
		  0x042f, 0x0444, 0x045d, 0x0479, 0x0499, 0x04bd, 0x04e3, 0x050d, 0x053a, 0x0569, 0x059c, 0x05d0, 0x0607, 0x0640, 0x067a, 0x06b6,
		  0x06f4, 0x0732, 0x0771, 0x07b1,
};

//Sensors
int array[64] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
int answer_sensor;
//LEDs
char toggle;
int led_status;
int yes = 0;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM7) {
		if (led_status == 1) {
			HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_1); //blink LED
			//Send_LED();
			//yes = 2;
		} else if (led_status == 2) {
			Send_LED();
			led_status = 3;
		} else if (led_status == 3) {
			Send_LED_OFF();
			led_status = 2;
		} else if (led_status == 4) {
			Send_LED();
		}
		Increment_Count();
	}
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_SPI1_Init();
  MX_SPI2_Init();
  MX_FATFS_Init();
  MX_DAC_Init();
  MX_TIM6_Init();
  MX_TIM2_Init();
  MX_TIM7_Init();
  /* USER CODE BEGIN 2 */


  //Inits
  HAL_TIM_Base_Start(&htim6); //Audio
  HAL_TIM_Base_Start_IT(&htim7);
  //Blinking with 1Hz
  //Testing sine wave: uncomment, change to circular DMA, change sampling frequency to 44100 (726-1)
  //HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
  //HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (uint32_t *)wav440Hz, 100, DAC_ALIGN_12B_R); //48
  ILI9341_Init(); //LCD
  HAL_Delay(500);
  //All LEDs
  LED_Init(); //WS2812B
  led_status = 0; //Push Button

  //Setup logo
  ILI9341_SetRotation(SCREEN_HORIZONTAL_2);
  ILI9341_FillScreen(WHITE);
  ILI9341_DrawPicture(10,75,(const Picture *)&sorry_logo);
  Button_Wait(BLACK, WHITE);

  //Game
  int turn;
  /*
  int pawns[4][4] = {{44, 45, 46, 47},
                     {44, 45, 46, 47},
                     {44, 45, 46, 47},
                     {44, 45, 46, 47}};
*/
  extern int pawns[4][4];
  extern pawn_t pawns_init[4];
  extern bool endgame;
  extern bool done[4];
  int start[4] = {4, 4, 4, 4};
  int home[4]  = {0, 0, 0, 0};
  int colorlcd[4] = {RED, YELLOW, GREEN, BLUE};
  extern int led[64];
  extern int led_num;
  int test_num = 0;
  int test[16] = {1, 2, 1, 2, 6, 6, 6, 6};

  turn = 0;
  initialize_board();
  populate_led_array();
  led_status = 4;
  Audio_Card("sorry_bump.wav");
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  //Sensor
	  //answer_sensor = change_det(array);
	  ILI9341_FillScreen(colorlcd[turn]);
	  HAL_Delay(250);
	  Button_Wait(BLACK, colorlcd[turn]);
	  //card = Generate_Random();
	  card = test[test_num];
	  Display_Card(cards[card]);
	  Audio_Card(audio_cards[card]);
	  //int check_seven = 0;

begin:
	zero_led(0);
	led_num = 0;
	int curr_loc;
	curr_loc = 0;
	//Populate array with current pawn positions
	    for (int i = 0; i < 4; i++) {
	        led[led_num] = pawns[i][turn];
	        led_num++;
	    }
	//Populate array with possible pawn positions
	if (card == 1 || card == 2) {
        if (start[turn] != 0 && !pos_occ(pawns_init[turn].after_start, turn)) {
            led[led_num] = pawns_init[turn].after_start;
            led_num++;
        }
    }
    if (card == 1 || card == 2 || card == 3 || card == 5 || card == 8 || card == 9) {
        increment_card(card, curr_loc, turn);
    } else if (card == 4) {
        decrement_card(card, curr_loc, turn);
    } else if (card == 10) {
        increment_card(card, curr_loc, turn);
        decrement_card(1, curr_loc, turn); //backwards 1
    } else if (card == 0) { //sorry card
    	if (start[turn] == 0)
    		break;
    	for (int i = 0; i < 4; i++) {
    	   if (i == turn)
    	      continue;
         	for (int j = 0; j < 4; j++) {
    	      if (pawns[j][i] <= 43 && pawns[j][i] >= 0) {
    	    	led[led_num++] = pawns[j][i];
    	      }
    	    }
    	}
    } else if (card == 6 && start[turn] != 4) {
    	increment_card(card, curr_loc, turn);
    	for (int i = 0; i < 4; i++) {
    	    if (i == turn)
    	    	 continue;
    	    for (int j = 0; j < 4; j++) {
    	    	 if (pawns[j][i] <= 43 && pawns[j][i] >= 0) {
    	    	   	led[led_num++] = pawns[j][i];
    	         }
      	    }
       	}
    } else if (card == 7 && start[turn] != 4) {
    	/*
    	if (start[turn] == 3) //1 pawn is out
    		increment(card, curr_loc, turn);
    	else if (start[turn] == 2) { //more than two pawns are out
    		for (int j = 0)
    	}*/
    }


    //No possible moves
    if (led_num == 4) {
    	Audio_Card("nomoves.wav");
        goto next_turn;
    }

    //Display LEDs
    print_led(led_num, turn);

    int change;
    int put_back;
    //Wait for pawn to be lifted
move_pawn:
    change = change_det(array);

    //Check valid sensor
    if (!valid_lift(change)) {
wrong_put_back:
		Audio_Card("incorrectmove.wav");
		HAL_Delay(500);
        //Place pawn back
put_pawn_back:
        put_back = change_det(array);
        if (put_back != change)
            goto wrong_put_back;
        else
            goto move_pawn;
    }

    //Check which pawn moved
    int pawn_change;
    for (pawn_change = 0; pawn_change < 4; pawn_change++) {
        if (pawns[pawn_change][turn] == change)
            break;
    }

 lift:
        //Display possible positions for only that pawn
        if (card == 1 || card == 2 || card == 3 || card == 5 || card == 8 || card == 9 || card == 6) {
            zero_led(5);
            led_num = 5;
            if (change == pawns_init[turn].start) {
                if (!pos_occ(pawns_init[turn].after_start, turn) && (card == 1 || card == 2))
                    led[4] = pawns_init[turn].after_start;
                else {
                    led_num = 4;
                    print_led(led_num, turn);
                    goto put_pawn_back;
                }
            } else if ((change >= (pawns_init[turn].before_stretch-(card-1))) && (change <= (pawns_init[turn].before_stretch)) && !pos_occ(change+pawns_init[turn].start_stretch-pawns_init[turn].before_stretch+(card-1), turn)) {
                led[4] = change+pawns_init[turn].start_stretch-pawns_init[turn].before_stretch+(card-1);
            } else if ((change >= (44-card) && change <= 43) && !pos_occ(change-(44-card), turn)) {
                led[4] = change-(44-card);
            } else if (!pos_occ(change+card, turn)) {
                led[4] = change+card;
            } else
                led_num = 4;

            if (card == 6) {
            	for (int i = 0; i < 4; i++) {
            	    if (i == turn)
            	      continue;
            	   	for (int j = 0; j < 4; j++) {
               	      if (pawns[j][i] <= 43 && pawns[j][i] >= 0) {
            	    	led[led_num++] = pawns[j][i];
        	   	      }
         	   	    }
       	    	}
            }
        } else if (card == 4) {
            zero_led(4);
            led_num = 4;
            if (change == pawns_init[turn].start) {
                print_led(led_num, turn);
                goto put_pawn_back;
            } else if ((change >= 0 && change < card)) {
                if (!pos_occ(change+(44-card), turn))
                    led[led_num++] = change+(44-card);
            } else if ((change >= (pawns_init[turn].start_stretch)) && (change < (pawns_init[turn].home))) {
                if (!pos_occ(change+pawns_init[turn].before_stretch-pawns_init[turn].start_stretch-(card-1), turn))
                    led[led_num++] = change+pawns_init[turn].start_stretch-pawns_init[turn].before_stretch+(card-1);
            } else if (!pos_occ(change-card, turn)) {
                led[led_num++] = change-card;
            }
        } else if (card == 10) {
            zero_led(4);
            led_num = 4;
            if (change == pawns_init[turn].start) {
                led_num = 4;
                print_led(led_num, turn);
                goto put_pawn_back;
            } else if ((change >= (pawns_init[turn].before_stretch-(card-1))) && (change <= (pawns_init[turn].before_stretch)) && !pos_occ(change+pawns_init[turn].start_stretch-pawns_init[turn].before_stretch+(card-1), turn)) { //Forward 10
                led[led_num] = change+pawns_init[turn].start_stretch-pawns_init[turn].before_stretch+(card-1);
                led_num++;
            } else if ((change >= (44-card) && change <= 43) && !pos_occ(change-(44-card), turn)) {
                led[led_num] = change-(44-card);
                led_num++;
            } else if (!pos_occ(change+card, turn)) {
                led[led_num] = change+card;
                led_num++;
            }
            //Backward 1
            if ((change >= (pawns_init[turn].start_stretch)) && (change < (pawns_init[turn].home)) && !pos_occ(change+pawns_init[turn].before_stretch-pawns_init[turn].start_stretch-(1-1), turn)) {
                led[led_num++] = change+pawns_init[turn].start_stretch-pawns_init[turn].before_stretch+(1-1);
            } else if ((change >= 0 && change < 1) && !pos_occ(change+(44-1), turn)) {
                led[led_num++] = change+(44-1);
            } else if (!pos_occ(change-1, turn)) {
                led[led_num++] = change-1;
            }
        }

        print_led(led_num, turn);

        //Wait for pawn to be placed
        int next_change;
        next_change = change_det(array);

        //Check if pawn moved pack from where it was placed
        if (change == next_change) {
           zero_led(4);
           goto begin;
        }
        //Check for valid play
            bool in_arr = false;
            int i;
            for (i = 4; i < led_num; i++) {
                if (led[i] == next_change) {
                    in_arr = true;
                    break;
                }
            }
            if (!in_arr) {
                Audio_Card("incorrectmove.wav");
                goto lift;
            } else if (card == 6 && !((next_change == change-(44-card)) || (next_change == change+pawns_init[turn].start_stretch-pawns_init[turn].before_stretch+(card-1))
            		                   || (next_change == change+card))) { //check if pawn moved home: assume if move 6 overlaps with diff pawn, then assume it goes home and not change
            	HAL_Delay(1000);
            	HAL_Delay(1000);
            	HAL_Delay(1000);
            	int check_change;
six:
            	check_change = change_det(array); //wait for opponent pawn to move to current player pos.
            	if (check_change != change) {
            		Audio_Card("incorrectmove.wav");
            		HAL_Delay(500);
            		goto six;
            	}
            	for (int i = 0; i < 4; i++) {
            	   if (i == turn)
            	     continue;
            	   for (int j = 0; j < 4; j++) {
            	     if (pawns[j][i] == next_change) {
            	    	pawns[j][i] = pawns[pawn_change][turn];
             			break;
            	     }
            	  }
            	}
            	pawns[pawn_change][turn] = led[i];
            	HAL_Delay(1000);
            	HAL_Delay(1000);
            	HAL_Delay(1000);
            } else {
                if (pawns[pawn_change][turn] == pawns_init[turn].start)
                    start[turn]--;
                //check if bumped back;
                for (int i = 0; i < 4; i++) {
                	if (i == turn)
                		continue;
                	for (int j = 0; j < 4; j++) {
                		if (pawns[j][i] == next_change) {
                			pawns[j][i] = pawns_init[i].start;
                			start[i]++;
                			Audio_Card("sorry_bump.wav");
                			HAL_Delay(1000);
                			break;
                		}
                	}
                }
                pawns[pawn_change][turn] = led[i];
            }

            if (led[i] == pawns_init[turn].home) {
            	Audio_Card("yourehome.wav");
                home[turn]++;
                if (home[turn] == 4) {
                	done[turn] =  true;
                	Audio_Card("youwin.wav");
                }
            }


      next_turn:
          turn = update_turn(turn);

          populate_led_array();
          //print_led(led_num);
          led_status = 4;
          if (test_num == 7)
        	  test_num = 0;
          else
        	  test_num++;

      if (endgame) {
    	  ILI9341_FillScreen(BLACK);
    	  break;
      }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  RCC_OscInitStruct.PLL.PLLDIV = RCC_PLL_DIV3;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief DAC Initialization Function
  * @param None
  * @retval None
  */
static void MX_DAC_Init(void)
{

  /* USER CODE BEGIN DAC_Init 0 */

  /* USER CODE END DAC_Init 0 */

  DAC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN DAC_Init 1 */

  /* USER CODE END DAC_Init 1 */

  /** DAC Initialization
  */
  hdac.Instance = DAC;
  if (HAL_DAC_Init(&hdac) != HAL_OK)
  {
    Error_Handler();
  }

  /** DAC channel OUT1 config
  */
  sConfig.DAC_Trigger = DAC_TRIGGER_T6_TRGO;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DAC_Init 2 */

  /* USER CODE END DAC_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 40-1;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

/**
  * @brief TIM6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM6_Init(void)
{

  /* USER CODE BEGIN TIM6_Init 0 */

  /* USER CODE END TIM6_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM6_Init 1 */

  /* USER CODE END TIM6_Init 1 */
  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 1-1;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 2000-1;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM6_Init 2 */

  /* USER CODE END TIM6_Init 2 */

}

/**
  * @brief TIM7 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM7_Init(void)
{

  /* USER CODE BEGIN TIM7_Init 0 */

  /* USER CODE END TIM7_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM7_Init 1 */

  /* USER CODE END TIM7_Init 1 */
  htim7.Instance = TIM7;
  htim7.Init.Prescaler = 32000-1;
  htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim7.Init.Period = 500-1;
  htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM7_Init 2 */

  /* USER CODE END TIM7_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel2_IRQn);
  /* DMA1_Channel3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);
  /* DMA1_Channel7_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel7_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel7_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(B_LED_GPIO_Port, B_LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, SPI1_CS_Pin|SPI1_DC_Pin|SPI1_RST_Pin|SENSOR1_OUT_Pin
                          |SENSOR2_OUT_Pin|SENSOR3_OUT_Pin|SENSOR4_ON_Pin|SENSOR5_ON_Pin
                          |SENSOR6_ON_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(SPI2_CS_GPIO_Port, SPI2_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B_LED_Pin */
  GPIO_InitStruct.Pin = B_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(B_LED_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : B_IN_Pin */
  GPIO_InitStruct.Pin = B_IN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B_IN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : SPI1_CS_Pin SPI1_DC_Pin SPI1_RST_Pin SENSOR1_OUT_Pin
                           SENSOR2_OUT_Pin SENSOR3_OUT_Pin SENSOR4_ON_Pin SENSOR5_ON_Pin
                           SENSOR6_ON_Pin */
  GPIO_InitStruct.Pin = SPI1_CS_Pin|SPI1_DC_Pin|SPI1_RST_Pin|SENSOR1_OUT_Pin
                          |SENSOR2_OUT_Pin|SENSOR3_OUT_Pin|SENSOR4_ON_Pin|SENSOR5_ON_Pin
                          |SENSOR6_ON_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : SPI2_CS_Pin */
  GPIO_InitStruct.Pin = SPI2_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SPI2_CS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : SENSOR_IN_Pin */
  GPIO_InitStruct.Pin = SENSOR_IN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(SENSOR_IN_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
