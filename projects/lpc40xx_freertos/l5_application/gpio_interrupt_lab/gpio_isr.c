// @file gpio_isr.c
#include "gpio_isr.h"
#include "lpc40xx.h"

// Note: You may want another separate array for falling vs. rising edge callbacks
static function_pointer_t gpio0_callbacks_r[32]; // rising callbacks
static function_pointer_t gpio0_callbacks_f[32]; // falling callbacks

static uint32_t get_respective_pin(); // get the pin that generated the interrupt
static void clear_pin_interrupt(uint8_t pin_that_generated_interrupt);

void gpio0__attach_interrupt(uint8_t pin, gpio_interrupt_e interrupt_type, function_pointer_t callback) {
  uint32_t target_pin = (1U << pin);
  // 1) Store the callback based on the pin at gpio0_callbacks
  // 2) Configure GPIO 0 pin for rising or falling edge

  if (interrupt_type == GPIO_INTR__FALLING_EDGE) {
    LPC_GPIOINT->IO0IntEnF |= target_pin;
    gpio0_callbacks_f[pin] = callback;
  } else // must righting edge
    LPC_GPIOINT->IO0IntEnR |= target_pin;
  gpio0_callbacks_r[pin] = callback;
}

// We wrote some of the implementation for you
void gpio0__interrupt_dispatcher(void) {
  // Check which pin generated the interrupt
  function_pointer_t attached_user_handler;
  const int pin_that_generated_interrupt = get_respective_pin();
  if (LPC_GPIOINT->IO0IntStatR & (1U << pin_that_generated_interrupt))
    attached_user_handler = gpio0_callbacks_r[pin_that_generated_interrupt];
  else
    attached_user_handler = gpio0_callbacks_f[pin_that_generated_interrupt];

  // Invoke the user registered callback, and then clear the interrupt
  attached_user_handler();
  clear_pin_interrupt(pin_that_generated_interrupt);
}

static uint32_t get_respective_pin() {
  for (uint32_t bit = 0; bit < 32; ++bit) {
    if (LPC_GPIOINT->IO0IntStatF & (1U << bit))
      return bit;
    if (LPC_GPIOINT->IO0IntStatR & (1U << bit))
      return bit;
  }
}

static void clear_pin_interrupt(uint8_t pin_that_generated_interrupt) {
  LPC_GPIOINT->IO0IntClr |= (1U << pin_that_generated_interrupt);
}