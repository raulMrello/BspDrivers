/*
 * DMA_SPI.h
 *
 *  Created on: Oct 2017
 *      Author: raulMrello
 *
 *  DMA_SPI es un m�dulo C++ que proporciona un mecanismo de transmisi�n/recepci�n SPI utilizando los
 *  canales DMA asociados para enviar y/o recibir datos desde un buffer de memoria.
 *  Se pueden realizar operaciones de env�o, de recepci�n o de env�o+recepci�n.
 *  La notificaci�n de eventos se delega a callbacks dedicadas: dmaHalfIsrCb, dmaCpltIsrCb y dmaErrIsrCb que
 *  deber�n ser instaladas al inicio del proceso.
 *
 *  NOTA: Esta librer�a es compatible con procesadores STM32L4xx. Nota la velocidad del bus SPI es un m�ltiplo
 *  de fpclk/br siendo br (2,4,8,...,256) y fpclk (SPI1: PCLK2 (80MHz), SPI3: PCLK1)
 */
 
 
#ifndef SPIDMAINTERFACE_H
#define SPIDMAINTERFACE_H

 
#include "mbed.h"
#include "spi_api.h"
//#include "stm32l4xx_hal_def.h" 
//#include "stm32l4xx_hal_dma.h"    
#include "DMA.h"



//------------------------------------------------------------------------------------
//- CLASS DMA_SPI ------------------------------------------------------------
//------------------------------------------------------------------------------------


class DMA_SPI : public SPI, public DMA {
  public:

    enum ErrorResult{
        NO_ERRORS = HAL_OK,
        UNKNOWN_ERROR = HAL_ERROR,
        BUSY_ERROR = HAL_BUSY,
        TIMEOUT_ERROR = HAL_TIMEOUT,
        TRANSFER_ERROR,        
        ABORT_ERROR,
    };
	
    /** @fn DMA_SPI()
     *  @brief Constructor, que asocia un manejador SPI (SPI_x)
     *  @param hz Velocidad bus spi
     *  @param mosi SPI Master Out, Slave In pin
     *  @param miso SPI Master In, Slave Out pin
     *  @param sclk SPI Clock pin
     *  @param ssel SPI chip select pin
     */
    DMA_SPI(int hz, PinName mosi, PinName miso, PinName sclk, PinName ssel=NC);

	
    /** @fn ~DMA_SPI()
     *  @brief Destructor por defecto
     */
    virtual ~DMA_SPI(){}

	
    /** @fn transmit()
     *  @brief Env�a un buffer por medio de DMA
     *  @param txbuf Datos de origen
     *  @param bufsize Tama�o de los datos a enviar
     *  @param dmaHalfIsrCb Callback para recibir eventos halfDma
     *  @param dmaCpltIsrCb Callback para recibir eventos CpltDma
     *  @param dmaErrIsrCb Callback para recibir eventos ErrorDma
     */
    void transmit(uint8_t* txbuf, uint16_t bufsize, Callback<void()>& dmaHalfIsrCb, 
                        Callback<void()>& dmaCpltIsrCb, Callback<void(ErrorResult)>& dmaErrIsrCb);

	
    /** @fn receive()
     *  @brief Recibe un buffer por medio de DMA
     *  @param rxbuf Datos de destino
     *  @param bufsize Tama�o de los datos a enviar
     *  @param dmaHalfIsrCb Callback para recibir eventos halfDma
     *  @param dmaCpltIsrCb Callback para recibir eventos CpltDma
     *  @param dmaErrIsrCb Callback para recibir eventos ErrorDma
     */
    void receive(uint8_t* rxbuf, uint16_t bufsize, Callback<void()>& dmaHalfIsrCb, 
                        Callback<void()>& dmaCpltIsrCb, Callback<void(ErrorResult)>& dmaErrIsrCb);

	
    /** @fn transmitAndReceive()
     *  @brief Env�a y recibe un buffer por medio de DMA
     *  @param txbuf Datos de origen para la escritura
     *  @param rxbuf Datos de destino tras la lectura
     *  @param bufsize Tama�o de los datos a enviar
     *  @param dmaHalfIsrCb Callback para recibir eventos halfDma
     *  @param dmaCpltIsrCb Callback para recibir eventos CpltDma
     *  @param dmaErrIsrCb Callback para recibir eventos ErrorDma
     */
    void transmitAndReceive(uint8_t* txbuf, uint8_t* rxbuf, uint16_t bufsize, 
                        Callback<void()>& dmaHalfIsrCb, Callback<void()>& dmaCpltIsrCb, Callback<void(ErrorResult)>& dmaErrIsrCb);

	
    /** @fn getHandler()
     *  @brief Obtiene la referencia al manejador SPI
     *  @return Manejador spi
     */
    SPI_HandleTypeDef* getHandler(){ return _handle; }

    /** Callbacks de notificaci�n de interrupci�n dma */
    Callback<void()> dmaHalfIsrCb;
    Callback<void()> dmaCpltIsrCb;
    Callback<void(ErrorResult)> dmaErrIsrCb;  
    
  protected:        
    SPI_HandleTypeDef* _handle;
    DMA_HandleTypeDef _hdma_tx;
    DMA_HandleTypeDef _hdma_rx;
  
};    



#endif   /* DMA_SPI_H */
