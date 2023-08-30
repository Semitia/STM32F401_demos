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
#define TASK1_PRIO      2                   /* �������ȼ� */
#define TASK1_STK_SIZE  128                 /* �����ջ��С */
TaskHandle_t            Task1Task_Handler;  /* ������ */
void info_Task(void *pvParameters);         /* ������ */

/* TASK2--CMD ���� ���� */
#define TASK2_PRIO      1                   /* �������ȼ� */
#define TASK2_STK_SIZE  128                 /* �����ջ��С */
TaskHandle_t            Task2Task_Handler;  /* ������ */
void CMD_Task(void *pvParameters);          /* ������ */

/* TASK3--FOC �������� */
#define TASK3_PRIO      3                   /* �������ȼ� */
#define TASK3_STK_SIZE  128                 /* �����ջ��С */
TaskHandle_t            Task3Task_Handler;  /* ������ */
void FOC_Task(void *pvParameters);          /* ������ */


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
  usart_mutex = xSemaphoreCreateMutex();

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
  /* ��������3 */
  xTaskCreate((TaskFunction_t )FOC_Task,
              (const char*    )"FOCTask",
              (uint16_t       )TASK3_STK_SIZE,
              (void*          )NULL,
              (UBaseType_t    )TASK3_PRIO,
              (TaskHandle_t*  )&Task3Task_Handler);
              
  vTaskDelete(StartTask_Handler); /* ɾ����ʼ���� */
  taskEXIT_CRITICAL();            /* �˳��ٽ��� */
}

void info_Task(void *argument)
{
  //char pcWriteBuffer[1024];
  while(1)
  {
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);
    osDelay(1000);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);
		osDelay(1000);
    printf("elec_angle: %.2f, velocity: %.2f\r\n", electricalAngle(&M0_encoder), getVelocity(&M0_encoder));
    // // ����vTaskList()��������ȡ��������Ļ�����Ϣ��������pcWriteBuffer����
    //   vTaskList(pcWriteBuffer);

    //   // ��ӡpcWriteBuffer���������
    //   printf("Name\tState\tPriority\tRemainStack\tSequence\r\n");
    //   printf("%s\r\n", pcWriteBuffer);

    //   // ����uxTaskGetStackHighWaterMark()��������ȡ����1������2����Сʣ��ջ�ռ䣬����ӡ����
    //   //printf("Task 1 minimum remaining stack: %d bytes\r\n", uxTaskGetStackHighWaterMark(xTask1Handle));
    //   //printf("Task 2 minimum remaining stack: %d bytes\r\n", uxTaskGetStackHighWaterMark(xTask2Handle));

    //   // ����vTaskGetRunTimeStats()��������ȡ���������������Ϣ��������pcWriteBuffer����
    //   vTaskGetRunTimeStats(pcWriteBuffer);

    //   // ��ӡpcWriteBuffer���������
    //   printf("Name\tRunCounts\tUsingRate\r\n");
    //   printf("%s\r\n", pcWriteBuffer);
  }
}

void CMD_Task(void *argument)
{
	//FOC_init(12.0f,7,1); //it should be put here, while I don't konw why
  //AS5600_test();
	while(1){
		CMD_ctrl();
		osDelay(100);
	}
}

void FOC_Task(void *argument)
{
  FOC_init(12.0f,7,1); //it should be put here, while I don't konw why

  while(1){
    switch(control_mode)
    {
      case TORQUE:
        setTorque(target_torque_0, electricalAngle(&M0_encoder));
        break;
      case VELOCITY:
        setVelocity(target_velocity_0);
        break;
      case POSITION:
        setPosition(target_position_0);
        break;
      default:
        break;
    }
    osDelay(10);
  }
}

