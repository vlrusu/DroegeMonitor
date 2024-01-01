// ADS124S08 struct definition
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include <math.h>

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef ADS124S08_H
#define ADS124S08_H


#define ADS124S08_CMD_RREG               1
#define ADS124S08_CMD_WREG               2

#define ADS124S08_CMD_NOP  0x0
#define ADS124S08_CMD_WAKEUP  0x2
#define ADS124S08_CMD_POWERDOWN  0x4
#define ADS124S08_CMD_RESET  0x6
#define ADS124S08_CMD_START  0x8
#define ADS124S08_CMD_STOP   0xA
#define ADS124S08_CMD_SYOCAL  0x16
#define ADS124S08_CMD_SYGCAL  0x17
#define ADS124S08_CMD_SFOCAL  0x18
#define ADS124S08_CMD_RDATA  0x12

#define ADS124S08_REG_ID  0x0
#define ADS124S08_REG_STATUS  0x1
#define ADS124S08_REG_INPMUX  0x2
#define ADS124S08_REG_PGA  0x3
#define ADS124S08_REG_DATARATE  0x4
#define ADS124S08_REG_REF  0x5
#define ADS124S08_REG_IDACMAG  0x6
#define ADS124S08_REG_IDACMUX  0x7
#define ADS124S08_REG_VBIAS  0x8
#define ADS124S08_REG_SYS  0x9
#define ADS124S08_REG_OFCAL0  0xA
#define ADS124S08_REG_OFCAL1  0xB
#define ADS124S08_REG_OFCAL2  0xC
#define ADS124S08_REG_FSCAL0  0xD
#define ADS124S08_REG_FSCAL1  0xE
#define ADS124S08_REG_FSCAL2  0xF
#define ADS124S08_REG_GPIODAT  0x10
#define ADS124S08_REG_GPIOCON  0x11

#define ADS124S08_REFV 2.5



typedef struct {
    uint spi_port;
    uint cs_pin;
  //    uint start_pin;
  //    uint drdy_pin;
} ADS124S08;

// Function declarations
void ads124s08_init(ADS124S08* adc,  uint cs_pin);
void ads124s08_write_register(ADS124S08* adc,uint8_t reg, uint8_t value);
uint8_t ads124s08_read_register(ADS124S08* adc,uint8_t reg);
float ads124s08_read_data(ADS124S08* adc);

uint8_t ads124s08_readDeviceID(ADS124S08* adc);

uint8_t ads124s08_readStatus(ADS124S08* adc);

uint8_t ads124s08_readStatus(ADS124S08* adc);

uint8_t ads124s08_readInternalRef(ADS124S08* adc);

void ads124s08_command(ADS124S08* adc, uint8_t command );



#endif /* ADS124S08*/
