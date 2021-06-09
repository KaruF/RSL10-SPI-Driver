/*
 * OFB_SPI.c
 *
 *  Created on: 9 Nis 2021
 *      Author: m.omerfarukbulut@gmail.com
 *
 *      for now, this abstraction layer work just only with CM3 controller,master and full dublex communication...
 */


#include "app.h"




/* ----------------------------------------------------------------------------
 * Function      : void SPI0_ERROR_IRQHandler()
 * ----------------------------------------------------------------------------
 * Description   : - SPI0 Error Handler
 * Inputs        : - None           - None (void)
 * Outputs       : - Return value   - None (void)
 * Assumptions   :
 * ------------------------------------------------------------------------- */
void SPI0_ERROR_IRQHandler() {

    while (1)
        ;  //Watchdog will reset processor

}

/* ----------------------------------------------------------------------------
 * Function      : void SPI1_ERROR_IRQHandler()
 * ----------------------------------------------------------------------------
 * Description   : - SPI1 Error Handler
 * Inputs        : - None           - None (void)
 * Outputs       : - Return value   - None (void)
 * Assumptions   :
 * ------------------------------------------------------------------------- */
void SPI1_ERROR_IRQHandler() {

    while (1)
        ;  //Watchdog will reset processor

}

/* ----------------------------------------------------------------------------
 * Function      : void SPI0_RX_IRQHandler()
 * ----------------------------------------------------------------------------
 * Description   : - SPI0 Receive Data Handler
 * Inputs        : - None           - None (void)
 * Outputs       : - Return value   - None (void)
 * Assumptions   :
 * ------------------------------------------------------------------------- */
void SPI0_RX_IRQHandler() {

    SPI_Receive_Flag = RECEIVE_DONE;

}

/* ----------------------------------------------------------------------------
 * Function      : void SPI0_TX_IRQHandler()
 * ----------------------------------------------------------------------------
 * Description   : - SPI0 Transmit Data Handler
 * Inputs        : - None           - None (void)
 * Outputs       : - Return value   - None (void)
 * Assumptions   :
 * ------------------------------------------------------------------------- */
void SPI0_TX_IRQHandler() {



    SPI_Transmit_Flag = TRANSMIT_DONE;


 }

/* ----------------------------------------------------------------------------
 * Function      : void SPI1_RX_IRQHandler()
 * ----------------------------------------------------------------------------
 * Description   : - SPI1 Receive Data Handler
 * Inputs        : - None           - None (void)
 * Outputs       : - Return value   - None (void)
 * Assumptions   :
 * ------------------------------------------------------------------------- */
void SPI1_RX_IRQHandler() {

    SPI_Receive_Flag = RECEIVE_DONE;

}

/* ----------------------------------------------------------------------------
 * Function      : void SPI1_TX_IRQHandler()
 * ----------------------------------------------------------------------------
 * Description   : - SPI1 Transmit Data Handler
 * Inputs        : - None           - None (void)
 * Outputs       : - Return value   - None (void)
 * Assumptions   :
 * ------------------------------------------------------------------------- */
void SPI1_TX_IRQHandler() {

    SPI_Transmit_Flag = TRANSMIT_DONE;

}

/* ----------------------------------------------------------------------------
 * Function      : void OFB_SPI_Setup(SPI_Selections SPI_cfg)
 * ----------------------------------------------------------------------------
 * Description   : - SPI configuration function
 * Inputs        : - SPI_cfg        - SPI Configuration Structure member
 * Outputs       : - Return value   - None (void)
 * Assumptions   :
 * ------------------------------------------------------------------------- */
