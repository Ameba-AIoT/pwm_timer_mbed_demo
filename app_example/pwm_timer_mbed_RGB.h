#ifndef __PWM_TIMER_MBED_RGB__
#define __PWM_TIMER_MBED_RGB__
#include "os_wrapper.h"
#include <stdio.h>
#include "main.h"
#include "device.h"
#include "gpio_api.h"   // mbed
#include "timer_api.h"

#define TAG    "PWM_TIMER_DEMO"
//定义三个gpio对应的evb pin idex
#define LED1_PIN _PA_14
#define LED2_PIN _PA_15
#define LED3_PIN _PA_16

#define PWM_TIME_US  2000000
#define LED2_TIME_US PWM_TIME_US / 8
#define LED3_TIME_US PWM_TIME_US / 4

void pwm_timer_mbed_RGB(void);
void timer1_timeout_handler(uint32_t id);
void timer2_timeout_handler(uint32_t id);
void timer3_timeout_handler(uint32_t id);

#endif
