//
//  nrf24_x.h
//  MegaRF24
//
//  Created by Michael Kwasnicki on 04.01.19.
//  Copyright © 2019 Michael Kwasnicki. All rights reserved.
//

#ifndef nrf24_h
#define nrf24_h


#include <stdbool.h>
#include <stdint.h>



void nrf24_rx_start(void);
void nrf24_rx_stop(void);
void nrf24_flush_rx(void);
void nrf24_flush_tx(void);
void nrf24_tx(const uint8_t in_LENGTH, const uint8_t in_PAYLOAD[static const in_LENGTH]);
bool nrf24_tx_done(void);
bool nrf24_tx_fail(void);
bool nrf24_rx(uint8_t *pipe, uint8_t *length, uint8_t payload[static const 32]);
bool nrf24_enqueue_ack_payload(const uint8_t in_PIPE, const uint8_t in_LENGTH, const uint8_t in_PAYLOAD[static const in_LENGTH]);
void nrf24_clear_interrupts(void);
void nrf24_clear_plos_cnt(void);
void nrf24_init(void);
void nrf24_debug(void);
void nrf24_carrier_start(void);
void nrf24_carrier_stop(void);



#endif /* nrf24_x_h */