void OFB_SPI_Setup(SPI_Selections SPI_cfg) {

    uint32_t config = SPI_cfg.Cfg_CTRL0.SPI_Controller | SPI_cfg.Cfg_CTRL0.SPI_Overrun_Int | SPI_cfg.Cfg_CTRL0.SPI_Prescale | SPI_cfg.Cfg_CTRL0.SPI_Slave | SPI_cfg.Cfg_CTRL0.SPI_Underrun_Int | SPI_cfg.Cfg_CTRL0.SPI_Enable;


    Sys_SPI_Config(SPI_cfg.number_SPI, config);
    Sys_SPI_DIOConfig(SPI_cfg.number_SPI, SPI_cfg.Cfg_CTRL0.SPI_Slave, DIO_NO_PULL | DIO_LPF_DISABLE, SPI_cfg.SPI_DIOs.clk, SPI_cfg.SPI_DIOs.cs, SPI_cfg.SPI_DIOs.miso, SPI_cfg.SPI_DIOs.mosi);



    if (SPI_cfg.Cfg_CTRL0.SPI_Controller == SPI0_CONTROLLER_DMA) {

        //do dma config with DMA_CFG variable in header file

    }


    if (SPI_cfg.Cfg_CTRL0.SPI_Overrun_Int || SPI_cfg.Cfg_CTRL0.SPI_Underrun_Int) {

        if (SPI_cfg.number_SPI == 0)
            NVIC_EnableIRQ(SPI0_ERROR_IRQn);
        else
            NVIC_EnableIRQ(SPI1_ERROR_IRQn);
    }


    if ((SPI_cfg.SPI_DATA_Int_ENABLE == 1) && (SPI_cfg.Cfg_CTRL1.SPI_RW_CMD == SPI0_READ_DATA)) {

        if (SPI_cfg.number_SPI == 0)
            NVIC_EnableIRQ(SPI0_RX_IRQn);
        else
            NVIC_EnableIRQ(SPI1_RX_IRQn);

    } else if ((SPI_cfg.SPI_DATA_Int_ENABLE == 1) && (SPI_cfg.Cfg_CTRL1.SPI_RW_CMD == SPI0_WRITE_DATA)) {

        if (SPI_cfg.number_SPI == 0)

            NVIC_EnableIRQ(SPI0_TX_IRQn);
        else
            NVIC_EnableIRQ(SPI1_TX_IRQn);

    } else {

        if (SPI_cfg.number_SPI == 0) {
            NVIC_EnableIRQ(SPI0_TX_IRQn);
            NVIC_EnableIRQ(SPI0_RX_IRQn);
        } else {
            NVIC_EnableIRQ(SPI1_TX_IRQn);
            NVIC_EnableIRQ(SPI1_RX_IRQn);
        }


    }



}


/* ----------------------------------------------------------------------------
 * Function      : void OFB_SPI_FullDuplex_INT()
 * ----------------------------------------------------------------------------
 * Description   : - That function is full duplex SPI transfer function
 * Inputs        : - SPI_cfg        - SPI Configuration Structure member
 *                 - RXdata_point   - Pointer to receive data
 *                 - TXdata_point   - Pointer to transmit data
 * Outputs       : - Return value   - None (void)
 * Assumptions   :
 * ------------------------------------------------------------------------- */
void OFB_SPI_FullDuplex_INT(SPI_Selections SPI_cfg, uint32_t *RXdata_point, uint32_t *TXdata_point) {

    SPI0->TX_DATA = *TXdata_point;

    ((SPI_Type *) (SPI0_BASE + SPI_cfg.number_SPI * SPI_BASE_DIFF))->CTRL1 = SPI_cfg.Cfg_CTRL1.SPI_CS | SPI_cfg.Cfg_CTRL1.SPI_RW_CMD | SPI_cfg.Cfg_CTRL1.SPI_Word_Size | SPI0_START;

    while (!SPI_Transmit_Flag)
        ;
    SPI_Transmit_Flag = TRANSMIT_IDLE;

    while (!SPI_Receive_Flag)
        ;
    *RXdata_point = SPI0->RX_DATA;
    SPI_Receive_Flag = RECEIVE_IDLE;

}

/* ----------------------------------------------------------------------------
 * Function      : void OFB_Disable_SPI()
 * ----------------------------------------------------------------------------
 * Description   :
 * Inputs        : - num            - Number of SPI Peripheral
 * Outputs       : - Return value   - None (void)
 * Assumptions   :
 * ------------------------------------------------------------------------- */

void OFB_Disable_SPI(uint8_t num) {

    ((SPI_Type *) (SPI0_BASE + num * SPI_BASE_DIFF))->CTRL0 = SPI0_DISABLE;

}

/* ----------------------------------------------------------------------------
 * Function      : void OFB_SPI_CS_High(uint8_t num)
 * ----------------------------------------------------------------------------
 * Description   :
 * Inputs        : - num            - Number of SPI Peripheral
 * Outputs       : - Return value   - None (void)
 * Assumptions   :
 * ------------------------------------------------------------------------- */

void OFB_SPI_CS_High(uint8_t num) {

    ((SPI_Type *) (SPI0_BASE + num * SPI_BASE_DIFF))->CTRL1 = SPI0_CS_1;


}

/* ----------------------------------------------------------------------------
 * Function      : void OFB_SPI_CS_Low(uint8_t num)
 * ----------------------------------------------------------------------------
 * Description   :
 * Inputs        : - num            - Number of SPI Peripheral
 * Outputs       : - Return value   - None (void)
 * Assumptions   :
 * ------------------------------------------------------------------------- */

void OFB_SPI_CS_Low(uint8_t num) {

    ((SPI_Type *) (SPI0_BASE + num * SPI_BASE_DIFF))->CTRL1 = SPI0_CS_0;

}
