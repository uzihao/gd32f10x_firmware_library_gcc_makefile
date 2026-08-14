#include "board.h"
#include "debug_uart.h"

#include "gd32f10x.h"

#define BOARD_LED_GPIO GPIOA
#define BOARD_LED_PIN  GPIO_PIN_0

void board_init(void)
{
    debug_uart_init();
    rcu_periph_clock_enable(RCU_GPIOA);
    gpio_init(BOARD_LED_GPIO, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, BOARD_LED_PIN);
    board_led_off();
}

void board_led_on(void)
{
    gpio_bit_set(BOARD_LED_GPIO, BOARD_LED_PIN);
}

void board_led_off(void)
{
    gpio_bit_reset(BOARD_LED_GPIO, BOARD_LED_PIN);
}
