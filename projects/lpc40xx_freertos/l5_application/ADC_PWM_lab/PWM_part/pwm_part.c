#include "pwm_part.h"

static gpio_s pwn_channel; // just in case we need to manipulate the pin
static uint8_t pwn_pin = 0;

void pwm_task(void *p) {
  pwm1__init_single_edge(1000);

  // Locate a GPIO pin that a PWM channel will control
  // NOTE You can use gpio__construct_with_function() API from gpio.h
  // TODO Write this function yourself
  pwn_channel =
      gpio__construct_with_function(GPIO__PORT_2, pwn_pin, GPIO__FUNCTION_1); // function 1 is PWN functionality

  // We only need to set PWM configuration once, and the HW will drive
  // the GPIO at 1000Hz, and control set its duty cycle to 50%
  pwm1__set_duty_cycle(PWM1__2_0, 50);

  // Continue to vary the duty cycle in the loop
  uint8_t percent = 0;
  while (1) {
    pwm1__set_duty_cycle(PWM1__2_0, percent);

    if (++percent > 50)
      percent = 0;

    vTaskDelay(100);
  }
}
