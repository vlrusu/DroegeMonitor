#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "ADS124S08.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>



// Define SPI and GPIO pin configuration
#define SPI_PORT spi0
#define PIN_MISO 4
#define PIN_CS   5
#define PIN_SCK  6
#define PIN_MOSI 7


// Main function
int main() {
    stdio_init_all();

    gpio_init(PIN_SCK);
    gpio_set_dir(PIN_SCK, GPIO_OUT);
    gpio_put(PIN_SCK, 0);
    gpio_init(PIN_MOSI);
    gpio_set_dir(PIN_MOSI, GPIO_OUT);
    gpio_set_pulls(PIN_MOSI, true, false);
    gpio_put(PIN_MOSI, 0);
    gpio_init(PIN_MISO);
    gpio_set_dir(PIN_MISO, GPIO_IN);

    spi_init(spi0, 500000); // Initialize SPI at 500 kHz
    spi_set_format(spi0, 24, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);

    
    ADS124S08 adc;
    ads124s08_init(&adc, PIN_CS) ;

    ads124s08_command(&adc,ADS124S08_CMD_NOP);
    ads124s08_command(&adc,ADS124S08_CMD_WAKEUP);
    ads124s08_command(&adc,ADS124S08_CMD_NOP);
    ads124s08_command(&adc,ADS124S08_CMD_RESET);


    ads124s08_write_register(&adc,ADS124S08_REG_PGA,0x08);
    ads124s08_write_register(&adc,ADS124S08_REG_DATARATE,0x34);
    ads124s08_write_register(&adc,ADS124S08_REG_REF,0x0A);
    ads124s08_write_register(&adc,ADS124S08_REG_SYS,0x91);

    
    //    ads124s08_command(&adc,ADS124S08_CMD_START);
    //    ads124s08_command(&adc,ADS124S08_CMD_STOP);

    
      
      
    while (true) {
      int32_t adc_value = ads124s08_readDeviceID(&adc);
      printf("ADC Value: %ld\n", adc_value);
      sleep_ms(1000); 
    }
    
    return 0;
}


