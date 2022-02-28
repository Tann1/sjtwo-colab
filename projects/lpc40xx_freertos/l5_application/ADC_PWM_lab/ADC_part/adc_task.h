#pragma once

#include <stdio.h>

#include "FreeRTOS.h"
#include "adc.h"
#include "lpc40xx.h"
#include "task.h"

void adc_task(void *p);