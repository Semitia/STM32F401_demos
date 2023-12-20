/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "adc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

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
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

uint16_t i;
#define ADC1_CHANNEL_CNT 6
#define ADC1_CHANNEL_FRE 3	
uint16_t adc1_val_buf[ADC1_CHANNEL_CNT*ADC1_CHANNEL_FRE]; 
uint32_t adc1_aver_val[ADC1_CHANNEL_CNT] = {0}; 
uint16_t value[ADC1_CHANNEL_CNT] = {0};
void StartDefaultTask(void *argument)
{
  if(HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&adc1_val_buf, (ADC1_CHANNEL_CNT*ADC1_CHANNEL_FRE)) != HAL_OK)
  {
    /* Start Conversation Error */
    Error_Handler(); 
  }
  while(1) {
				for(i = 0; i < ADC1_CHANNEL_CNT; i++)
		{
			adc1_aver_val[i] = 0;
		}

		for(i = 0; i < ADC1_CHANNEL_FRE; i++)
		{
			adc1_aver_val[0] +=  adc1_val_buf[i*ADC1_CHANNEL_CNT+0];
			adc1_aver_val[1] +=  adc1_val_buf[i*ADC1_CHANNEL_CNT+1];
			adc1_aver_val[2] +=  adc1_val_buf[i*ADC1_CHANNEL_CNT+2];
			adc1_aver_val[3] +=  adc1_val_buf[i*ADC1_CHANNEL_CNT+3];
			adc1_aver_val[4] +=  adc1_val_buf[i*ADC1_CHANNEL_CNT+4];
			adc1_aver_val[5] +=  adc1_val_buf[i*ADC1_CHANNEL_CNT+5];
		}

		for(i = 0; i < ADC1_CHANNEL_CNT; i++)
		{
			value[i] = adc1_aver_val[i] / ADC1_CHANNEL_FRE;
		}
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

