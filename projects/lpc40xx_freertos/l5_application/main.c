#include <math.h>
#include <stdio.h>

#include "FreeRTOS.h"
#include "clock.h"
#include "periodic_scheduler.h"
#include "queue.h"
#include "task.h"

#include "acceleration.h"
#include "ff.h"

#define SAMPLE_SIZE 100

QueueHandle_t sensor_queue;

float magnitude(acceleration__axis_data_s data) {
  uint32_t x = pow(data.x, 2);
  uint32_t y = pow(data.y, 2);
  uint32_t z = pow(data.z, 2);

  return pow(x + y + z, 0.5);
}

void producer(void *p) {
  uint16_t count = 0;
  float sum = 0;
  float avg = 0;
  while (1) {
    sum += magnitude(acceleration__get_data());
    count++;
    if (count == SAMPLE_SIZE) {
      avg = sum / SAMPLE_SIZE;
      xQueueSend(sensor_queue, &avg, 0);
      sum = 0;   // reset sum
      count = 0; // reset count
    }
    vTaskDelay(1);
  }
}

void consumer(void *p) {
  float data;
  while (1) {
    if (xQueueReceive(sensor_queue, &data, portMAX_DELAY)) {
      fprintf(stderr, "Received avg: %.2f\n", (double)data);
    }
  }
}

int main(void) {

  sensor_queue = xQueueCreate(1, sizeof(float));

  acceleration__init();

  xTaskCreate(producer, "producer", 512, NULL, PRIORITY_MEDIUM, NULL);
  xTaskCreate(consumer, "consumer", 512, NULL, PRIORITY_MEDIUM, NULL);

  vTaskStartScheduler(); // This function never returns unless RTOS scheduler runs out of memory and fails

  return 0;
}
