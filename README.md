# BspDrivers

BspDrivers es una librer�a orientada a MBED, que proporciona una serie de controladores relativos a perif�ricos
hardware que suelen encontrarse en cualquier BSP tales como: leds, pulsadores, rel�s, etc...


  
## Changelog

----------------------------------------------------------------------------------------------
##### 25.09.2017 ->commit:"Creo repositorio"
- [x] Creo repositorio con los siguientes drivers:
		- Led: opci�n de salida digital o pwm, con capacidad de dimerizaci�n y parpadeo integrados.
		- Pushbutton: generaci�n de eventos press, relase, hold
		- Relay: con opci�n de salida a simple o doble nivel
		- SerialTerminal: con opci�n de manejador dedicado o por eoc
		- Zerocross: con opci�n de selecci�n del flanco activo


----------------------------------------------------------------------------------------------
##### 22.09.2017 ->commit:"A�ado gesti�n de topics por nombre"
- [x] A�ado funciones para gesti�n de topics por nombre.
- [ ] Falta implementar chequeo de verificaci�n de nombre con wildcards especiales como # y +
      a la hora de obtener el topic_id durante el proceso de suscripci�n.

----------------------------------------------------------------------------------------------
##### 21.09.2017 ->commit:"Modifico callbacks para que utilicen topics uin32_t"
- [x] Cambio callbacks con par�metros uint16_t a a uint32_t
- [ ] 
----------------------------------------------------------------------------------------------
##### 19.09.2017 ->commit:"Incluyo m�dulo FuncPtr para CMSIS_OS"
- [x] Incluyo m�dulo FuncPtr para las callbacks
- [ ] 
----------------------------------------------------------------------------------------------
##### 18.09.2017 ->commit:"Librer�a compatible con la API de MBED 5.x y con CMSIS_OS RTOSv1"
- [x] Cambio c�digo v�lido para cmsis o mbed5.
- [ ] Incluir par�metros de configuraci�n (thread, mutex, etc) en funci�n del entorno de desarrollo utilizado:
		> Por ejemplo para mbed-os < 5x, cmsis-os, cmsis-os2,...

----------------------------------------------------------------------------------------------
##### 18.09.2017 ->commit:"Compatibilizo con cmsis y mbed5"
- [x] Cambio c�digo v�lido para cmsis o mbed2.
- [ ] Incluir par�metros de configuraci�n (thread, mutex, etc) en funci�n del entorno de desarrollo utilizado:
		> Por ejemplo para mbed-os < 5x, cmsis-os, cmsis-os2,...

----------------------------------------------------------------------------------------------
##### 17.09.2017 ->commit:"Versi�n 1.0.0-build-17-sep-2017"
- [x] Funciones b�sicas creadas para su funcionamiento con mbed-os 5.x
- [ ] Incluir par�metros de configuraci�n (thread, mutex, etc) en funci�n del entorno de desarrollo utilizado:
		> Por ejemplo para mbed-os < 5x, cmsis-os, cmsis-os2,...

