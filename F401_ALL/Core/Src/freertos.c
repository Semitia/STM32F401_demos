#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "usart.h"
#include "stdio.h"
#include "delay.h"
#include "stdlib.h"
//#include "myiic.h"
#include "AS5600.h"
#include "test.h"
#include "TIM.h"
#include "FOC.h"

uint8_t USART1_BUF[] = "Hello FreeRTOS\r\n";

/* START_TASK ���� ���� */
#define START_TASK_PRIO 1                   /* �������ȼ� ����Խ�����ȼ�Խ��*/
#define START_STK_SIZE  128                 /* �����ջ��С */
TaskHandle_t            StartTask_Handler;  /* ������ */
void start_task(void *pvParameters);        /* ������ */

/* TASK1--INFO ���� ���� */
#define TASK1_PRIO      3                   /* �������ȼ� */
#define TASK1_STK_SIZE  128                 /* �����ջ��С */
TaskHandle_t            Task1Task_Handler;  /* ������ */
void info_Task(void *pvParameters);             /* ������ */

/* TASK2--CMD ���� ���� */
#define TASK2_PRIO      2                   /* �������ȼ� */
#define TASK2_STK_SIZE  128                 /* �����ջ��С */
TaskHandle_t            Task2Task_Handler;  /* ������ */
void CMD_Task(void *pvParameters);             /* ������ */


/**
  * @brief  FreeRTOS initialization
  */
void MX_FREERTOS_Init(void) {
	xTaskCreate((TaskFunction_t )start_task,          /* ������ */
						(const char*    )"start_task",          /* �������� */
						(uint16_t       )START_STK_SIZE,        /* �����ջ��С */
						(void*          )NULL,                  /* ������������Ĳ��� */
						(UBaseType_t    )START_TASK_PRIO,       /* �������ȼ� */
						(TaskHandle_t*  )&StartTask_Handler);   /* ������ */
}

/**
 * @brief       start_task
 * @param       pvParameters : �������(δ�õ�)
 * @retval      ��
 */
void start_task(void *pvParameters)
{
  
  taskENTER_CRITICAL();           /* �����ٽ��� */
  /* ��������1 */
  xTaskCreate((TaskFunction_t )info_Task,
              (const char*    )"infoTask",
              (uint16_t       )TASK1_STK_SIZE,
              (void*          )NULL,
              (UBaseType_t    )TASK1_PRIO,
              (TaskHandle_t*  )&Task1Task_Handler);
  /* ��������2 */
  xTaskCreate((TaskFunction_t )CMD_Task,
              (const char*    )"CMDTask",
              (uint16_t       )TASK2_STK_SIZE,
              (void*          )NULL,
              (UBaseType_t    )TASK2_PRIO,
              (TaskHandle_t*  )&Task2Task_Handler);
  vTaskDelete(StartTask_Handler); /* ɾ����ʼ���� */
  taskEXIT_CRITICAL();            /* �˳��ٽ��� */
}

void info_Task(void *argument)
{
  while(1)
  {
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);
    osDelay(1000);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);
		osDelay(1000);
  }
}

void CMD_Task(void *argument)
{
  FOC_init(11.1f,7,1);
  AS5600_test();

}

