#pragma once

#include "pwm1.h"

#include "FreeRTOS.h"
#include "gpio.h"
#include "task.h"

void pwm_task(void *p);