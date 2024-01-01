#include "ADS124S08.h"
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>


const float adc_to_V  = ADS124S08_REFV / pow(2, 23) ;

// Initialize ADS124S08
void ads124s08_init(ADS124S08* adc, uint cs_pin) {

    adc->cs_pin = cs_pin;
    //    adc->start_pin = start_pin;
    //    adc->drdy_pin = drdy_pin;

  
    
    // Initialize GPIO pins
    gpio_init(adc->cs_pin);
    gpio_set_dir(adc->cs_pin, GPIO_OUT);
    gpio_put(adc->cs_pin, 1); // Deselect the ADS124S08
    
    /* gpio_init(adc->drdy_pin); */
    /* gpio_set_dir(adc->drdy_pin, GPIO_IN); */
    
    /* gpio_init(adc->start_pin); */
    /* gpio_set_dir(adc->start_pin, GPIO_OUT); */
    /* gpio_put(adc->start_pin, 0); // Keep the START pin low initially */
}

// Write a value to a register
void ads124s08_write_register(ADS124S08* adc,uint8_t reg, uint8_t value) {
  uint8_t tx_buffer[3];// = {(ADS124S08_CMD_WREG << 5 | reg), 0x00, value};
  tx_buffer[0] = (ADS124S08_CMD_WREG << 5 | reg);
  tx_buffer[1] = 0;
  tx_buffer[2] = value;
  
  gpio_put(adc->cs_pin, 0); // Select the ADS124S08
  spi_write_blocking(spi1, tx_buffer, 3);
  gpio_put(adc->cs_pin, 1); // Deselect the ADS124S08
}

// Read a value from a register
uint8_t ads124s08_read_register(ADS124S08* adc,uint8_t reg) {
  uint8_t tx_buffer[3];// = {(ADS124S08_CMD_RREG << 5) | reg, 0x00, 0x00};
  tx_buffer[0] = (ADS124S08_CMD_RREG << 5) | reg;
  tx_buffer[1] = 0;
  tx_buffer[2] = 0;
  uint8_t rx_buffer[3];
    
  gpio_put(adc->cs_pin, 0); // Select the ADS124S08
  spi_write_blocking(spi1, tx_buffer, 2); // Send read register command and address
  spi_read_blocking(spi1, 0x00, rx_buffer, 1); // Read register value
  gpio_put(adc->cs_pin, 1); // Deselect the ADS124S08
  
  return rx_buffer[0];
}

// Read ADC data
float ads124s08_read_data(ADS124S08* adc) {
  //    gpio_put(adc->start_pin, 1); // Start the conversion
  //    while (gpio_get(adc->drdy_pin) == 1); // Wait for conversion to complete
    
  uint8_t tx_buffer[4];// = {ADS124S08_CMD_RDATA, 0x00, 0x00, 0x00}; // RDATA command
    tx_buffer[0] = ADS124S08_CMD_RDATA;
    tx_buffer[1] = 0;
    tx_buffer[2] = 0;
    tx_buffer[3] = 0;
    uint8_t rx_buffer[4];
    
    gpio_put(adc->cs_pin, 0); // Select the ADS124S08
    spi_write_blocking(spi1, tx_buffer, 1); // Send the RDATA command
    spi_read_blocking(spi1, 0x00, rx_buffer, 4); // Read the conversion result
    gpio_put(adc->cs_pin, 1); // Deselect the ADS124S08

    //    printf("TEST %x\n",rx_buffer[0]);
    //    uint32_t result = (rx_buffer[0] << 16) | (rx_buffer[1] << 8) | rx_buffer[2];
    uint32_t result = (rx_buffer[1] << 16) | (rx_buffer[2] << 8) | rx_buffer[3];
    int32_t val = (int32_t) result;
    //    printf("TESTINT %d\n",val);
    float fval = adc_to_V * (float)val;
    
    return fval;
}


uint8_t ads124s08_readDeviceID(ADS124S08* adc){
  return ads124s08_read_register(adc,ADS124S08_REG_ID);
}

uint8_t ads124s08_readStatus(ADS124S08* adc){
  return ads124s08_read_register(adc,ADS124S08_REG_STATUS);
}


uint8_t ads124s08_readInternalRef(ADS124S08* adc){
  return ads124s08_read_register(adc,ADS124S08_REG_REF);
}


void ads124s08_command(ADS124S08* adc, uint8_t command ){


  uint8_t tx_buffer[2];
  //  tx_buffer[0] = ADS124S08_CMD_NOP;
  tx_buffer[0] = command;
  
  gpio_put(adc->cs_pin, 0); // Select the ADS124S08
  spi_write_blocking(spi1, tx_buffer, 1);
  gpio_put(adc->cs_pin, 1); // Deselect the ADS124S08
}




