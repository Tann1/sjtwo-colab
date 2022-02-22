#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"

#include "board_io.h"
#include "common_macros.h"
#include "gpio.h"
#include "gpio_isr.h"
#include "lpc_peripherals.h"
#include "periodic_scheduler.h"
#include "semphr.h"
#include "sj2_cli.h"

static gpio_s _switch, led;
static uint8_t sw_pin = 30, led_pin = 24;

static SemaphoreHandle_t switch_pressed_signal;

static void config_gpio_interrupt();
static void clear_gpio_interrupt();
static void sleep_on_sem_task(void *p);
void gpio_interrupt(void);

int main(void) {
  switch_pressed_signal = xSemaphoreCreateBinary();
  // xTaskCreate(esp32_tcp_hello_world_task, "uart3", 1000, NULL, PRIORITY_LOW, NULL); // Include esp32_task.h

  //_switch = gpio__construct_as_input(GPIO__PORT_0, sw_pin); // set switch as input
  led = gpio__construct_as_output(GPIO__PORT_1, led_pin); // set led pin
  gpio__set(led);                                         // set led

  lpc_peripheral__enable_interrupt(LPC_PERIPHERAL__GPIO, gpio0__interrupt_dispatcher, "gpio dispatcher");
  // config_gpio_interrupt();
  NVIC_EnableIRQ(GPIO_IRQn);

  gpio0__attach_interrupt(sw_pin, GPIO_INTR__RISING_EDGE, gpio_interrupt);

  xTaskCreate(sleep_on_sem_task, "sem", (512U * 4) / sizeof(void *), NULL, PRIORITY_LOW, NULL);

  puts("Starting RTOS");
  vTaskStartScheduler(); // This function never returns unless RTOS scheduler runs out of memory and fails

  return 0;
}

static void config_gpio_interrupt() {
  LPC_GPIOINT->IO0IntEnR |= (1U << sw_pin); // trigger port0 int on rising edge
  lpc_peripheral__enable_interrupt(LPC_PERIPHERAL__GPIO, gpio_interrupt, "gpio led int");
}

static void clear_gpio_interrupt() {
  LPC_GPIOINT->IO0IntClr |= (1U << sw_pin); // clear the pin of the int signal
}

static void sleep_on_sem_task(void *p) {
  while (1) {
    if (xSemaphoreTake(switch_pressed_signal, portMAX_DELAY)) {
      gpio__reset(led);
      fprintf(stderr, "woke up from sleeping . . .\n");
      vTaskDelay(50);
    }

    gpio__set(led);
  }
}

void gpio_interrupt(void) {

  fprintf(stderr, "GPIO ISR called.\n");
  xSemaphoreGiveFromISR(switch_pressed_signal, NULL);
  clear_gpio_interrupt();
}