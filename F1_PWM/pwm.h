#ifndef __PWM_H__
#define __PWM_H__

#include "stm32f10x.h"
#include <rthw.h>
#include <rtthread.h>

void set_pwm(uint16_t arr,uint16_t psc,uint16_t pulse);

#endif

