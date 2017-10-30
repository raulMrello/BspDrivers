/*
 * DMA.h
 *
 *  Created on: Oct 2017
 *      Author: raulMrello
 *
 *  DMA es el m�dulo C++ que proporciona acceso a los diferentes canales DMA. Dependiendo de la plataforma
 *  existir�n m�s o menos canales
 *  el archivo .cpp
 */
 
 
#ifndef DMA_H
#define DMA_H

#if defined(TARGET_STM32L4) 
#include "stm32l4xx_hal.h"
#else
#error Realizar implementaci�n para la plataforma en cuesti�n
#endif
  



//------------------------------------------------------------------------------------
//- STATIC CLASS DMA -----------------------------------------------------------------
//------------------------------------------------------------------------------------


class DMA{
  public:
      
    /** Manejadores DMA para perif�ricos TIMx */
    static TIM_HandleTypeDef*  tim1_ch1;
    static TIM_HandleTypeDef*  tim1_ch2;
    static TIM_HandleTypeDef*  tim1_ch3;
    static TIM_HandleTypeDef*  tim1_ch4;
    static TIM_HandleTypeDef*  tim16_ch1;
  
    /** Manejadores DMA para perif�ricos SPIx */
    static SPI_HandleTypeDef*  spi1;    
    static SPI_HandleTypeDef*  spi3;
 
    /** Manejadores DMA para perif�ricos I2Cx */
    static I2C_HandleTypeDef*  i2c1;    
    static I2C_HandleTypeDef*  i2c3;        
 
    /** Manejadores DMA para perif�ricos USARTx */
    static USART_HandleTypeDef*  usart1;    
    static USART_HandleTypeDef*  usart2;        
    static USART_HandleTypeDef*  usart3;        
 
    /** Manejadores DMA para perif�ricos CANx */
    static CAN_HandleTypeDef*  can1;    
 
    /** Manejadores DMA para perif�ricos ADCx */
    static ADC_HandleTypeDef*  adc1;    
 
    /** Manejadores DMA para perif�ricos DACx */
    static DAC_HandleTypeDef*  dac1;    
    static DAC_HandleTypeDef*  dac2;        
    static DAC_HandleTypeDef*  dac3;        

    /** Manejadores DMA para perif�ricos SAIx */
    static SAI_HandleTypeDef*  sai1;    
    static SAI_HandleTypeDef*  sai2;    

   
};


//------------------------------------------------------------------------------------
//- DMA IRQ HANDLERS -----------------------------------------------------------------
//------------------------------------------------------------------------------------


/** Manejadores de interrupci�n para los canales DMA asociados a los interfaces SPI (SPI1 y SPI3)*/

#ifdef __cplusplus
extern "C" {
#endif

void DMA1_Channel1_IRQHandler(void);
void DMA1_Channel2_IRQHandler(void);
void DMA1_Channel3_IRQHandler(void);
void DMA1_Channel4_IRQHandler(void);
void DMA1_Channel5_IRQHandler(void);
void DMA1_Channel6_IRQHandler(void);
void DMA1_Channel7_IRQHandler(void);

void DMA2_Channel1_IRQHandler(void);
void DMA2_Channel2_IRQHandler(void);
void DMA2_Channel3_IRQHandler(void);
void DMA2_Channel4_IRQHandler(void);
void DMA2_Channel5_IRQHandler(void);
void DMA2_Channel6_IRQHandler(void);
void DMA2_Channel7_IRQHandler(void);
    
#ifdef __cplusplus
}
#endif


#endif   /* DMA_H */
