#include "stm32f4xx_hal.h"
#include "NRF24L01.h"

extern SPI_HandleTypeDef hspi2;

#define NRF24_SPI &hspi1

#define NRF24_CSN_PORT GPIOB
#define NRF24_CSN_PIN GPIO_PIN_12

#define NRF24_CE_PORT GPIOB
#define NRF24_CE_PIN GPIO_PIN_13

void CS_Select(void) {
    HAL_GPIO_WritePin(NRF24_CSN_PORT, NRF24_CSN_PIN, GPIO_PIN_RESET); // Unselect to activate (Active-Low)
}

void CS_UnSelect(void) {
    HAL_GPIO_WritePin(NRF24_CSN_PORT, NRF24_CSN_PIN, GPIO_PIN_SET);  // Select to deactivate (Active-Low)
}

void CE_Enable(void) {
    HAL_GPIO_WritePin(NRF24_CE_PORT, NRF24_CE_PIN, GPIO_PIN_SET);  
}

void CE_Disable(void) { 
    HAL_GPIO_WritePin(NRF24_CE_PORT, NRF24_CE_PIN, GPIO_PIN_RESET); 
}

// Write a single byte to the particular register:
void nrf24_WriteReg(uint8_t Reg, uint8_t Data) {
    uint8_t buff[2];
    buff[0] = Reg | 1 << 5;
    buff[1] = Data;

    CS_Select(); // Pull the CS Pin LOW to select the device

    // Parameters: 
    HAL_SPI_Transmit(NRF24_SPI, buff, 2, 1000);

    CS_UnSelect(); // Pull the CS HIGH to select the device
}

// Write multiple bytes starting from a particular register:
void nrf24_WriteRegMulti(uint8_t Reg, uint8_t* data, int size) {
    uint8_t buff[2];
    buff[0] = Reg | 1 << 5;
    buff[1] = Data;

    CS_Select(); // Pull the CS Pin LOW to select the device

    HAL_SPI_Transmit(NRF24_SPI, buff, 1, 100);
    HAL_SPI_Transmit(NRF24_SPI, data, size, 1000);

    CS_UnSelect(); // Pull the CS HIGH to select the device
}

uint8_t nrf24_ReadReg(uint8_t Reg) {
    uint8_t data=0;

    CS_Select(); // Pull the CS Pin LOW to select the device

    HAL_SPI_Transmit(NRF24_SPI, &Reg, 1, 100);
    HAL_SPI_Transmit(NRF24_SPI, &data, 1, 100);

    CS_UnSelect(); // Pull the CS HIGH to select the device

    return data;
}

void nrf24_ReadReg_Multi(uint8_t Reg, uint8_t* data, int size) { 
    CS_Select(); // Pull the CS Pin LOW to select the device

    HAL_SPI_Transmit(NRF24_SPI, &Reg, 1, 100);
    HAL_SPI_Transmit(NRF24_SPI, &data, 1, 100);

    CS_UnSelect(); // Pull the CS HIGH to select the device
}

void nrfsendCmd(uint8_t cmd) {
    CS_Select(); // Pull the CS Pin LOW to select the device

    HAL_SPI_Transmit(NRF24_SPI, &cmd, 1, 100);

    CS_UnSelect(); // Pull the CS HIGH to select the device
}

void NRF24_Init(void) {
    // Disable the chip before configuring the device
    CE_Disable();
    CE_Enable();

    nrf24_WriteReg(CONFIG, 0); // Will be configured later

    nrf24_WriteReg(EN_AA, 0); // No Auto ACK (Acknowledgement)

    nrf24_WriteReg(EN_RXADDR, 0); // Not Enabling any data pipe right now

    nrf24_WriteReg(SETUP_AW, 0); // 5 Bytes for the Tx / Rx Address

    nrf24_WriteReg(SETUP_RETR, 0); // No Re-Transmission

    nrf24_WriteReg(RF_CH, 0); // Will be setup during Tx or Rx

    nrf24_WriteReg(RF_SETUP, 0); // Power = 0db, data rate = 2 Mbps
}
