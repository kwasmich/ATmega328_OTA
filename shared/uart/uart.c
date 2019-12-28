//
//  uart.c
//  MegaRF24
//
//  Created by Michael Kwasnicki on 15.01.19.
//  Copyright © 2019 Michael Kwasnicki. All rights reserved.
//


#include "uart.h"

#include "config.h"
#include "macros.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <stdio.h>
#include <util/delay.h>


#ifndef UART_BAUD
#   define UART_BAUD 9600
#endif
// #define UART_STDOUT


// setbaud.h must be included after BAUD has ben defined!
#define BAUD UART_BAUD
#include <util/setbaud.h>



volatile static uint8_t uart_received_data;
volatile static bool uart_received_flag;

static uart_callback_t *uart_callback;



ISR(USART_RX_vect) {
    if (uart_received_flag) {
        // next byte received before the previous one was consumed :(
    }

    uart_received_data = UDR0;
    uart_received_flag = true;
}



#ifdef UART_STDOUT
static int uart_putc(char c, FILE *stream) {
    if (c == '\n') {
        uart_putc('\r', stream);
    }

    uart_putchar(c);
    return c;
}
#endif



static void uart_calibrate_osc(uint8_t in_OSCCAL) {
    // calibrate clock to match baud rate = 115.2kHz * 16 * x to match the system Clock ~7.3728MHz
    //OSCCAL = 0x54; //7.352.960

    while (OSCCAL > in_OSCCAL) {
        OSCCAL--;
        _delay_ms(10);
    }

    while (OSCCAL < in_OSCCAL) {
        OSCCAL++;
        _delay_ms(10);
    }

    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;

#if USE_2X
    UCSR0A = _BV(U2X0);
#else
    UCSR0A = 0;
#endif
}



void uart_init(uint8_t in_OSCCAL) {
    uart_calibrate_osc(in_OSCCAL);

    UCSR0B = _BV2(RXEN0, TXEN0);
    UCSR0C = _BV2(UCSZ01, UCSZ00); // 8N1

#ifdef UART_STDOUT
    static FILE uart_stdout = FDEV_SETUP_STREAM(uart_putc, NULL, _FDEV_SETUP_WRITE);
    stdout = &uart_stdout;
    // printf("%ld %ld %d\n", UBRRH_VALUE, UBRRL_VALUE, USE_2X);
#endif

    uart_callback = NULL;
}



void uart_putchar(char c) {
    loop_until_bit_is_set(UCSR0A, UDRE0);   // wait for transmit buffer to be ready
    UDR0 = c;
}



uint8_t uart_getchar() {
    loop_until_bit_is_set(UCSR0A, RXC0);    // wait for data to be received
    return UDR0;
}



void uart_init_async(uart_callback_t * const in_UART_CALLBACK, uint8_t in_OSCCAL) {
    uart_calibrate_osc(in_OSCCAL);

    UCSR0B = _BV3(RXCIE0, RXEN0, TXEN0);
    UCSR0C = _BV2(UCSZ01, UCSZ00); // 8N1

#ifdef UART_STDOUT
    static FILE uart_stdout = FDEV_SETUP_STREAM(uart_putc, NULL, _FDEV_SETUP_WRITE);
    stdout = &uart_stdout;
    // printf("%ld %ld %d\n", UBRRH_VALUE, UBRRL_VALUE, USE_2X);
#endif

    uart_callback = in_UART_CALLBACK;
}



void uart_loop_async() {
    if (uart_received_flag) {
        uart_received_flag = false;
        uart_callback(uart_received_data);
    }
}



bool uart_getchar_async(char * const out_c) {
    if (uart_received_flag) {
        uart_received_flag = false;
        *out_c = uart_received_data;
        return true;
    }

    return false;
}
