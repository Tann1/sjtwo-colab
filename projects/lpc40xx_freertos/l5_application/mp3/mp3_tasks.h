#pragma once

#include "FreeRTOS.h"
#include "queue.h"
#include "struct_definition.h"
#include "ff.h"
#include <stdio.h>

extern QueueHandle_t songname_q;

void mp3_reader_task(void *p);
void mp3_player_task(void *p);