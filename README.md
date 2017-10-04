# BspDrivers

BspDrivers es una librer�a orientada a MBED, que proporciona una serie de controladores relativos a perif�ricos
hardware que suelen encontrarse en cualquier BSP tales como: leds, pulsadores, rel�s, etc...


  
## Changelog

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
