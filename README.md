# BspDrivers

BspDrivers es una librería orientada a MBED, que proporciona una serie de controladores relativos a periféricos
hardware que suelen encontrarse en cualquier BSP tales como: leds, pulsadores, relés, etc...


  
## Changelog

----------------------------------------------------------------------------------------------
##### 04.10.2017 ->commit:"Modifico Led"
- [x] Simplifico funciones pública Led más intuitivas

----------------------------------------------------------------------------------------------
##### 26.09.2017 ->commit:"Añado SpiFlashBlockDevice"
- [x] Añado el driver para controlar una spi flash NOR

----------------------------------------------------------------------------------------------
##### 25.09.2017 ->commit:"Añado Shifter, Stepper y PirDetector"
- [x] Añado nuevos drivers

----------------------------------------------------------------------------------------------
##### 25.09.2017 ->commit:"Creo repositorio"
- [x] Creo repositorio con los siguientes drivers:
		- Led: opción de salida digital o pwm, con capacidad de dimerización y parpadeo integrados.
		- Pushbutton: generación de eventos press, relase, hold
		- Relay: con opción de salida a simple o doble nivel
		- SerialTerminal: con opción de manejador dedicado o por eoc
		- Zerocross: con opción de selección del flanco activo
