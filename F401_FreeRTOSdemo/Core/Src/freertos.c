#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "usart.h"
#include "stdio.h"

uint8_t USART1_BUF[] = "Hello FreeRTOS\r\n";

/* START_TASK ���� ���� */
#define START_TASK_PRIO 1                   /* �������ȼ� ����Խ�����ȼ�Խ��*/
#define START_STK_SIZE  128                 /* �����ջ��С */
TaskHandle_t            StartTask_Handler;  /* ������ */
void start_task(void *pvParameters);        /* ������ */

/* TASK1 ���� ���� */
#define TASK1_PRIO      3                   /* �������ȼ� */
#define TASK1_STK_SIZE  128                 /* �����ջ��С */
TaskHandle_t            Task1Task_Handler;  /* ������ */
void Task1(void *pvParameters);             /* ������ */

/* TASK2 ���� ���� */
#define TASK2_PRIO      2                   /* �������ȼ� */
#define TASK2_STK_SIZE  128                 /* �����ջ��С */
TaskHandle_t            Task2Task_Handler;  /* ������ */
void Task2(void *pvParameters);             /* ������ */


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
    xTaskCreate((TaskFunction_t )Task1,
                (const char*    )"Task1",
                (uint16_t       )TASK1_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )TASK1_PRIO,
                (TaskHandle_t*  )&Task1Task_Handler);
    /* ��������2 */
    xTaskCreate((TaskFunction_t )Task2,
                (const char*    )"Task2",
                (uint16_t       )TASK2_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )TASK2_PRIO,
                (TaskHandle_t*  )&Task2Task_Handler);
    vTaskDelete(StartTask_Handler); /* ɾ����ʼ���� */
    taskEXIT_CRITICAL();            /* �˳��ٽ��� */
}

void Task1(void *argument)
{

  while(1)
  {
		if(g_usart_rx_sta & 0x8000 ) {
			printf("got: %s",g_usart_rx_buf);
			g_usart_rx_sta = 0;
		}
		//else printf("Task1: %s",USART1_BUF);
		osDelay(100);
  }
}

void Task2(void *argument)
{
	
	while(1)
  {
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);
    osDelay(1000);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);
		osDelay(1000);
		

  }
}

