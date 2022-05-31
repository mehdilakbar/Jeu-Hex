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
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32746g_discovery_lcd.h"
#include "stm32746g_discovery_ts.h"
#include "stdio.h"
#include "HorombeRGB565.h"
#include "fondHex2.h"
#include "gameOver.h"
#include "paused.h"
#include <time.h>

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
ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc3;

DAC_HandleTypeDef hdac;
DMA_HandleTypeDef hdma_dac1;

DMA2D_HandleTypeDef hdma2d;

I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c3;

LTDC_HandleTypeDef hltdc;

RTC_HandleTypeDef hrtc;

SPI_HandleTypeDef hspi2;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim5;
TIM_HandleTypeDef htim7;
TIM_HandleTypeDef htim8;

UART_HandleTypeDef huart7;
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart6;

SDRAM_HandleTypeDef hsdram1;

osThreadId defaultTaskHandle;
osThreadId tacheJeuHandle;
osThreadId tacheSonHandle;
osThreadId tachePauseHandle;
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC3_Init(void);
static void MX_DMA2D_Init(void);
static void MX_FMC_Init(void);
static void MX_I2C1_Init(void);
static void MX_I2C3_Init(void);
static void MX_LTDC_Init(void);
static void MX_RTC_Init(void);
static void MX_SPI2_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM5_Init(void);
static void MX_TIM8_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART6_UART_Init(void);
static void MX_ADC1_Init(void);
static void MX_DAC_Init(void);
static void MX_DMA_Init(void);
static void MX_UART7_Init(void);
static void MX_TIM7_Init(void);
void StartDefaultTask(void const *argument);
void codeTacheJeu(void const *argument);
void codeTacheSon(void const *argument);
void codeTachePause(void const *argument);

/* USER CODE BEGIN PFP */
int milieuxHexagone;
uint16_t compteurPoints = 6;
Point listePoints[40];
int compteurTab = 0;
int compteurRemplissage = 0;
int compteurJoueurs = 0;
Point fillPoly[6];
pPoint pfill = &fillPoly;
int pause = 0;
int TSold = 0;
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
	/* USER CODE BEGIN 1 */
	char text[50] = { };
	static TS_StateTypeDef TS_State;
	uint32_t potl, potr, joystick_h, joystick_v;
	ADC_ChannelConfTypeDef sConfig = { 0 };
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
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
	MX_ADC3_Init();
	MX_DMA2D_Init();
	MX_FMC_Init();
	MX_I2C1_Init();
	MX_I2C3_Init();
	MX_LTDC_Init();
	MX_RTC_Init();
	MX_SPI2_Init();
	MX_TIM1_Init();
	MX_TIM2_Init();
	MX_TIM3_Init();
	MX_TIM5_Init();
	MX_TIM8_Init();
	MX_USART1_UART_Init();
	MX_USART6_UART_Init();
	MX_ADC1_Init();
	MX_DAC_Init();
	MX_DMA_Init();
	MX_UART7_Init();
	MX_TIM7_Init();
	/* USER CODE BEGIN 2 */
	BSP_LCD_Init();
	BSP_LCD_LayerDefaultInit(0, LCD_FB_START_ADDRESS);
	BSP_LCD_LayerDefaultInit(1,
	LCD_FB_START_ADDRESS + BSP_LCD_GetXSize() * BSP_LCD_GetYSize() * 4);
	BSP_LCD_DisplayOn();
	BSP_LCD_SelectLayer(0);
	BSP_LCD_Clear(LCD_COLOR_RED);
	BSP_LCD_DrawBitmap(0, 0, (uint8_t*) fondHexbmp_bmp);
	BSP_LCD_SelectLayer(1);
	BSP_LCD_Clear(00);
	BSP_LCD_SetFont(&Font12);
	BSP_LCD_SetTextColor(LCD_COLOR_RED);
	BSP_LCD_SetBackColor(00);

	BSP_TS_Init(BSP_LCD_GetXSize(), BSP_LCD_GetYSize());
	/* USER CODE END 2 */

	/* USER CODE BEGIN RTOS_MUTEX */
	/* add mutexes, ... */
	/* USER CODE END RTOS_MUTEX */

	/* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
	/* USER CODE END RTOS_SEMAPHORES */

	/* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
	/* USER CODE END RTOS_TIMERS */

	/* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
	/* USER CODE END RTOS_QUEUES */

	/* Create the thread(s) */
	/* definition and creation of defaultTask */
	osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
	defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

	/* definition and creation of tacheJeu */
	osThreadDef(tacheJeu, codeTacheJeu, osPriorityNormal, 0, 500);
	tacheJeuHandle = osThreadCreate(osThread(tacheJeu), NULL);

	/* definition and creation of tacheSon */
	osThreadDef(tacheSon, codeTacheSon, osPriorityNormal, 0, 2056);
	tacheSonHandle = osThreadCreate(osThread(tacheSon), NULL);

	/* definition and creation of tachePause */
	osThreadDef(tachePause, codeTachePause, osPriorityHigh, 0, 1028);
	tachePauseHandle = osThreadCreate(osThread(tachePause), NULL);

	/* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
	/* USER CODE END RTOS_THREADS */

	/* Start scheduler */
	osKernelStart();

	/* We should never get here as control is now taken by the scheduler */
	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	int bp1, bp2, bp1_old, bp2_old, i;

	const uint16_t sinus12bit[360] = { 512, 521, 530, 539, 548, 557, 566, 574,
			583, 592, 601, 610, 618, 627, 636, 645, 653, 662, 670, 679, 687,
			695, 704, 712, 720, 728, 736, 744, 752, 760, 768, 76, 783, 791, 798,
			806, 813, 820, 827, 834, 841, 848, 855, 861, 868, 874, 880, 886,
			892, 898, 904, 910, 915, 921, 926, 931, 936, 941, 946, 951, 955,
			960, 964, 968, 972, 976, 980, 983, 987, 990, 993, 996, 999, 1002,
			1004, 1007, 1009, 1011, 1013, 1015, 1016, 1018, 1019, 1020, 1021,
			1022, 1023, 1023, 1024, 1024, 1024, 1024, 1024, 1023, 1023, 1022,
			1021, 1020, 1019, 1018, 1016, 1015, 1013, 1011, 1009, 1007, 1004,
			1002, 999, 996, 993, 990, 987, 983, 980, 976, 972, 968, 964, 960,
			955, 951, 946, 941, 936, 931, 926, 921, 915, 910, 904, 898, 893,
			886, 880, 874, 868, 861, 855, 848, 841, 834, 827, 820, 813, 806,
			798, 791, 783, 776, 768, 760, 752, 744, 737, 728, 720, 712, 704,
			696, 687, 679, 670, 662, 653, 645, 636, 627, 619, 610, 601, 592,
			583, 574, 566, 557, 548, 539, 530, 521, 512, 503, 494, 485, 476,
			467, 459, 450, 441, 432, 423, 414, 406, 397, 388, 380, 371, 362,
			354, 345, 337, 329, 320, 312, 304, 296, 288, 280, 272, 264, 256,
			248, 241, 233, 226, 218, 211, 204, 197, 190, 183, 176, 169, 163,
			156, 150, 144, 138, 132, 126, 120, 114, 109, 103, 98, 93, 88, 83,
			78, 73, 69, 64, 60, 56, 52, 48, 44, 41, 37, 34, 31, 28, 25, 22, 20,
			17, 15, 13, 11, 9, 8, 6, 5, 4, 3, 2, 1, 1, 0, 0, 0, 0, 0, 1, 1, 2,
			3, 4, 5, 6, 8, 9, 11, 13, 15, 17, 20, 22, 25, 28, 31, 34, 37, 41,
			44, 48, 52, 56, 60, 64, 69, 73, 78, 83, 87, 93, 98, 103, 108, 114,
			120, 126, 131, 137, 144, 150, 156, 163, 169, 176, 183, 190, 197,
			204, 211, 218, 226, 233, 241, 248, 256, 264, 272, 279, 287, 295,
			304, 312, 320, 328, 337, 345, 354, 362, 371, 379, 388, 397, 405,
			414, 423, 432, 441, 449, 458, 467, 476, 485, 494, 503 };

	fillTabPositions(238, 37, compteurTab);
	compteurTab++;
	fillTabPositions(238, 70, compteurTab);
	compteurTab++;

	fillTabPositions(238, 103, compteurTab);
	compteurTab++;
	fillTabPositions(238, 136, compteurTab);
	compteurTab++;
	fillTabPositions(238, 169, compteurTab);
	compteurTab++;
	fillTabPositions(238, 202, compteurTab);
	compteurTab++;
	fillTabPositions(238, 235, compteurTab);
	compteurTab++;

	fillTabPositions(210, 218, 2);
	for (i = 0; i < 6; i++) {
		fillTabPositions(295, 37 + 33 * i, 2);
		fillTabPositions(181, 37 + 33 * i, 2);
	}
	for (i = 0; i < 4; i++) {
		fillTabPositions(352, 70 + 33 * i, compteurTab);
		compteurTab++;
		fillTabPositions(124, 70 + 33 * i, compteurTab);
		compteurTab++;
	}
	fillTabPositions(409, 136, compteurTab);
	compteurTab++;
	fillTabPositions(67, 136, compteurTab);
	compteurTab++;
	sprintf(text, "valeur : %d", listePoints[9].X);
	BSP_LCD_DisplayStringAtLine(5, (uint8_t*) text);

	while (1) {
		/*sprintf(text,"valeur : %d", j);
		 BSP_LCD_DisplayStringAtLine(5,(uint8_t*) text);*/
		bp1 = HAL_GPIO_ReadPin(BP1_GPIO_Port, BP1_Pin);
		bp2 = HAL_GPIO_ReadPin(BP2_GPIO_Port, BP2_Pin);
		if (!bp1 && bp1_old) {
			/*##-2- Enable DAC selected channel and associated DMA */
			if (HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (uint32_t*) sinus12bit,
					360, DAC_ALIGN_12B_R) != HAL_OK) {
				/* Start DMA Error */
				Error_Handler();
			}
			htim7.Init.Period = 100;
			if (HAL_TIM_Base_Init(&htim7) != HAL_OK) {
				Error_Handler();
			}
		}
		if (!bp2 && bp2_old) {
			/*##-2- Enable DAC selected channel and associated DMA */
			if (HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (uint32_t*) sinus12bit,
					360, DAC_ALIGN_12B_R) != HAL_OK) {
				/* Start DMA Error */
				Error_Handler();
			}
			htim7.Init.Period = 200;
			if (HAL_TIM_Base_Init(&htim7) != HAL_OK) {
				Error_Handler();
			}
		}
		bp1_old = bp1;
		bp2_old = bp2;

		BSP_TS_GetState(&TS_State);

		if (TS_State.touchDetected && TSold != TS_State.touchDetected) {
			drawHexagon(TS_State.touchX[0], TS_State.touchY[0]);
			//boutonPause(TS_State.touchX[0],TS_State.touchY[0]);
			if (TS_State.touchX[0] > 460 && TS_State.touchY[0] < 20
					&& pause == 0) {
				BSP_LCD_SetLayerVisible(1, ENABLE);
				BSP_LCD_DrawBitmap(0, 0, (uint8_t*) paused_bmp);
				BSP_LCD_SetTransparency(1, 0x6F);

				pause = 1;
				myDelay(1);

			}

			if (compteurJoueurs % 2 == 0) {
				BSP_LCD_SetTextColor(LCD_COLOR_BLUE);

			}
			if (compteurJoueurs % 2 == 1) {
				BSP_LCD_SetTextColor(LCD_COLOR_RED);
			}
			compteurJoueurs++;
			//sprintf(text,"valeur : %d", compteurRemplissage);
			//BSP_LCD_DisplayStringAtLine(5,(uint8_t*) text);

			for (i = 0; i < 8; i++) {
				/*	fillTabPositions(xdraw+12,ydraw+i,4);
				 fillTabPositions(xdraw+12-i,ydraw+12+i,4);

				 fillTabPositions(xdraw-i,ydraw+12,4);
				 fillTabPositions(xdraw+i,ydraw-i,4);
				 fillTabPositions(xdraw-i,ydraw-i,4);

				 fillTabPositions(xdraw-i,ydraw,4);
				 fillTabPositions(xdraw+i,ydraw,4);
				 fillTabPositions(xdraw,ydraw-i,4);
				 fillTabPositions(xdraw,ydraw+i,4); */

				/*BSP_LCD_DrawLine(TS_State.touchX[0]+16, TS_State.touchY[0],TS_State.touchX[0]+9,TS_State.touchY[0]+14);
				 BSP_LCD_DrawLine(TS_State.touchX[0]+9, TS_State.touchY[0]+14,TS_State.touchX[0]-9,TS_State.touchY[0]+14);
				 BSP_LCD_DrawLine(TS_State.touchX[0]-9, TS_State.touchY[0]+14,TS_State.touchX[0]-16,TS_State.touchY[0]);
				 BSP_LCD_DrawLine(TS_State.touchX[0]-16, TS_State.touchY[0],TS_State.touchX[0]-9,TS_State.touchY[0]-14);
				 BSP_LCD_DrawLine(TS_State.touchX[0]-9, TS_State.touchY[0]-14,TS_State.touchX[0]+9,TS_State.touchY[0]-14);
				 BSP_LCD_DrawLine(TS_State.touchX[0]+9, TS_State.touchY[0]-14,TS_State.touchX[0]+16,TS_State.touchY[0]);
				 listePoints[0]->X= xdraw+16	;
				 listePoints[0]->Y= ydraw	;
				 listePoints[1]->X= xdraw+9;
				 listePoints[1]->Y= ydraw+14	;
				 listePoints[2]->X= xdraw-9	;
				 listePoints[2]->Y= ydraw+14	;
				 listePoints[3]->X= xdraw-16	;
				 listePoints[3]->Y= ydraw;
				 listePoints[4]->X= xdraw-9	;
				 listePoints[4]->Y= ydraw-14	;
				 listePoints[5]->X= xdraw+9	;
				 listePoints[5]->Y= ydraw-14	; */
			}

		}

		TSold = TS_State.touchDetected;

	}

}

