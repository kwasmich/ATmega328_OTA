#define EXTERNAL_MEMORY_I2C 1
#define EXTERNAL_MEMORY_SPI 2

//#undef EXTERNAL_MEMORY
//#define EXTERNAL_MEMORY EXTERNAL_MEMORY_I2C
//#define EXTERNAL_MEMORY EXTERNAL_MEMORY_SPI


//#define NUM_PAGES       ((FLASHEND + 1UL) / SPM_PAGESIZE)                       // 256
//#define NUM_WRITE_PAGES 8
//#define NUM_BOOT_PAGES  8


// I²C
//#define I2C_SPEED 400000
//#define I2C_EEPROM_ADDRESS 0x50
//#define I2C_EEPROM_PAGE_SIZE 128

// Intel HEX
#define IHEX_DATA_MAX 32

// LCD 16x2
#define LCD_INTERFACE_4_BIT
//#define LCD_STDOUT

// SPI
//#define SPI_EEPROM_24BIT
//#define SPI_STDOUT

// TIME
#define TIME_WDT
//#define TIME_TC0
//#define TIME_TC1
//#define TIME_TC2

// UART
#define UART_BAUD 19200
#define UART_RX_BUFFER_SIZE 4
#define UART_TX_BUFFER_SIZE 4
#define UART_STDOUT

// nRF24L01+
//#define NRF24_CE_PORT  PORTB
//#define NRF24_CE_PIN   PB1
//#define NRF24_CSN_PORT PORTB
//#define NRF24_CSN_PIN  PB2
//#define NRF24_IRQ_PORT PORTD
//#define NRF24_IRQ_PIN  PD2
//#define NRF24_IRQ_INT  INT0
//#define NRF24_IRQ_VECT INT0_vect
//#define NRF24_CRC_BYTES             NRF_CRC_BYTES_2
//#define NRF24_ADDRESS_WIDTH         NRF24_ADDRESS_WIDTH_5
//#define NRF24_AUTO_RETRANSMIT_DELAY NRF24_AUTO_RETRANSMIT_DELAY_500US
//#define NRF24_AUTO_RETRANSMIT_COUNT 0
//#define NRF24_RF_CHANNEL            0x2E                                        // 0-125
//#define NRF24_DATA_RATE             NRF24_DATA_RATE_2000
//#define NRF24_RF_POWER              NRF24_RF_POWER_18DBM
//#define NRF24_
