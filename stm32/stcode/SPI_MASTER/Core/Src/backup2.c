//#include "main.h"
//#include "cmsis_os.h"
//#include <stdio.h>
//
//char txBuf[] = "EFGH";
//char txBuf2[100];
//
//char rxBuff1[100];
//char txBuff1[100];
//
//SPI_HandleTypeDef hspi1;
//
//osThreadId_t defaultTaskHandle;
//const osThreadAttr_t defaultTask_attributes = { .name = "defaultTask",
//		.priority = (osPriority_t) osPriorityNormal, .stack_size = 128 * 4 };
//
//osThreadId_t myTask02Handle;
//const osThreadAttr_t myTask02_attributes = { .name = "myTask02", .priority =
//		(osPriority_t) osPriorityLow, .stack_size = 128 * 4 };
//
//void SystemClock_Config(void);
//static void MX_GPIO_Init(void);
//static void MX_SPI1_Init(void);
//void StartDefaultTask(void *argument);
//void StartTask02(void *argument);
//
//int main(void) {
//
//	HAL_Init();
//	SystemClock_Config();
//
//	MX_GPIO_Init();
//	MX_SPI1_Init();
//
//	osKernelInitialize();
//	defaultTaskHandle = osThreadNew(StartDefaultTask, NULL,
//			&defaultTask_attributes);
//	myTask02Handle = osThreadNew(StartTask02, NULL, &myTask02_attributes);
//	osKernelStart();
//
//}
//
//void SystemClock_Config(void) {
//	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
//	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };
//
//	__HAL_RCC_PWR_CLK_ENABLE();
//	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
//	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
//	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
//	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
//	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
//	RCC_OscInitStruct.PLL.PLLM = 4;
//	RCC_OscInitStruct.PLL.PLLN = 72;
//	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
//	RCC_OscInitStruct.PLL.PLLQ = 3;
//	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
//		Error_Handler();
//	}
//
//	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
//			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
//	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
//	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
//	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
//	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
//
//	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
//		Error_Handler();
//	}
//}
//
//static void MX_SPI1_Init(void) {
//	hspi1.Instance = SPI1;
//	hspi1.Init.Mode = SPI_MODE_MASTER;
//	hspi1.Init.Direction = SPI_DIRECTION_2LINES;
//	hspi1.Init.DataSize = SPI_DATASIZE_16BIT;
//	hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
//	hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
//	hspi1.Init.NSS = SPI_NSS_SOFT;
//	hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
//	hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
//	hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
//	hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
//	hspi1.Init.CRCPolynomial = 10;
//	if (HAL_SPI_Init(&hspi1) != HAL_OK) {
//		Error_Handler();
//	}
//}
//
//static void MX_GPIO_Init(void) {
//	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
//
//	__HAL_RCC_GPIOH_CLK_ENABLE();
//	__HAL_RCC_GPIOA_CLK_ENABLE();
//	__HAL_RCC_GPIOB_CLK_ENABLE();
//
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
//
//	GPIO_InitStruct.Pin = GPIO_PIN_4;
//	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//	GPIO_InitStruct.Pull = GPIO_NOPULL;
//	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//
//}
//
//void StartDefaultTask(void *argument) {
//
//	for (;;) {
//		printf("from task2\n");
//		osDelay(10);
//	}
//}
//
//void StartTask02(void *argument) {
//	int counter = 0;
//	for (;;) {
//		printf("\n");
//		printf("................\n");
//
//		snprintf(txBuf2, sizeof(txBuf2), "%s %010d", txBuf, counter);
//
//		printf("spi send: ");
//		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
//		HAL_SPI_Transmit(&hspi1, (uint8_t*) txBuf2, sizeof(txBuf2), 10);
//		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
//
//		printf("%s\n", txBuf2);
//		counter++;
//		osDelay(10);
//
////		printf("spi rcv: ");
////		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
////		HAL_SPI_TransmitReceive(&hspi1,(uint8_t*) txBuff1, (uint8_t*) rxBuff1, 100, 10);
////		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
////		printf("%s\n", rxBuff1);
////
////		printf("................\n");
////		osDelay(10);
//
//	}
//}
//
//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
//
//	if (htim->Instance == TIM6) {
//		HAL_IncTick();
//	}
//}
//
//void Error_Handler(void) {
//
//	__disable_irq();
//	while (1) {
//	}
//}
//
//int _write(int file, char *ptr, int len) {
//	int DataIdx;
//	for (DataIdx = 0; DataIdx < len; DataIdx++) {
//		ITM_SendChar(*ptr++);
//	}
//	return len;
//}
//
//#ifdef  USE_FULL_ASSERT
//
//void assert_failed(uint8_t *file, uint32_t line)
//{
//}
//#endif