void fillTabPositions(int x, int y, int j) { //Fonction permettant de remplir le tableaux de structures
	(listePoints[j]).X = x;
	(listePoints[j]).Y = y;
}

void drawHexagon(int x, int y) { //Fonction permettant de dessiner un hexagone rempli
	int xdraw = 1000;
	int ydraw = 1000;
	char txt[50];
	while (compteurRemplissage < 16) {

		if (pow((x - listePoints[compteurRemplissage].X), 2)
				+ pow((y - listePoints[compteurRemplissage].Y), 2)
				< (pow((x - xdraw), 2) + pow((y - ydraw), 2))) {

			xdraw = listePoints[compteurRemplissage].X;
			ydraw = listePoints[compteurRemplissage].Y;
			sprintf(txt, "valeur : %d %d", xdraw, ydraw);
			BSP_LCD_DisplayStringAtLine(5, (uint8_t*) txt);

		}
		compteurRemplissage++;

	}
	BSP_LCD_DrawLine(xdraw + 16, ydraw, xdraw + 9, ydraw + 14);
	BSP_LCD_DrawLine(xdraw + 9, ydraw + 14, xdraw - 9, ydraw + 14);
	BSP_LCD_DrawLine(xdraw - 9, ydraw + 14, xdraw - 16, ydraw);
	BSP_LCD_DrawLine(xdraw - 16, ydraw, xdraw - 9, ydraw - 14);
	BSP_LCD_DrawLine(xdraw - 9, ydraw - 14, xdraw + 9, ydraw - 14);
	BSP_LCD_DrawLine(xdraw + 9, ydraw - 14, xdraw + 16, ydraw);
	(fillPoly[0]).X = xdraw + 16;
	(fillPoly[0]).Y = ydraw;
	(fillPoly[1]).X = xdraw + 9;
	(fillPoly[1]).Y = ydraw + 14;
	(fillPoly[2]).X = xdraw - 9;
	(fillPoly[2]).Y = ydraw + 14;
	(fillPoly[3]).X = xdraw - 16;
	(fillPoly[3]).Y = ydraw;
	(fillPoly[4]).X = xdraw - 9;
	(fillPoly[4]).Y = ydraw - 14;
	(fillPoly[5]).X = xdraw + 9;
	(fillPoly[5]).Y = ydraw - 14;
	pfill = &fillPoly;
	BSP_LCD_FillPolygon(pfill, 6);
	compteurRemplissage = 0;

}

