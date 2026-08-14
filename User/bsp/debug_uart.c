#include "debug_uart.h"

#include "gd32f10x.h"

#define DEBUG_USART         USART0
#define DEBUG_USART_GPIO    GPIOA
#define DEBUG_USART_TX_PIN  GPIO_PIN_9

void debug_uart_init(void)
{
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_USART0);
    gpio_init(DEBUG_USART_GPIO, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ,
              DEBUG_USART_TX_PIN);
    usart_deinit(DEBUG_USART);
    usart_baudrate_set(DEBUG_USART, 115200U);
    usart_word_length_set(DEBUG_USART, USART_WL_8BIT);
    usart_stop_bit_set(DEBUG_USART, USART_STB_1BIT);
    usart_parity_config(DEBUG_USART, USART_PM_NONE);
    usart_transmit_config(DEBUG_USART, USART_TRANSMIT_ENABLE);
    usart_enable(DEBUG_USART);
}

int _write(int file, char *ptr, int len)
{
    int index;

    (void)file;
    for (index = 0; index < len; index++) {
        usart_data_transmit(DEBUG_USART, (uint8_t)ptr[index]);
        while (RESET == usart_flag_get(DEBUG_USART, USART_FLAG_TBE)) {
        }
    }
    return len;
}
