#include <stdio.h>

#include "FreeRTOS.h"
#include "adc.h"
#include "gpio.h"
#include "lpc40xx.h"
#include "periodic_scheduler.h"
#include "pwm1.h"
#include "queue.h"
#include "task.h"

static QueueHandle_t adc_to_pwm_task_queue;
static gpio_s pwn_channel;

static void pin_configure_adc_channel_as_io_pin(void) {
  LPC_IOCON->P1_31 |= 0b011;        // ADC function
  LPC_IOCON->P1_31 &= ~(0b11 << 3); // set MODE to 0b00
  LPC_IOCON->P1_31 &= ~(1U << 7);   // set analog mode
}

void adc_task_part2(void *p) {
  // NOTE: Reuse the code from Part 1
  adc__initialize();
  adc__enable_burst_mode();
  fprintf(stderr, "control register: 0x%lx\n", LPC_ADC->CR); // making sure CR is configured properly
  pin_configure_adc_channel_as_io_pin();                     // Note that P1.31 is A type IOCON stands for Analog

  uint32_t adc_reading = 0; // Note that this 'adc_reading' is not the same variable as the one from adc_task
  float adc_voltage = 0;
  while (1) {
    // Implement code to send potentiometer value on the queue
    // a) read ADC input to 'int adc_reading'
    adc_reading = adc__get_channel_reading_with_burst_mode(ADC__CHANNEL_5);
    // b) Send to queue: xQueueSend(adc_to_pwm_task_queue, &adc_reading, 0);
    adc_voltage = (3.3 / 4096) * adc_reading;
    fprintf(stderr, "adc voltage: %.2f\n", adc_voltage);
    xQueueSend(adc_to_pwm_task_queue, &adc_reading, 0);

    vTaskDelay(100);
  }
}

void pwm_task_part2(void *p) {
  // NOTE: Reuse the code from Part 0
  uint32_t adc_reading = 0;
  uint8_t pwn_pin = 0;
  float percent = 0;
  pwm1__init_single_edge(1000);

  // Locate a GPIO pin that a PWM channel will control
  // NOTE You can use gpio__construct_with_function() API from gpio.h
  // TODO Write this function yourself
  pwn_channel =
      gpio__construct_with_function(GPIO__PORT_2, pwn_pin, GPIO__FUNCTION_1); // function 1 is PWN functionality

  // We only need to set PWM configuration once, and the HW will drive
  // the GPIO at 1000Hz, and control set its duty cycle to 50%

  while (1) {
    // Implement code to receive potentiometer value from queue
    if (xQueueReceive(adc_to_pwm_task_queue, &adc_reading, 100)) {
      // fprintf(stderr, "Recieved value: %lu\n", adc_reading);
      percent = (adc_reading / 4096.0) * 100;
      fprintf(stderr, "percent: %.2f\n", percent);
      pwm1__set_duty_cycle(PWM1__2_0, percent);
      // fprintf(stderr, "percent: %.2f\n", percent);
      // fprintf(stderr, "MR0: 0x%lx\n", LPC_PWM1->MR0);
      // fprintf(stderr, "MR1: 0x%lx\n", LPC_PWM1->MR1);
    }

    // We do not need task delay because our queue API will put task to sleep when there is no data in the queue
    // vTaskDelay(100);
  }
}

int main(void) {
  adc_to_pwm_task_queue = xQueueCreate(1, sizeof(int));
  xTaskCreate(pwm_task_part2, "pwm_task", (512U * 4) / sizeof(void *), NULL, PRIORITY_LOW, NULL);
  xTaskCreate(adc_task_part2, "adc_task", (512U * 4) / sizeof(void *), NULL, PRIORITY_HIGH, NULL);

  puts("Starting RTOS");
  vTaskStartScheduler(); // This function never returns unless RTOS scheduler runs out of memory and fails

  return 0;
}