void myDelay(int i) /*Pause l'application pour i seconds*/
{
	clock_t start, end;
	start = clock();
	while (((end = clock()) - start) <= i * CLOCKS_PER_SEC){

	}

}

void boutonPause(int x, int y) {

	if (x > 460 && y < 20) {

		BSP_LCD_DrawBitmap(0, 0, (uint8_t*) paused_bmp);
		BSP_LCD_SetTransparency(1, 0x6F);
		/*if (TS_State.touchDetected){


		 while (TS_State.touchDetected){
		 printf("salut");
		 }
		 }

		 while(!TS_State.touchDetected){
		 printf("salut");
		 }
		 BSP_LCD_Clear(00);*/

	}

	/* USER CODE END WHILE */

	/* USER CODE BEGIN 3 */

	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/** Configure LSE Drive Capability
	 */
	HAL_PWR_EnableBkUpAccess();
	/** Configure the main internal regulator output voltage
	 */
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI
			| RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.LSIState = RCC_LSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 25;
	RCC_OscInitStruct.PLL.PLLN = 400;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 9;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}
	/** Activate the Over-Drive mode
	 */
	if (HAL_PWREx_EnableOverDrive() != HAL_OK) {
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_6) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief ADC1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_ADC1_Init(void) {

	/* USER CODE BEGIN ADC1_Init 0 */

	/* USER CODE END ADC1_Init 0 */

	ADC_ChannelConfTypeDef sConfig = { 0 };

	/* USER CODE BEGIN ADC1_Init 1 */

	/* USER CODE END ADC1_Init 1 */
	/** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
	 */
	hadc1.Instance = ADC1;
	hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
	hadc1.Init.Resolution = ADC_RESOLUTION_12B;
	hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
	hadc1.Init.ContinuousConvMode = DISABLE;
	hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.NbrOfConversion = 1;
	hadc1.Init.DMAContinuousRequests = DISABLE;
	hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	if (HAL_ADC_Init(&hadc1) != HAL_OK) {
		Error_Handler();
	}
	/** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
	 */
	sConfig.Channel = ADC_CHANNEL_0;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN ADC1_Init 2 */

	/* USER CODE END ADC1_Init 2 */

}

/**
 * @brief ADC3 Initialization Function
 * @param None
 * @retval None
 */
static void MX_ADC3_Init(void) {

	/* USER CODE BEGIN ADC3_Init 0 */

	/* USER CODE END ADC3_Init 0 */

	ADC_ChannelConfTypeDef sConfig = { 0 };

	/* USER CODE BEGIN ADC3_Init 1 */

	/* USER CODE END ADC3_Init 1 */
	/** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
	 */
	hadc3.Instance = ADC3;
	hadc3.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
	hadc3.Init.Resolution = ADC_RESOLUTION_12B;
	hadc3.Init.ScanConvMode = ADC_SCAN_DISABLE;
	hadc3.Init.ContinuousConvMode = DISABLE;
	hadc3.Init.DiscontinuousConvMode = DISABLE;
	hadc3.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc3.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc3.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc3.Init.NbrOfConversion = 1;
	hadc3.Init.DMAContinuousRequests = DISABLE;
	hadc3.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	if (HAL_ADC_Init(&hadc3) != HAL_OK) {
		Error_Handler();
	}
	/** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
	 */
	sConfig.Channel = ADC_CHANNEL_6;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN ADC3_Init 2 */

	/* USER CODE END ADC3_Init 2 */

}

/**
 * @brief DAC Initialization Function
 * @param None
 * @retval None
 */
