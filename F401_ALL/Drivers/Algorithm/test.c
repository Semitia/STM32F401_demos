#include "test.h"

void LED_test(void){
    while(1)
    {
        //HAL_UART_Transmit(&huart1,USART1_BUF,sizeof(USART1_BUF),0xffff);
        HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);
        osDelay(1000);
        HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);
        osDelay(1000);
    }
}

void PWM_test(void){
    uint8_t pwm = 0;
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 20);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 40);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 60);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, 80);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 30);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 70);
    while(1){
        pwm += 20;
        pwm %= 100;
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, pwm);
        printf("pwm: %d\r\n",pwm); 
        osDelay(1000);
    }

}

void AS5600_test(void){
    // iic_init();								/* Initialize IIC (it should be excute after delay_init)*/  
    // AS5600_t *as1 = (AS5600_t *)malloc(sizeof(AS5600_t));
    // if(as1 == NULL) {
    //     printf("as1 malloc failed\r\n");
    //     return;
    // }
    // AS_init(as1,1);

    while(1)
    {
        AS_update(&M0_encoder);
        printf("angle: %.2f, timestamp: %d\r\n", getMechanicalAngle(&M0_encoder), M0_encoder.ts);
        printf("velocity: %.2f\r\n", getVelocity(&M0_encoder));
        osDelay(1000);
    }
}

void USART_RX_test(void){
    if(g_usart_rx_sta & 0x8000)
    {
        printf("got: %s \r\n",g_usart_rx_buf);
        g_usart_rx_sta = 0;
    }
    osDelay(100);
}

/**
 * @brief       串口指令集
 *  byte 0:     指令类型
 *      0x00:   6通道PWM占空比调节
 *          byte 1:     通道序号
 *          byte 2:     占空比(0~100)     
 *      0x01:   读取编码器信息
 *      0x02:   步进
 *         byte 1~2:     Motor0_Uq*1000
 *         byte 3~4:     Motor1_Uq*1000
 *      0x03:   设置力矩
 *         byte 1~2:     Motor0_Uq*1000
 *         byte 3~4:     Motor1_Uq*1000
 *      0x04:   设置速度
 *         byte 1~2:     Motor0_velocity*1000
 *         byte 3~4:     Motor1_velocity*1000
 *      0x05:   设置位置
 *         byte 1~2:     Motor0_position*1000
 *         byte 3~4:     Motor1_position*1000
 *      0x06:   设置PID参数
 *         byte 1:     PID类型
 *              0x00:   M0_vel_PID
 *              0x01:   M1_vel_PID
 *              0x02:   M0_pos_PID
 *              0x03:   M1_pos_PID
 *        byte 2~3:     KP*1000
 *        byte 4~5:     KI*1000
 *        byte 6~7:     KD*1000
 * 
 * 
 * @param       无
 * 
*/
void CMD_ctrl(void){
    if(g_usart_rx_sta & 0x8000)
    {
        switch(g_usart_rx_buf[0]){
            case 0x00:
                switch(g_usart_rx_buf[1]){
                    case 0x00:
                            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, g_usart_rx_buf[2]);
                            break;
                    case 0x01:
                            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, g_usart_rx_buf[2]);
                            break;
                    case 0x02:
                            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, g_usart_rx_buf[2]);
                            break;
                    case 0x03:
                            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, g_usart_rx_buf[2]);
                            break;
                    case 0x04:
                            __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, g_usart_rx_buf[2]);
                            break;
                    case 0x05:
                            __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, g_usart_rx_buf[2]);
                            break;
                    default:
                    printf("wrong channel number\r\n");
                            break;
                }
                printf("set channel: %d, pwm: %d\r\n",g_usart_rx_buf[1],g_usart_rx_buf[2]);
                break;
            case 0x01:
                AS_update(&M0_encoder);
                printf("M0: \r\n  timestamp: %d, angle: %.2f, elec_angle:%.2f, velocity: %.2f\r\n", M0_encoder.ts, getMechanicalAngle(&M0_encoder), electricalAngle(&M0_encoder), getVelocity(&M0_encoder));
                //printf("M1: \r\ntimestamp: %d, angle: %.2f, velocity: %.2f\r\n", M1_encoder.ts, getMechanicalAngle(&M1_encoder),getVelocity(&M1_encoder));
                break;
            case 0x02:
                setTorque((float)(g_usart_rx_buf[1]*256+g_usart_rx_buf[2])/1000.0f, electricalAngle(&M0_encoder));
                printf("step forward\r\n");
                break;
            case 0x03:
                target_torque_0 = (float)(g_usart_rx_buf[1]*256+g_usart_rx_buf[2])/1000.0f;
                target_torque_1 = (float)(g_usart_rx_buf[3]*256+g_usart_rx_buf[4])/1000.0f;
                control_mode = TORQUE;
                printf("set torque: %.2f, %.2f\r\n",target_torque_0,target_torque_1);
                break;
            case 0x04:
                target_velocity_0 = (float)(g_usart_rx_buf[1]*256+g_usart_rx_buf[2])/1000.0f;
                target_velocity_1 = (float)(g_usart_rx_buf[3]*256+g_usart_rx_buf[4])/1000.0f;
                control_mode = VELOCITY;
                printf("set velocity: %.2f, %.2f\r\n",target_velocity_0,target_velocity_1);
                break;
            case 0x05:
                target_position_0 = (float)(g_usart_rx_buf[1]*256+g_usart_rx_buf[2])/1000.0f;
                target_position_1 = (float)(g_usart_rx_buf[3]*256+g_usart_rx_buf[4])/1000.0f;
                control_mode = POSITION;
                printf("set position: %.2f, %.2f\r\n",target_position_0,target_position_1);
                break;
            case 0x06: {
                PID_t *pid;
                switch (g_usart_rx_buf[1])
                {
                    case 0x00:
                        pid = &M0_vel_PID;
                        break;
                    case 0x01:
                        pid = &M1_vel_PID;
                        break;
                    case 0x02:  
                        pid = &M0_pos_PID;
                        break;
                    case 0x03:
                        pid = &M1_pos_PID;
                        break;
                    default:
                        printf("wrong PID type\r\n");
                        break;
                }
                pid->Kp = (float)(g_usart_rx_buf[2]*256+g_usart_rx_buf[3])/1000.0f;
                pid->Ki = (float)(g_usart_rx_buf[4]*256+g_usart_rx_buf[5])/1000.0f;
                pid->Kd = (float)(g_usart_rx_buf[6]*256+g_usart_rx_buf[7])/1000.0f;
                printf("set PID: %.3f, %.3f, %.3f\r\n",pid->Kp,pid->Ki,pid->Kd);
                break;
            }
            default:
                printf("wrong command\r\n");
                break;
        }

        g_usart_rx_sta = 0;
    }
    else
    {return;}
}






