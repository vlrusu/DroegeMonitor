#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "ADS124S08.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>



// Define SPI and GPIO pin configuration
#define SPI_PORT spi1
#define PIN_MISO 12
#define PIN_CS   13
#define PIN_SCK  10
#define PIN_MOSI 11

#define UART_TX_PIN 0
#define UART_RX_PIN 1

#define UART_ID uart0
#define BAUD_RATE 115200
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY    UART_PARITY_NONE

// Main function
int main() {
    stdio_init_all();

    //    uart_init(UART_ID, 2400);


    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    gpio_init(PIN_SCK);
    gpio_set_dir(PIN_SCK, GPIO_OUT);
    gpio_put(PIN_SCK, 0);
    gpio_init(PIN_MOSI);
    gpio_set_dir(PIN_MOSI, GPIO_OUT);
    gpio_set_pulls(PIN_MOSI, true, false);
    gpio_put(PIN_MOSI, 0);
    gpio_init(PIN_MISO);
    gpio_set_dir(PIN_MISO, GPIO_IN);

    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
    gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);

    spi_init(spi1, 5000000); // Initialize SPI at 500 kHz
    spi_set_format(spi1, 8, SPI_CPOL_0, SPI_CPHA_1, SPI_MSB_FIRST);

    
    ADS124S08 adc;
    ads124s08_init(&adc, PIN_CS) ;

    ads124s08_command(&adc,ADS124S08_CMD_NOP);
    ads124s08_command(&adc,ADS124S08_CMD_WAKEUP);
    ads124s08_command(&adc,ADS124S08_CMD_NOP);
    ads124s08_command(&adc,ADS124S08_CMD_RESET);
    sleep_ms(1000);

    //PGA enabled GAIN=1
    ads124s08_write_register(&adc,ADS124S08_REG_PGA,0x08);

    //CHOP enabled and 100SPS
    ads124s08_write_register(&adc,ADS124S08_REG_DATARATE,0x07);

    //internal reference 2.5V
    ads124s08_write_register(&adc,ADS124S08_REG_REF,0x3A);


    //send status BYTE and 8 samples for calibration 
    ads124s08_write_register(&adc,ADS124S08_REG_SYS,0x91);

    
    //
    //    ads124s08_command(&adc,ADS124S08_CMD_STOP);


    
    //sys 8 samples(default) disable CRC at the end and enable STATUS in preamble (4 words in DATAREAD) 
    ads124s08_write_register(&adc,ADS124S08_REG_SYS,0x11);



    uint8_t reg;    
    
    ads124s08_command(&adc,ADS124S08_CMD_START);
    ads124s08_command(&adc,ADS124S08_CMD_SFOCAL);
    ads124s08_command(&adc,ADS124S08_CMD_STOP);      

    ads124s08_command(&adc,ADS124S08_CMD_START);

    float conversion_delay = 50;
    float i1,i2,i3,v1,v2,v3;
    while (true) {


      
      
      //      ads124s08_command(&adc,ADS124S08_CMD_START);
      /* printf("OFCAL0: %x\n", ads124s08_read_register(&adc,ADS124S08_REG_OFCAL0)); */
      /* printf("OFCAL0: %x\n", ads124s08_read_register(&adc,ADS124S08_REG_OFCAL1)); */
      /* printf("OFCAL0: %x\n", ads124s08_read_register(&adc,ADS124S08_REG_OFCAL2));        */
      
      /* printf("REF: %x\n", ads124s08_read_register(&adc,ADS124S08_REG_REF)); */
      /* printf("PGA: %x\n", ads124s08_read_register(&adc,ADS124S08_REG_PGA)); */
      /* printf("STATUS: %x\n", ads124s08_read_register(&adc,ADS124S08_REG_STATUS)); */


      //      ads124s08_command(&adc,ADS124S08_CMD_START);      
      uint8_t channel = 2;
      reg = (channel << 4) | 0xC;
      ads124s08_write_register(&adc,ADS124S08_REG_INPMUX,reg);
      sleep_ms(conversion_delay);
      i1 = ads124s08_read_data(&adc)/2;

      channel = 3;
      reg = (channel << 4) | 0xC;
      ads124s08_write_register(&adc,ADS124S08_REG_INPMUX,reg);
      sleep_ms(conversion_delay);
      i2 = ads124s08_read_data(&adc)/2;

      channel = 4;
      reg = (channel << 4) | 0xC;
      ads124s08_write_register(&adc,ADS124S08_REG_INPMUX,reg);
      sleep_ms(conversion_delay);
      i3 = ads124s08_read_data(&adc)/2;

      //voltages
      channel = 5;
      reg = (channel << 4) | 0xC;
      ads124s08_write_register(&adc,ADS124S08_REG_INPMUX,reg);
      sleep_ms(conversion_delay);
      v1 = ads124s08_read_data(&adc);

      channel = 6;
      reg = (channel << 4) | 0xC;
      ads124s08_write_register(&adc,ADS124S08_REG_INPMUX,reg);
      sleep_ms(conversion_delay);
      v2 = ads124s08_read_data(&adc);


      channel = 7;
      reg = (channel << 4) | 0xC;
      ads124s08_write_register(&adc,ADS124S08_REG_INPMUX,reg);
      sleep_ms(conversion_delay);
      v3 = ads124s08_read_data(&adc);
      

      
      printf("ADC Values: %.5f %.5f %.5f %.5f %.5f %.5f \n", i1,i2,i3,v1,v2,v3);
    }
    
    return 0;
}