static void MX_DAC_Init(void) {

	/* USER CODE BEGIN DAC_Init 0 */
	{
		/* USER CODE BEGIN DAC_Init 0 */
		hdac.Instance = DAC;
		if (HAL_DAC_DeInit(&hdac) != HAL_OK) {
			Error_Handler();
		}
		/* USER CODE END DAC_Init 0 */

		DAC_ChannelConfTypeDef sConfig = { 0 };

		/* USER CODE BEGIN DAC_Init 1 */

		/* USER CODE END DAC_Init 1 */
		/** DAC Initialization
		 */
		hdac.Instance = DAC;
		if (HAL_DAC_Init(&hdac) != HAL_OK) {
			Error_Handler();
		}
		/** DAC channel OUT1 config
		 */
		sConfig.DAC_Trigger = DAC_TRIGGER_T7_TRGO;
		sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
		if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1) != HAL_OK) {
			Error_Handler();
		}
		/* USER CODE BEGIN DAC_Init 2 */

		/* USER CODE END DAC_Init 2 */

	}
}

	/**
	 * @brief DMA2D Initialization Function
	 * @param None
	 * @retval None
	 */
	static void MX_DMA2D_Init(void) {

		/* USER CODE BEGIN DMA2D_Init 0 */

		/* USER CODE END DMA2D_Init 0 */

		/* USER CODE BEGIN DMA2D_Init 1 */

		/* USER CODE END DMA2D_Init 1 */
		hdma2d.Instance = DMA2D;
		hdma2d.Init.Mode = DMA2D_M2M;
		hdma2d.Init.ColorMode = DMA2D_OUTPUT_ARGB8888;
		hdma2d.Init.OutputOffset = 0;
		hdma2d.LayerCfg[1].InputOffset = 0;
		hdma2d.LayerCfg[1].InputColorMode = DMA2D_INPUT_ARGB8888;
		hdma2d.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
		hdma2d.LayerCfg[1].InputAlpha = 0;
		if (HAL_DMA2D_Init(&hdma2d) != HAL_OK) {
			Error_Handler();
		}
		if (HAL_DMA2D_ConfigLayer(&hdma2d, 1) != HAL_OK) {
			Error_Handler();
		}
		/* USER CODE BEGIN DMA2D_Init 2 */

		/* USER CODE END DMA2D_Init 2 */

	}

	/**
	 * @brief I2C1 Initialization Function
	 * @param None
	 * @retval None
	 */
	static void MX_I2C1_Init(void) {

		/* USER CODE BEGIN I2C1_Init 0 */

		/* USER CODE END I2C1_Init 0 */

		/* USER CODE BEGIN I2C1_Init 1 */

		/* USER CODE END I2C1_Init 1 */
		hi2c1.Instance = I2C1;
		hi2c1.Init.Timing = 0x00C0EAFF;
		hi2c1.Init.OwnAddress1 = 0;
		hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
		hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
		hi2c1.Init.OwnAddress2 = 0;
		hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
		hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
		hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
		if (HAL_I2C_Init(&hi2c1) != HAL_OK) {
			Error_Handler();
		}
		/** Configure Analogue filter
		 */
		if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE)
				!= HAL_OK) {
			Error_Handler();
		}
		/** Configure Digital filter
		 */
		if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK) {
			Error_Handler();
		}
		/* USER CODE BEGIN I2C1_Init 2 */

		/* USER CODE END I2C1_Init 2 */

	}

	/**
	 * @brief I2C3 Initialization Function
	 * @param None
	 * @retval None
	 */
	static void MX_I2C3_Init(void) {

		/* USER CODE BEGIN I2C3_Init 0 */

		/* USER CODE END I2C3_Init 0 */

		/* USER CODE BEGIN I2C3_Init 1 */

		/* USER CODE END I2C3_Init 1 */
		hi2c3.Instance = I2C3;
		hi2c3.Init.Timing = 0x00C0EAFF;
		hi2c3.Init.OwnAddress1 = 0;
		hi2c3.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
		hi2c3.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
		hi2c3.Init.OwnAddress2 = 0;
		hi2c3.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
		hi2c3.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
		hi2c3.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
		if (HAL_I2C_Init(&hi2c3) != HAL_OK) {
			Error_Handler();
		}
		/** Configure Analogue filter
		 */
		if (HAL_I2CEx_ConfigAnalogFilter(&hi2c3, I2C_ANALOGFILTER_ENABLE)
				!= HAL_OK) {
			Error_Handler();
		}
		/** Configure Digital filter
		 */
		if (HAL_I2CEx_ConfigDigitalFilter(&hi2c3, 0) != HAL_OK) {
			Error_Handler();
		}
		/* USER CODE BEGIN I2C3_Init 2 */

		/* USER CODE END I2C3_Init 2 */

	}

	/**
	 * @brief LTDC Initialization Function
	 * @param None
	 * @retval None
	 */
	static void MX_LTDC_Init(void) {

		/* USER CODE BEGIN LTDC_Init 0 */

		/* USER CODE END LTDC_Init 0 */

		LTDC_LayerCfgTypeDef pLayerCfg = { 0 };

		/* USER CODE BEGIN LTDC_Init 1 */

		/* USER CODE END LTDC_Init 1 */
		hltdc.Instance = LTDC;
		hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AL;
		hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AL;
		hltdc.Init.DEPolarity = LTDC_DEPOLARITY_AL;
		hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
		hltdc.Init.HorizontalSync = 40;
		hltdc.Init.VerticalSync = 9;
		hltdc.Init.AccumulatedHBP = 53;
		hltdc.Init.AccumulatedVBP = 11;
		hltdc.Init.AccumulatedActiveW = 533;
		hltdc.Init.AccumulatedActiveH = 283;
		hltdc.Init.TotalWidth = 565;
		hltdc.Init.TotalHeigh = 285;
		hltdc.Init.Backcolor.Blue = 0;
		hltdc.Init.Backcolor.Green = 0;
		hltdc.Init.Backcolor.Red = 0;
		if (HAL_LTDC_Init(&hltdc) != HAL_OK) {
			Error_Handler();
		}
		pLayerCfg.WindowX0 = 0;
		pLayerCfg.WindowX1 = 480;
		pLayerCfg.WindowY0 = 0;
		pLayerCfg.WindowY1 = 272;
		pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_RGB565;
		pLayerCfg.Alpha = 255;
		pLayerCfg.Alpha0 = 0;
		pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
		pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
		pLayerCfg.FBStartAdress = 0xC0000000;
		pLayerCfg.ImageWidth = 480;
		pLayerCfg.ImageHeight = 272;
		pLayerCfg.Backcolor.Blue = 0;
		pLayerCfg.Backcolor.Green = 0;
		pLayerCfg.Backcolor.Red = 0;
		if (HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg, 0) != HAL_OK) {
			Error_Handler();
		}
		/* USER CODE BEGIN LTDC_Init 2 */

		/* USER CODE END LTDC_Init 2 */

	}

	/**
	 * @brief RTC Initialization Function
	 * @param None
	 * @retval None
	 */
	static void MX_RTC_Init(void) {

		/* USER CODE BEGIN RTC_Init 0 */

		/* USER CODE END RTC_Init 0 */

		RTC_TimeTypeDef sTime = { 0 };
		RTC_DateTypeDef sDate = { 0 };
		RTC_AlarmTypeDef sAlarm = { 0 };

		/* USER CODE BEGIN RTC_Init 1 */

		/* USER CODE END RTC_Init 1 */
		/** Initialize RTC Only
		 */
		hrtc.Instance = RTC;
		hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
		hrtc.Init.AsynchPrediv = 127;
		hrtc.Init.SynchPrediv = 255;
		hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
		hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
		hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
		if (HAL_RTC_Init(&hrtc) != HAL_OK) {
			Error_Handler();
		}

		/* USER CODE BEGIN Check_RTC_BKUP */

		/* USER CODE END Check_RTC_BKUP */

		/** Initialize RTC and set the Time and Date
		 */
		sTime.Hours = 0x0;
		sTime.Minutes = 0x0;
		sTime.Seconds = 0x0;
		sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
		sTime.StoreOperation = RTC_STOREOPERATION_RESET;
		if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK) {
			Error_Handler();
		}
		sDate.WeekDay = RTC_WEEKDAY_MONDAY;
		sDate.Month = RTC_MONTH_JANUARY;
		sDate.Date = 0x1;
		sDate.Year = 0x0;
		if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK) {
			Error_Handler();
		}
		/** Enable the Alarm A
		 */
		sAlarm.AlarmTime.Hours = 0x0;
		sAlarm.AlarmTime.Minutes = 0x0;
		sAlarm.AlarmTime.Seconds = 0x0;
		sAlarm.AlarmTime.SubSeconds = 0x0;
		sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
		sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
		sAlarm.AlarmMask = RTC_ALARMMASK_NONE;
		sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
		sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
		sAlarm.AlarmDateWeekDay = 0x1;
		sAlarm.Alarm = RTC_ALARM_A;
		if (HAL_RTC_SetAlarm(&hrtc, &sAlarm, RTC_FORMAT_BCD) != HAL_OK) {
			Error_Handler();
		}
		/** Enable the Alarm B
		 */
		sAlarm.Alarm = RTC_ALARM_B;
		if (HAL_RTC_SetAlarm(&hrtc, &sAlarm, RTC_FORMAT_BCD) != HAL_OK) {
			Error_Handler();
		}
		/** Enable the TimeStamp
		 */
		if (HAL_RTCEx_SetTimeStamp(&hrtc, RTC_TIMESTAMPEDGE_RISING,
				RTC_TIMESTAMPPIN_POS1) != HAL_OK) {
			Error_Handler();
		}
		/* USER CODE BEGIN RTC_Init 2 */

		/* USER CODE END RTC_Init 2 */

	}

	/**
	 * @brief SPI2 Initialization Function
	 * @param None
	 * @retval None
	 */
	static void MX_SPI2_Init(void) {

		/* USER CODE BEGIN SPI2_Init 0 */

		/* USER CODE END SPI2_Init 0 */

		/* USER CODE BEGIN SPI2_Init 1 */

		/* USER CODE END SPI2_Init 1 */
		/* SPI2 parameter configuration*/
		hspi2.Instance = SPI2;
		hspi2.Init.Mode = SPI_MODE_MASTER;
		hspi2.Init.Direction = SPI_DIRECTION_2LINES;
		hspi2.Init.DataSize = SPI_DATASIZE_4BIT;
		hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
		hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
		hspi2.Init.NSS = SPI_NSS_HARD_OUTPUT;
		hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
		hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
		hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
		hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
		hspi2.Init.CRCPolynomial = 7;
		hspi2.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
		hspi2.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
		if (HAL_SPI_Init(&hspi2) != HAL_OK) {
			Error_Handler();
		}
		/* USER CODE BEGIN SPI2_Init 2 */

		/* USER CODE END SPI2_Init 2 */

	}

	/**
	 * @brief TIM1 Initialization Function
	 * @param None
	 * @retval None
	 */
	static void MX_TIM1_Init(void) {

		/* USER CODE BEGIN TIM1_Init 0 */

		/* USER CODE END TIM1_Init 0 */

		TIM_ClockConfigTypeDef sClockSourceConfig = { 0 };
		TIM_MasterConfigTypeDef sMasterConfig = { 0 };

		/* USER CODE BEGIN TIM1_Init 1 */

		/* USER CODE END TIM1_Init 1 */
		htim1.Instance = TIM1;
		htim1.Init.Prescaler = 0;
		htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
		htim1.Init.Period = 65535;
		htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
		htim1.Init.RepetitionCounter = 0;
		htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
		if (HAL_TIM_Base_Init(&htim1) != HAL_OK) {
			Error_Handler();
		}
		sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
		if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK) {
			Error_Handler();
		}
		sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
		sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
		sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
		if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig)
				!= HAL_OK) {
			Error_Handler();
		}
		/* USER CODE BEGIN TIM1_Init 2 */

		/* USER CODE END TIM1_Init 2 */

	}

	/**
	 * @brief TIM2 Initialization Function
	 * @param None
	 * @retval None
	 */
	static void MX_TIM2_Init(void) {

		/* USER CODE BEGIN TIM2_Init 0 */

		/* USER CODE END TIM2_Init 0 */

		TIM_ClockConfigTypeDef sClockSourceConfig = { 0 };
		TIM_MasterConfigTypeDef sMasterConfig = { 0 };

		/* USER CODE BEGIN TIM2_Init 1 */

		/* USER CODE END TIM2_Init 1 */
		htim2.Instance = TIM2;
		htim2.Init.Prescaler = 0;
		htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
		htim2.Init.Period = 4294967295;
		htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
		htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
		if (HAL_TIM_Base_Init(&htim2) != HAL_OK) {
			Error_Handler();
		}
		sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
		if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK) {
			Error_Handler();
		}
		sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
		sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
		if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig)
				!= HAL_OK) {
			Error_Handler();
		}
		/* USER CODE BEGIN TIM2_Init 2 */

		/* USER CODE END TIM2_Init 2 */

	}

	/**
	 * @brief TIM3 Initialization Function
	 * @param None
	 * @retval None
	 */
	static void MX_TIM3_Init(void) {

		/* USER CODE BEGIN TIM3_Init 0 */

		/* USER CODE END TIM3_Init 0 */

		TIM_ClockConfigTypeDef sClockSourceConfig = { 0 };
		TIM_MasterConfigTypeDef sMasterConfig = { 0 };

		/* USER CODE BEGIN TIM3_Init 1 */

		/* USER CODE END TIM3_Init 1 */
		htim3.Instance = TIM3;
		htim3.Init.Prescaler = 0;
		htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
		htim3.Init.Period = 65535;
		htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
		htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
		if (HAL_TIM_Base_Init(&htim3) != HAL_OK) {
			Error_Handler();
		}
		sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
		if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK) {
			Error_Handler();
		}
		sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
		sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
		if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig)
				!= HAL_OK) {
			Error_Handler();
		}
		/* USER CODE BEGIN TIM3_Init 2 */

		/* USER CODE END TIM3_Init 2 */

	}

	/**
	 * @brief TIM5 Initialization Function
	 * @param None
	 * @retval None
	 */
	static void MX_TIM5_Init(void) {

		/* USER CODE BEGIN TIM5_Init 0 */

		/* USER CODE END TIM5_Init 0 */

		TIM_ClockConfigTypeDef sClockSourceConfig = { 0 };
		TIM_MasterConfigTypeDef sMasterConfig = { 0 };

		/* USER CODE BEGIN TIM5_Init 1 */

		/* USER CODE END TIM5_Init 1 */
		htim5.Instance = TIM5;
		htim5.Init.Prescaler = 0;
		htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
		htim5.Init.Period = 4294967295;
		htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
		htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
		if (HAL_TIM_Base_Init(&htim5) != HAL_OK) {
			Error_Handler();
		}
		sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
		if (HAL_TIM_ConfigClockSource(&htim5, &sClockSourceConfig) != HAL_OK) {
			Error_Handler();
		}
		sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
		sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
		if (HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig)
				!= HAL_OK) {
			Error_Handler();
		}
		/* USER CODE BEGIN TIM5_Init 2 */

		/* USER CODE END TIM5_Init 2 */

	}

	/**
	 * @brief TIM7 Initialization Function
	 * @param None
	 * @retval None
	 */
	static void MX_TIM7_Init(void) {

		/* USER CODE BEGIN TIM7_Init 0 */

		/* USER CODE END TIM7_Init 0 */

		TIM_MasterConfigTypeDef sMasterConfig = { 0 };

		/* USER CODE BEGIN TIM7_Init 1 */

		/* USER CODE END TIM7_Init 1 */
		htim7.Instance = TIM7;
		htim7.Init.Prescaler = 0;
		htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
		htim7.Init.Period = 100;
		htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
		if (HAL_TIM_Base_Init(&htim7) != HAL_OK) {
			Error_Handler();
		}
		sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
		sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
		if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig)
				!= HAL_OK) {
			Error_Handler();
		}
		/* USER CODE BEGIN TIM7_Init 2 */
		HAL_TIM_Base_Start(&htim7);

		/* USER CODE END TIM7_Init 2 */

	}

	/**
	 * @brief TIM8 Initialization Function
	 * @param None
	 * @retval None
	 */
	static void MX_TIM8_Init(void) {

		/* USER CODE BEGIN TIM8_Init 0 */

		/* USER CODE END TIM8_Init 0 */

		TIM_ClockConfigTypeDef sClockSourceConfig = { 0 };
		TIM_MasterConfigTypeDef sMasterConfig = { 0 };

		/* USER CODE BEGIN TIM8_Init 1 */

		/* USER CODE END TIM8_Init 1 */
		htim8.Instance = TIM8;
		htim8.Init.Prescaler = 0;
		htim8.Init.CounterMode = TIM_COUNTERMODE_UP;
		htim8.Init.Period = 65535;
		htim8.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
		htim8.Init.RepetitionCounter = 0;
		htim8.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
		if (HAL_TIM_Base_Init(&htim8) != HAL_OK) {
			Error_Handler();
		}
		sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
		if (HAL_TIM_ConfigClockSource(&htim8, &sClockSourceConfig) != HAL_OK) {
			Error_Handler();
		}
		sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
		sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
		sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
		if (HAL_TIMEx_MasterConfigSynchronization(&htim8, &sMasterConfig)
				!= HAL_OK) {
			Error_Handler();
		}
		/* USER CODE BEGIN TIM8_Init 2 */

		/* USER CODE END TIM8_Init 2 */

	}

	/**
	 * @brief UART7 Initialization Function
	 * @param None
	 * @retval None
	 */
	static void MX_UART7_Init(void) {

		/* USER CODE BEGIN UART7_Init 0 */

		/* USER CODE END UART7_Init 0 */

		/* USER CODE BEGIN UART7_Init 1 */

		/* USER CODE END UART7_Init 1 */
		huart7.Instance = UART7;
		huart7.Init.BaudRate = 115200;
		huart7.Init.WordLength = UART_WORDLENGTH_8B;
		huart7.Init.StopBits = UART_STOPBITS_1;
		huart7.Init.Parity = UART_PARITY_NONE;
		huart7.Init.Mode = UART_MODE_TX_RX;
		huart7.Init.HwFlowCtl = UART_HWCONTROL_NONE;
		huart7.Init.OverSampling = UART_OVERSAMPLING_16;
		huart7.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
		huart7.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
		if (HAL_UART_Init(&huart7) != HAL_OK) {
			Error_Handler();
		}
		/* USER CODE BEGIN UART7_Init 2 */

		/* USER CODE END UART7_Init 2 */

	}

	/**
	 * @brief USART1 Initialization Function
	 * @param None
	 * @retval None
	 */
	static void MX_USART1_UART_Init(void) {

		/* USER CODE BEGIN USART1_Init 0 */

		/* USER CODE END USART1_Init 0 */

		/* USER CODE BEGIN USART1_Init 1 */

		/* USER CODE END USART1_Init 1 */
		huart1.Instance = USART1;
		huart1.Init.BaudRate = 115200;
		huart1.Init.WordLength = UART_WORDLENGTH_8B;
		huart1.Init.StopBits = UART_STOPBITS_1;
		huart1.Init.Parity = UART_PARITY_NONE;
		huart1.Init.Mode = UART_MODE_TX_RX;
		huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
		huart1.Init.OverSampling = UART_OVERSAMPLING_16;
		huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
		huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
		if (HAL_UART_Init(&huart1) != HAL_OK) {
			Error_Handler();
		}
		/* USER CODE BEGIN USART1_Init 2 */

		/* USER CODE END USART1_Init 2 */

	}

	/**
	 * @brief USART6 Initialization Function
	 * @param None
	 * @retval None
	 */
	static void MX_USART6_UART_Init(void) {

		/* USER CODE BEGIN USART6_Init 0 */

		/* USER CODE END USART6_Init 0 */

		/* USER CODE BEGIN USART6_Init 1 */

		/* USER CODE END USART6_Init 1 */
		huart6.Instance = USART6;
		huart6.Init.BaudRate = 115200;
		huart6.Init.WordLength = UART_WORDLENGTH_8B;
		huart6.Init.StopBits = UART_STOPBITS_1;
		huart6.Init.Parity = UART_PARITY_NONE;
		huart6.Init.Mode = UART_MODE_TX_RX;
		huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
		huart6.Init.OverSampling = UART_OVERSAMPLING_16;
		huart6.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
		huart6.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
		if (HAL_UART_Init(&huart6) != HAL_OK) {
			Error_Handler();
		}
		/* USER CODE BEGIN USART6_Init 2 */

		/* USER CODE END USART6_Init 2 */

	}

	/**
	 * Enable DMA controller clock
	 */
	static void MX_DMA_Init(void) {

		/* DMA controller clock enable */
		__HAL_RCC_DMA1_CLK_ENABLE();

		/* DMA interrupt init */
		/* DMA1_Stream5_IRQn interrupt configuration */
		HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 5, 0);
		HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);

	}

	/* FMC initialization function */
	static void MX_FMC_Init(void) {

		/* USER CODE BEGIN FMC_Init 0 */

		/* USER CODE END FMC_Init 0 */

		FMC_SDRAM_TimingTypeDef SdramTiming = { 0 };

		/* USER CODE BEGIN FMC_Init 1 */

		/* USER CODE END FMC_Init 1 */

		/** Perform the SDRAM1 memory initialization sequence
		 */
		hsdram1.Instance = FMC_SDRAM_DEVICE;
		/* hsdram1.Init */
		hsdram1.Init.SDBank = FMC_SDRAM_BANK1;
		hsdram1.Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_8;
		hsdram1.Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_12;
		hsdram1.Init.MemoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_16;
		hsdram1.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
		hsdram1.Init.CASLatency = FMC_SDRAM_CAS_LATENCY_3;
		hsdram1.Init.WriteProtection = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
		hsdram1.Init.SDClockPeriod = FMC_SDRAM_CLOCK_PERIOD_2;
		hsdram1.Init.ReadBurst = FMC_SDRAM_RBURST_ENABLE;
		hsdram1.Init.ReadPipeDelay = FMC_SDRAM_RPIPE_DELAY_0;
		/* SdramTiming */
		SdramTiming.LoadToActiveDelay = 2;
		SdramTiming.ExitSelfRefreshDelay = 7;
		SdramTiming.SelfRefreshTime = 4;
		SdramTiming.RowCycleDelay = 7;
		SdramTiming.WriteRecoveryTime = 3;
		SdramTiming.RPDelay = 2;
		SdramTiming.RCDDelay = 2;

		if (HAL_SDRAM_Init(&hsdram1, &SdramTiming) != HAL_OK) {
			Error_Handler();
		}

		/* USER CODE BEGIN FMC_Init 2 */

		/* USER CODE END FMC_Init 2 */
	}

	/**
	 * @brief GPIO Initialization Function
	 * @param None
	 * @retval None
	 */
	static void MX_GPIO_Init(void) {
		GPIO_InitTypeDef GPIO_InitStruct = { 0 };

		/* GPIO Ports Clock Enable */
		__HAL_RCC_GPIOE_CLK_ENABLE();
		__HAL_RCC_GPIOG_CLK_ENABLE();
		__HAL_RCC_GPIOB_CLK_ENABLE();
		__HAL_RCC_GPIOD_CLK_ENABLE();
		__HAL_RCC_GPIOC_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_GPIOJ_CLK_ENABLE();
		__HAL_RCC_GPIOI_CLK_ENABLE();
		__HAL_RCC_GPIOK_CLK_ENABLE();
		__HAL_RCC_GPIOF_CLK_ENABLE();
		__HAL_RCC_GPIOH_CLK_ENABLE();

		/*Configure GPIO pin Output Level */
		HAL_GPIO_WritePin(GPIOE, LED14_Pin | LED15_Pin, GPIO_PIN_RESET);

		/*Configure GPIO pin Output Level */
		HAL_GPIO_WritePin(OTG_FS_PowerSwitchOn_GPIO_Port,
				OTG_FS_PowerSwitchOn_Pin, GPIO_PIN_SET);

		/*Configure GPIO pin Output Level */
		HAL_GPIO_WritePin(LED16_GPIO_Port, LED16_Pin, GPIO_PIN_RESET);

		/*Configure GPIO pin Output Level */
		HAL_GPIO_WritePin(LCD_BL_CTRL_GPIO_Port, LCD_BL_CTRL_Pin, GPIO_PIN_SET);

		/*Configure GPIO pin Output Level */
		HAL_GPIO_WritePin(LCD_DISP_GPIO_Port, LCD_DISP_Pin, GPIO_PIN_SET);

		/*Configure GPIO pin Output Level */
		HAL_GPIO_WritePin(GPIOH,
				LED13_Pin | LED17_Pin | LED11_Pin | LED12_Pin | LED2_Pin
						| LED18_Pin, GPIO_PIN_RESET);

		/*Configure GPIO pin Output Level */
		HAL_GPIO_WritePin(GPIOG, ARDUINO_D4_Pin | ARDUINO_D2_Pin | EXT_RST_Pin,
				GPIO_PIN_RESET);

		/*Configure GPIO pin : OTG_HS_OverCurrent_Pin */
		GPIO_InitStruct.Pin = OTG_HS_OverCurrent_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(OTG_HS_OverCurrent_GPIO_Port, &GPIO_InitStruct);

		/*Configure GPIO pin : QSPI_D2_Pin */
		GPIO_InitStruct.Pin = QSPI_D2_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
		HAL_GPIO_Init(QSPI_D2_GPIO_Port, &GPIO_InitStruct);

		/*Configure GPIO pins : RMII_TXD1_Pin RMII_TXD0_Pin RMII_TX_EN_Pin */
		GPIO_InitStruct.Pin = RMII_TXD1_Pin | RMII_TXD0_Pin | RMII_TX_EN_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
		HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

		/*Configure GPIO pins : ULPI_D7_Pin ULPI_D6_Pin ULPI_D5_Pin ULPI_D2_Pin
		 ULPI_D1_Pin ULPI_D4_Pin */
		GPIO_InitStruct.Pin = ULPI_D7_Pin | ULPI_D6_Pin | ULPI_D5_Pin
				| ULPI_D2_Pin | ULPI_D1_Pin | ULPI_D4_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF10_OTG_HS;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

		/*Configure GPIO pin : SPDIF_RX0_Pin */
		GPIO_InitStruct.Pin = SPDIF_RX0_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		GPIO_InitStruct.Alternate = GPIO_AF8_SPDIFRX;
		HAL_GPIO_Init(SPDIF_RX0_GPIO_Port, &GPIO_InitStruct);

		/*Configure GPIO pins : SDMMC_CK_Pin SDMMC_D3_Pin SDMMC_D2_Pin PC9
		 PC8 */
		GPIO_InitStruct.Pin = SDMMC_CK_Pin | SDMMC_D3_Pin | SDMMC_D2_Pin
				| GPIO_PIN_9 | GPIO_PIN_8;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF12_SDMMC1;
		HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

		/*Configure GPIO pins : BP2_Pin BP1_Pin PA6 */
		GPIO_InitStruct.Pin = BP2_Pin | BP1_Pin | GPIO_PIN_6;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		/*Configure GPIO pins : LED14_Pin LED15_Pin */
		GPIO_InitStruct.Pin = LED14_Pin | LED15_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

		/*Configure GPIO pin : QSPI_NCS_Pin */
		GPIO_InitStruct.Pin = QSPI_NCS_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF10_QUADSPI;
		HAL_GPIO_Init(QSPI_NCS_GPIO_Port, &GPIO_InitStruct);

		/*Configure GPIO pin : OTG_FS_VBUS_Pin */
		GPIO_InitStruct.Pin = OTG_FS_VBUS_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(OTG_FS_VBUS_GPIO_Port, &GPIO_InitStruct);

		/*Configure GPIO pin : Audio_INT_Pin */
		GPIO_InitStruct.Pin = Audio_INT_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(Audio_INT_GPIO_Port, &GPIO_InitStruct);

		/*Configure GPIO pins : OTG_FS_P_Pin OTG_FS_N_Pin */
		GPIO_InitStruct.Pin = OTG_FS_P_Pin | OTG_FS_N_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF10_OTG_FS;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		/*Configure GPIO pins : SAI2_MCLKA_Pin SAI2_SCKA_Pin SAI2_FSA_Pin SAI2_SDA_Pin */
		GPIO_InitStruct.Pin = SAI2_MCLKA_Pin | SAI2_SCKA_Pin | SAI2_FSA_Pin
				| SAI2_SDA_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		GPIO_InitStruct.Alternate = GPIO_AF10_SAI2;
		HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

		/*Configure GPIO pin : SAI2_SDB_Pin */
		GPIO_InitStruct.Pin = SAI2_SDB_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		GPIO_InitStruct.Alternate = GPIO_AF10_SAI2;
		HAL_GPIO_Init(SAI2_SDB_GPIO_Port, &GPIO_InitStruct);

		/*Configure GPIO pins : OTG_FS_PowerSwitchOn_Pin LED16_Pin */
		GPIO_InitStruct.Pin = OTG_FS_PowerSwitchOn_Pin | LED16_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

		/*Configure GPIO pin : uSD_Detect_Pin */
		GPIO_InitStruct.Pin = uSD_Detect_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(uSD_Detect_GPIO_Port, &GPIO_InitStruct);

		/*Configure GPIO pin : LCD_BL_CTRL_Pin */
		GPIO_InitStruct.Pin = LCD_BL_CTRL_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(LCD_BL_CTRL_GPIO_Port, &GPIO_InitStruct);

		/*Configure GPIO pins : PG9 RMII_RXER_Pin */
		GPIO_InitStruct.Pin = GPIO_PIN_9 | RMII_RXER_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

		/*Configure GPIO pin : OTG_FS_OverCurrent_Pin */
		GPIO_InitStruct.Pin = OTG_FS_OverCurrent_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(OTG_FS_OverCurrent_GPIO_Port, &GPIO_InitStruct);

		/*Configure GPIO pin : SDMMC_CMD_Pin */
		GPIO_InitStruct.Pin = SDMMC_CMD_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF12_SDMMC1;
		HAL_GPIO_Init(SDMMC_CMD_GPIO_Port, &GPIO_InitStruct);

		/*Configure GPIO pins : TP3_Pin PH13 NC2_Pin */
		GPIO_InitStruct.Pin = TP3_Pin | GPIO_PIN_13 | NC2_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

		/*Configure GPIO pin : LCD_DISP_Pin */
		GPIO_InitStruct.Pin = LCD_DISP_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(LCD_DISP_GPIO_Port, &GPIO_InitStruct);

		/*Configure GPIO pins : LED13_Pin LED17_Pin LED11_Pin LED12_Pin
		 LED2_Pin LED18_Pin */
		GPIO_InitStruct.Pin = LED13_Pin | LED17_Pin | LED11_Pin | LED12_Pin
				| LED2_Pin | LED18_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

		/*Configure GPIO pin : LCD_INT_Pin */
		GPIO_InitStruct.Pin = LCD_INT_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(LCD_INT_GPIO_Port, &GPIO_InitStruct);

		/*Configure GPIO pin : ULPI_NXT_Pin */
		GPIO_InitStruct.Pin = ULPI_NXT_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF10_OTG_HS;
		HAL_GPIO_Init(ULPI_NXT_GPIO_Port, &GPIO_InitStruct);

		/*Configure GPIO pins : ARDUINO_D4_Pin ARDUINO_D2_Pin EXT_RST_Pin */
		GPIO_InitStruct.Pin = ARDUINO_D4_Pin | ARDUINO_D2_Pin | EXT_RST_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

		/*Configure GPIO pins : ULPI_STP_Pin ULPI_DIR_Pin */
		GPIO_InitStruct.Pin = ULPI_STP_Pin | ULPI_DIR_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF10_OTG_HS;
		HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

		/*Configure GPIO pins : RMII_MDC_Pin RMII_RXD0_Pin RMII_RXD1_Pin */
		GPIO_InitStruct.Pin = RMII_MDC_Pin | RMII_RXD0_Pin | RMII_RXD1_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
		HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

		/*Configure GPIO pin : PB2 */
		GPIO_InitStruct.Pin = GPIO_PIN_2;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

		/*Configure GPIO pins : QSPI_D1_Pin QSPI_D3_Pin QSPI_D0_Pin */
		GPIO_InitStruct.Pin = QSPI_D1_Pin | QSPI_D3_Pin | QSPI_D0_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
		HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

		/*Configure GPIO pins : RMII_REF_CLK_Pin RMII_MDIO_Pin RMII_CRS_DV_Pin */
		GPIO_InitStruct.Pin =
				RMII_REF_CLK_Pin | RMII_MDIO_Pin | RMII_CRS_DV_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		/*Configure GPIO pins : ULPI_CLK_Pin ULPI_D0_Pin */
		GPIO_InitStruct.Pin = ULPI_CLK_Pin | ULPI_D0_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF10_OTG_HS;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	}

	/* USER CODE BEGIN 4 */

	/* USER CODE END 4 */

	/* USER CODE BEGIN Header_StartDefaultTask */
	/**
	 * @brief  Function implementing the defaultTask thread.
	 * @param  argument: Not used
	 * @retval None
	 */
	/* USER CODE END Header_StartDefaultTask */
	void StartDefaultTask(void const *argument) {
		/* USER CODE BEGIN 5 */
		/* Infinite loop */
		for (;;) {
			osDelay(1);
		}
		/* USER CODE END 5 */
	}

	/* USER CODE BEGIN Header_codeTacheJeu */
	/**
	 * @brief Function implementing the tacheJeu thread.
	 * @param argument: Not used
	 * @retval None
	 */
	/* USER CODE END Header_codeTacheJeu */
	void codeTacheJeu(void const *argument) {
		/* USER CODE BEGIN codeTacheJeu */
		static TS_StateTypeDef TS_State;
		int i;
		char text[50];
		/* Infinite loop */fillTabPositions(238, 37, compteurTab);
		compteurTab++;
		fillTabPositions(238, 70, compteurTab);
		compteurTab++;

		fillTabPositions(238, 103, compteurTab);
		compteurTab++;
		fillTabPositions(238, 136, compteurTab);
		compteurTab++;
		fillTabPositions(238, 169, compteurTab);
		compteurTab++;
		fillTabPositions(238, 202, compteurTab);
		compteurTab++;
		fillTabPositions(238, 235, compteurTab);
		compteurTab++;

		fillTabPositions(210, 218, 2);
		for (i = 0; i < 6; i++) {
			fillTabPositions(295, 37 + 33 * i, 2);
			fillTabPositions(181, 37 + 33 * i, 2);
		}
		for (i = 0; i < 4; i++) {
			fillTabPositions(352, 70 + 33 * i, compteurTab);
			compteurTab++;
			fillTabPositions(124, 70 + 33 * i, compteurTab);
			compteurTab++;
		}
		fillTabPositions(409, 136, compteurTab);
		compteurTab++;
		fillTabPositions(67, 136, compteurTab); //Fonction définie plus haut
		compteurTab++;
		sprintf(text, "valeur : %d", listePoints[9].X);
		BSP_LCD_DisplayStringAtLine(5, (uint8_t*) text);
		for (;;) {
			BSP_TS_GetState(&TS_State);

			if (TS_State.touchDetected && TSold != TS_State.touchDetected) {
				drawHexagon(TS_State.touchX[0], TS_State.touchY[0]); //Fonciton définie plus
				//boutonPause(TS_State.touchX[0],TS_State.touchY[0]);

				if (compteurJoueurs % 2 == 0) {
					BSP_LCD_SetTextColor(LCD_COLOR_BLUE);

				}
				if (compteurJoueurs % 2 == 1) {
					BSP_LCD_SetTextColor(LCD_COLOR_RED);
				}
				compteurJoueurs++;
				//sprintf(text,"valeur : %d", compteurRemplissage);
				//BSP_LCD_DisplayStringAtLine(5,(uint8_t*) text);

				for (i = 0; i < 8; i++) {
					/*	fillTabPositions(xdraw+12,ydraw+i,4);
					 fillTabPositions(xdraw+12-i,ydraw+12+i,4);

					 fillTabPositions(xdraw-i,ydraw+12,4);
					 fillTabPositions(xdraw+i,ydraw-i,4);
					 fillTabPositions(xdraw-i,ydraw-i,4);

					 fillTabPositions(xdraw-i,ydraw,4);
					 fillTabPositions(xdraw+i,ydraw,4);
					 fillTabPositions(xdraw,ydraw-i,4);
					 fillTabPositions(xdraw,ydraw+i,4); */

					/*BSP_LCD_DrawLine(TS_State.touchX[0]+16, TS_State.touchY[0],TS_State.touchX[0]+9,TS_State.touchY[0]+14);
					 BSP_LCD_DrawLine(TS_State.touchX[0]+9, TS_State.touchY[0]+14,TS_State.touchX[0]-9,TS_State.touchY[0]+14);
					 BSP_LCD_DrawLine(TS_State.touchX[0]-9, TS_State.touchY[0]+14,TS_State.touchX[0]-16,TS_State.touchY[0]);
					 BSP_LCD_DrawLine(TS_State.touchX[0]-16, TS_State.touchY[0],TS_State.touchX[0]-9,TS_State.touchY[0]-14);
					 BSP_LCD_DrawLine(TS_State.touchX[0]-9, TS_State.touchY[0]-14,TS_State.touchX[0]+9,TS_State.touchY[0]-14);
					 BSP_LCD_DrawLine(TS_State.touchX[0]+9, TS_State.touchY[0]-14,TS_State.touchX[0]+16,TS_State.touchY[0]);
					 listePoints[0]->X= xdraw+16	;
					 listePoints[0]->Y= ydraw	;
					 listePoints[1]->X= xdraw+9;
					 listePoints[1]->Y= ydraw+14	;
					 listePoints[2]->X= xdraw-9	;
					 listePoints[2]->Y= ydraw+14	;
					 listePoints[3]->X= xdraw-16	;
					 listePoints[3]->Y= ydraw;
					 listePoints[4]->X= xdraw-9	;
					 listePoints[4]->Y= ydraw-14	;
					 listePoints[5]->X= xdraw+9	;
					 listePoints[5]->Y= ydraw-14	; */
				}

			}


			if (TS_State.touchX[0] > 460 && TS_State.touchY[0] < 20
					&& pause == 0) {
				BSP_LCD_SetLayerVisible(1, ENABLE);
				BSP_LCD_DrawBitmap(0, 0, (uint8_t*) paused_bmp);
				BSP_LCD_SetTransparency(1, 0x6F);



				pause = 1;


			}



			TSold = TS_State.touchDetected;
			vTaskDelay(50);

		}

	}
	/* USER CODE END codeTacheJeu */

