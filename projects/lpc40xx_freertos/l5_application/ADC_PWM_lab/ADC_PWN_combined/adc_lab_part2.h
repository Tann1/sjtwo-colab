#pragma once

#include <stdint.h>
#include <stdio.h>

#include "adc.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

void adc_task(void *p);
void pwm_task(void *p);