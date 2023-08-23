#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "usart.h"
#include "stdio.h"

uint8_t USART1_BUF[] = "Hello FreeRTOS\r\n";

/* START_TASK 任务 配置 */
#define START_TASK_PRIO 1                   /* 任务优先级 数字越大优先级越高*/
#define START_STK_SIZE  128                 /* 任务堆栈大小 */
TaskHandle_t            StartTask_Handler;  /* 任务句柄 */
void start_task(void *pvParameters);        /* 任务函数 */

/* TASK1 任务 配置 */
#define TASK1_PRIO      3                   /* 任务优先级 */
#define TASK1_STK_SIZE  128                 /* 任务堆栈大小 */
TaskHandle_t            Task1Task_Handler;  /* 任务句柄 */
void Task1(void *pvParameters);             /* 任务函数 */

/* TASK2 任务 配置 */
#define TASK2_PRIO      2                   /* 任务优先级 */
#define TASK2_STK_SIZE  128                 /* 任务堆栈大小 */
TaskHandle_t            Task2Task_Handler;  /* 任务句柄 */
void Task2(void *pvParameters);             /* 任务函数 */


/**
  * @brief  FreeRTOS initialization
  */
void MX_FREERTOS_Init(void) {
	
	xTaskCreate((TaskFunction_t )start_task,          /* 任务函数 */
						(const char*    )"start_task",          /* 任务名称 */
						(uint16_t       )START_STK_SIZE,        /* 任务堆栈大小 */
						(void*          )NULL,                  /* 传入给任务函数的参数 */
						(UBaseType_t    )START_TASK_PRIO,       /* 任务优先级 */
						(TaskHandle_t*  )&StartTask_Handler);   /* 任务句柄 */
}

/**
 * @brief       start_task
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           /* 进入临界区 */
    /* 创建任务1 */
    xTaskCreate((TaskFunction_t )Task1,
                (const char*    )"Task1",
                (uint16_t       )TASK1_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )TASK1_PRIO,
                (TaskHandle_t*  )&Task1Task_Handler);
    /* 创建任务2 */
    xTaskCreate((TaskFunction_t )Task2,
                (const char*    )"Task2",
                (uint16_t       )TASK2_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )TASK2_PRIO,
                (TaskHandle_t*  )&Task2Task_Handler);
    vTaskDelete(StartTask_Handler); /* 删除开始任务 */
    taskEXIT_CRITICAL();            /* 退出临界区 */
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