/* USER CODE BEGIN Header_codeTacheSon */
/**
 * @brief Function implementing the tacheSon thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_codeTacheSon */
void codeTacheSon(void const *argument) {
	/* USER CODE BEGIN codeTacheSon */
	/* Infinite loop */
	const uint16_t sinus12bit[360] = { 512, 521, 530, 539, 548, 557, 566, 574,
			583, 592, 601, 610, 618, 627, 636, 645, 653, 662, 670, 679, 687,
			695, 704, 712, 720, 728, 736, 744, 752, 760, 768, 76, 783, 791, 798,
			806, 813, 820, 827, 834, 841, 848, 855, 861, 868, 874, 880, 886,
			892, 898, 904, 910, 915, 921, 926, 931, 936, 941, 946, 951, 955,
			960, 964, 968, 972, 976, 980, 983, 987, 990, 993, 996, 999, 1002,
			1004, 1007, 1009, 1011, 1013, 1015, 1016, 1018, 1019, 1020, 1021,
			1022, 1023, 1023, 1024, 1024, 1024, 1024, 1024, 1023, 1023, 1022,
			1021, 1020, 1019, 1018, 1016, 1015, 1013, 1011, 1009, 1007, 1004,
			1002, 999, 996, 993, 990, 987, 983, 980, 976, 972, 968, 964, 960,
			955, 951, 946, 941, 936, 931, 926, 921, 915, 910, 904, 898, 893,
			886, 880, 874, 868, 861, 855, 848, 841, 834, 827, 820, 813, 806,
			798, 791, 783, 776, 768, 760, 752, 744, 737, 728, 720, 712, 704,
			696, 687, 679, 670, 662, 653, 645, 636, 627, 619, 610, 601, 592,
			583, 574, 566, 557, 548, 539, 530, 521, 512, 503, 494, 485, 476,
			467, 459, 450, 441, 432, 423, 414, 406, 397, 388, 380, 371, 362,
			354, 345, 337, 329, 320, 312, 304, 296, 288, 280, 272, 264, 256,
			248, 241, 233, 226, 218, 211, 204, 197, 190, 183, 176, 169, 163,
			156, 150, 144, 138, 132, 126, 120, 114, 109, 103, 98, 93, 88, 83,
			78, 73, 69, 64, 60, 56, 52, 48, 44, 41, 37, 34, 31, 28, 25, 22, 20,
			17, 15, 13, 11, 9, 8, 6, 5, 4, 3, 2, 1, 1, 0, 0, 0, 0, 0, 1, 1, 2,
			3, 4, 5, 6, 8, 9, 11, 13, 15, 17, 20, 22, 25, 28, 31, 34, 37, 41,
			44, 48, 52, 56, 60, 64, 69, 73, 78, 83, 87, 93, 98, 103, 108, 114,
			120, 126, 131, 137, 144, 150, 156, 163, 169, 176, 183, 190, 197,
			204, 211, 218, 226, 233, 241, 248, 256, 264, 272, 279, 287, 295,
			304, 312, 320, 328, 337, 345, 354, 362, 371, 379, 388, 397, 405,
			414, 423, 432, 441, 449, 458, 467, 476, 485, 494, 503 };
	int bp1_old = 1;
	int bp2_old = 1;
	int bp1, bp2;
	for (;;) {
		bp1 = HAL_GPIO_ReadPin(BP1_GPIO_Port, BP1_Pin);
		bp2 = HAL_GPIO_ReadPin(BP2_GPIO_Port, BP2_Pin);
		if (!bp1 && bp1_old) {
			/*##-2- Enable DAC selected channel and associated DMA */
			if (HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (uint32_t*) sinus12bit,
					360, DAC_ALIGN_12B_R) != HAL_OK) {
				/* Start DMA Error */
				Error_Handler();
			}
			htim7.Init.Period = 100;
			if (HAL_TIM_Base_Init(&htim7) != HAL_OK) {
				Error_Handler();
			}
		}
		if (!bp2 && bp2_old) {
			/*##-2- Enable DAC selected channel and associated DMA */
			if (HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (uint32_t*) sinus12bit,
					360, DAC_ALIGN_12B_R) != HAL_OK) {
				/* Start DMA Error */
				Error_Handler();
			}
			htim7.Init.Period = 200;
			if (HAL_TIM_Base_Init(&htim7) != HAL_OK) {
				Error_Handler();
			}
		}
		bp1_old = bp1;
		bp2_old = bp2;
		vTaskDelay(20);
	}
	/* USER CODE END codeTacheSon */
}

/* USER CODE BEGIN Header_codeTachePause */
/**
 * @brief Function implementing the tachePause thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_codeTachePause */
void codeTachePause(void const *argument) {
	/* USER CODE BEGIN codeTachePause */
	/* Infinite loop */

	static TS_StateTypeDef TS_State;
	for (;;) {
		if (pause==1){
				vTaskSuspend(tacheJeuHandle);
				myDelay(2);}
		if (TS_State.touchDetected && pause==1) {

			BSP_LCD_SetLayerVisible(1, DISABLE);

			pause = 0;

			vTaskResume(tacheJeuHandle);
		}
		vTaskDelay(20);
	}
	/* USER CODE END codeTachePause */
}

/**
 * @brief  Period elapsed callback in non blocking mode
 * @note   This function is called  when TIM6 interrupt took place, inside
 * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
 * a global variable "uwTick" used as application time base.
 * @param  htim : TIM handle
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	/* USER CODE BEGIN Callback 0 */

	/* USER CODE END Callback 0 */
	if (htim->Instance == TIM6) {
		HAL_IncTick();
	}
	/* USER CODE BEGIN Callback 1 */

	/* USER CODE END Callback 1 */
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
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

