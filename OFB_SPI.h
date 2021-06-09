/*
 * OFB_SPI.h
 *
 *  Created on: 9 Nis 2021
 *      Author: m.omerfarukbulut@gmail.com
 */

#ifndef INCLUDE_OFB_SPI_H_
#define INCLUDE_OFB_SPI_H_

//macro functions and variables

//#define concat_Int(x)       SPI##x##_ERROR_IRQn

#define SPI_Type                        SPI0_Type

//For polling operations
#define SPI_TRANSMIT_AND_VALUE          0x08
#define SPI_STATUS_TRANSMIT_SHIFT       3
#define SPI_TRANSMIT_CLEAR              SPI0_TRANSMIT_CLEAR_BITBAND
/////

//DMA configurations for SPI
#define DMA_CFG                         0

//variables

volatile uint8_t SPI_Transmit_Flag;
volatile uint8_t SPI_Receive_Flag;

////*** enum and structers*////


enum SPI_NUM {

    SPI_0, SPI_1,

};

typedef struct SPI_DIO {

    uint8_t clk;
    uint8_t miso;
    uint8_t mosi;
    uint8_t cs;
} SPI_dio;

/*
typedef struct SPI_Num {

    uint8_t SPI_Num;

 } SPIx;*/

typedef struct SPIx_CTRL0 {
    uint16_t SPI_Overrun_Int;
    uint16_t SPI_Underrun_Int;
    uint8_t SPI_Controller;
    uint8_t SPI_Slave;
    uint8_t SPI_Clk_Pol;
    uint8_t SPI_Mode_Select;
    uint8_t SPI_Enable;
    uint8_t SPI_Prescale;
} SPIx_ctrl0;

typedef struct SPIx_CTRL1 {
    uint8_t SPI_Start_Busy;
    uint8_t SPI_Busy_Status;
    uint8_t SPI_RW_CMD;
    uint8_t SPI_CS;
    uint8_t SPI_Word_Size;
} SPIx_ctrl1;

typedef struct SPI_Prop {

    uint8_t number_SPI;
    SPIx_ctrl0 Cfg_CTRL0;
    SPIx_ctrl1 Cfg_CTRL1;
    uint8_t SPI_DATA_Int_ENABLE; //SPI0_READ_DATA for RX, SPI0_WRITE_DATA for TX, otherwise for RX and TX
    SPI_dio SPI_DIOs;
} SPI_Selections;


enum SPI_TRANSRECEIVER_FLAGS {

    TRANSMIT_DONE = 1, RECEIVE_DONE = 1, TRANSMIT_IDLE = 0, RECEIVE_IDLE = 0,

};



///declarations of functions////

void OFB_SPI_Setup(SPI_Selections SPI_cfg);
void OFB_SPI_FullDuplex_INT(SPI_Selections SPI_cfg, uint32_t *RXdata_point, uint32_t *TXdata_point);
void OFB_Disable_SPI(uint8_t num);
void OFB_SPI_CS_High(uint8_t num);
void OFB_SPI_CS_Low(uint8_t num);

void SPI0_RX_IRQHandler();
void SPI0_TX_IRQHandler();
void SPI1_RX_IRQHandler();
void SPI1_TX_IRQHandler();

#endif /* INCLUDE_OFB_SPI_H_ */
