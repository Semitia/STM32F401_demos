#ifndef __FOC_H
#define __FOC_H

#include "main.h"
#include "cmsis_os.h"
#include "TIM.h"
#include "AS5600.h"
#include "stdio.h"
#include "usart.h"
#include "math.h"
#include "lowpass.h"
#include "PID.h"

void FOC_init(float _voltage_power_supply, int _PP, int _DIR);
void setPWM(float Ua,float Ub,float Uc);
void setTorque(float Uq, float angle);
void alignSensor(void);
float normalizeAngle(float angle);
float electricalAngle(AS5600_t *encoder);
extern AS5600_t M0_encoder, M1_encoder;

#endif

