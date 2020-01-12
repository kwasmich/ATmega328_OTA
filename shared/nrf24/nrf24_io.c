//
//  nrf24_io.c
//  TinyRF24
//
//  Created by Michael Kwasnicki on 09.01.18.
//  Copyright © 2018 Michael Kwasnicki. All rights reserved.
//

#include "nrf24_io.h"

#include "config.h"
#include "macros.h"

#include "spi/spi.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <iso646.h>
#include <stdlib.h>
#include <util/delay.h>



#define NRF24_CE_PORT  PORTB
#define NRF24_CE_PIN   PB1
#define NRF24_CSN_PORT PORTB
#define NRF24_CSN_PIN  PB2
#define NRF24_IRQ_PORT PORTD
#define NRF24_IRQ_PIN  PD2
#define NRF24_IRQ_INT  INT0
#define NRF24_IRQ_VECT INT0_vect

#define NRF24_T_POR 100                                                         // ms
#define NRF24_T_CE 11                                                           // µs



nrf24_register_status_u s_nrf24_io_status;
volatile bool s_nrf24_io_irq = false;



// try ISR_NAKED with all its consequences
ISR(NRF24_IRQ_VECT) {
    if (s_nrf24_io_irq) {
        // packet received before the previous one was consumed :(
    }

    s_nrf24_io_irq = true;
}



void nrf24_io_command(const uint8_t in_COMMAND) {
    BIT_CLR(NRF24_CSN_PORT, _BV(NRF24_CSN_PIN));
    s_nrf24_io_status.u8 = spi_exchange(in_COMMAND);
    BIT_SET(NRF24_CSN_PORT, _BV(NRF24_CSN_PIN));
}



uint8_t nrf24_io_command_1(const uint8_t in_COMMAND, uint8_t in_DATA) {
    BIT_CLR(NRF24_CSN_PORT, _BV(NRF24_CSN_PIN));
    s_nrf24_io_status.u8 = spi_exchange(in_COMMAND);
    in_DATA = spi_exchange(in_DATA);
    BIT_SET(NRF24_CSN_PORT, _BV(NRF24_CSN_PIN));
    return in_DATA;
}



void nrf24_io_command_n(const uint8_t in_COMMAND, const uint8_t in_LENGTH, uint8_t in_out_payload[static const in_LENGTH]) {
    BIT_CLR(NRF24_CSN_PORT, _BV(NRF24_CSN_PIN));
    s_nrf24_io_status.u8 = spi_exchange(in_COMMAND);

    for (uint8_t i = 0; i < in_LENGTH; i++) {
        if (in_out_payload == NULL) {
            spi_exchange(0x00);
        } else {
            in_out_payload[i] = spi_exchange(in_out_payload[i]);
        }
    }

    BIT_SET(NRF24_CSN_PORT, _BV(NRF24_CSN_PIN));
}



void nrf24_io_ce_pulse() {
    BIT_SET(NRF24_CE_PORT, _BV(NRF24_CE_PIN));
    _delay_us(NRF24_T_CE);
    BIT_CLR(NRF24_CE_PORT, _BV(NRF24_CE_PIN));
}



void nrf24_io_ce_hi() {
    BIT_SET(NRF24_CE_PORT, _BV(NRF24_CE_PIN));
}



void nrf24_io_ce_lo() {
    BIT_CLR(NRF24_CE_PORT, _BV(NRF24_CE_PIN));
}



void nrf24_io_init() {
    uint8_t sreg = SREG;                                                        // store previous interrupt status
    cli();                                                                      // disable interrupts
#if NRF24_IRQ_INT == INT0
    BIT_SET(EICRA, _BV(ISC01));                                                 // interrupt on falling edge of INT0
    BIT_CLR(EICRA, _BV(ISC00));
#elif NRF24_IRQ_INT == INT1
    BIT_SET(EICRA, _BV(ISC11));                                                 // interrupt on falling edge of INT1
    BIT_CLR(EICRA, _BV(ISC10));
#else
#   error "no interrupt defined!"
#endif
    
    BIT_SET(EIMSK, _BV(NRF24_IRQ_INT));                                         // enable interrupt on INTx
    SREG = sreg;                                                                // restore previous interrupt status

    BIT_CLR(DDRD, _BV(NRF24_IRQ_PIN));                                          // IRQ is input
    BIT_SET(DDRB, _BV2(NRF24_CE_PIN, NRF24_CSN_PIN));                           // CE and CSN are outputs
    BIT_SET(PORTB, _BV(NRF24_CSN_PIN));                                         // keep CSN up unless we are sending data to nRF
    _delay_ms(NRF24_T_POR);                                                     // wait for nRF24L01+ Oscillator to sattle (T_POR Power on reset)
}
