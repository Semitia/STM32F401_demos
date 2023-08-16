#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "usart.h"
#include "stdio.h"

uint8_t USART1_BUF[] = "Hello FreeRTOS\r\n";
osThreadId_t defaultTaskHandle, ledTaskHandle;

const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
const osThreadAttr_t LedTask_attributes = {
  .name = "LedTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

void StartDefaultTask(void *argument);
void LedTask(void *argument);
void MX_FREERTOS_Init(void); 

/**
  * @brief  FreeRTOS initialization
  */
void MX_FREERTOS_Init(void) {

  /* Create the thread(s) */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);
	ledTaskHandle = osThreadNew(LedTask, NULL, &LedTask_attributes);
}


void StartDefaultTask(void *argument)
{

  while(1)
  {
		//printf("USART1: %s", USART1_BUF); 
		HAL_UART_Transmit(&huart1,USART1_BUF,sizeof(USART1_BUF),0xffff);
    osDelay(1000);
  }
}

void LedTask(void *argument)
{
	while(1)
  {
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);
		osDelay(1000);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);
		osDelay(1000);
  }
}

