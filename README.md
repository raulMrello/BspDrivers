# BspDrivers

BspDrivers es una librer�a orientada a MBED, que proporciona una serie de controladores relativos a perif�ricos
hardware que suelen encontrarse en cualquier BSP tales como: leds, pulsadores, rel�s, etc...


  
## Changelog

----------------------------------------------------------------------------------------------
##### 31.10.2017 ->commit:"Incluyo driver PCA9685 servo 16-ch"
- [x] Incluyo el driver para el servo-driver de 16-ch aunque falta verificar funcionamiento.
	  El m�dulo se ha verificado en la tarjeta NUCLEO_L432KC

----------------------------------------------------------------------------------------------
##### 31.10.2017 ->commit:"Incluyo driver HCSR04 sensor ultrasonidos"
- [x] Incluyo el driver para el sensor de ultrasonidos, aunque falta probar su funcionamiento.
	  El m�dulo se ha verificado en la tarjeta NUCLEO_L432KC

----------------------------------------------------------------------------------------------
##### 30.10.2017 ->commit:"Incluyo driver WS281xLedStrip con funcionalidad m�nima"
- [x] Incluyo el driver para la tira de leds direccionables.
- [x] Corrijo error en driver DMA_PwmOut, elimiando el RepetitionCounter del TIMx
	  El m�dulo se ha verificado en la tarjeta NUCLEO_L432KC

----------------------------------------------------------------------------------------------
##### 30.10.2017 ->commit:"Incluyo drivers DMA"
- [x] Incluyo el directorio DMA para a�adir diversos drivers que hacen uso de capacidades DMA.
	  Por el momento a�ado los drivers DMA_SPI y DMA_PwmOut.
	  El m�dulo se ha verificado en la tarjeta NUCLEO_L432KC

----------------------------------------------------------------------------------------------
##### 26.10.2017 ->commit:"Incluyo SpiDmaInterface"
- [x] Incluyo este m�dulo que permite utilizar un canal SPI mediante transferencias DMA. Es capaz
	  de enviar, recibir y enviar&recibir. Para lo que se deben instalar las callbacks correspondientes
	  para las interrupciones dma_half_transfer, dma_complete_transfer, dma_error_transfer y 
	  dma_abort_transfer.
	  El m�dulo se ha verificado en la tarjeta NUCLEO_L432KC

----------------------------------------------------------------------------------------------
##### 04.10.2017 ->commit:"Modifico Led"
- [x] Simplifico funciones p�blica Led m�s intuitivas

----------------------------------------------------------------------------------------------
##### 26.09.2017 ->commit:"A�ado SpiFlashBlockDevice"
- [x] A�ado el driver para controlar una spi flash NOR

----------------------------------------------------------------------------------------------
##### 25.09.2017 ->commit:"A�ado Shifter, Stepper y PirDetector"
- [x] A�ado nuevos drivers

----------------------------------------------------------------------------------------------
##### 25.09.2017 ->commit:"Creo repositorio"
- [x] Creo repositorio con los siguientes drivers:
		- Led: opci�n de salida digital o pwm, con capacidad de dimerizaci�n y parpadeo integrados.
		- Pushbutton: generaci�n de eventos press, relase, hold
		- Relay: con opci�n de salida a simple o doble nivel
		- SerialTerminal: con opci�n de manejador dedicado o por eoc
		- Zerocross: con opci�n de selecci�n del flanco activo
