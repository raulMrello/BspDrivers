# BspDrivers

BspDrivers es una librería orientada a MBED, que proporciona una serie de controladores relativos a periféricos
hardware que suelen encontrarse en cualquier BSP tales como: leds, pulsadores, relés, etc...


  
## Changelog

----------------------------------------------------------------------------------------------
##### 25.09.2017 ->commit:"Creo repositorio"
- [x] Creo repositorio con los siguientes drivers:
		- Led: opción de salida digital o pwm, con capacidad de dimerización y parpadeo integrados.
		- Pushbutton: generación de eventos press, relase, hold
		- Relay: con opción de salida a simple o doble nivel
		- SerialTerminal: con opción de manejador dedicado o por eoc
		- Zerocross: con opción de selección del flanco activo


----------------------------------------------------------------------------------------------
##### 22.09.2017 ->commit:"Añado gestión de topics por nombre"
- [x] Añado funciones para gestión de topics por nombre.
- [ ] Falta implementar chequeo de verificación de nombre con wildcards especiales como # y +
      a la hora de obtener el topic_id durante el proceso de suscripción.

----------------------------------------------------------------------------------------------
##### 21.09.2017 ->commit:"Modifico callbacks para que utilicen topics uin32_t"
- [x] Cambio callbacks con parámetros uint16_t a a uint32_t
- [ ] 
----------------------------------------------------------------------------------------------
##### 19.09.2017 ->commit:"Incluyo módulo FuncPtr para CMSIS_OS"
- [x] Incluyo módulo FuncPtr para las callbacks
- [ ] 
----------------------------------------------------------------------------------------------
##### 18.09.2017 ->commit:"Librería compatible con la API de MBED 5.x y con CMSIS_OS RTOSv1"
- [x] Cambio código válido para cmsis o mbed5.
- [ ] Incluir parámetros de configuración (thread, mutex, etc) en función del entorno de desarrollo utilizado:
		> Por ejemplo para mbed-os < 5x, cmsis-os, cmsis-os2,...

----------------------------------------------------------------------------------------------
##### 18.09.2017 ->commit:"Compatibilizo con cmsis y mbed5"
- [x] Cambio código válido para cmsis o mbed2.
- [ ] Incluir parámetros de configuración (thread, mutex, etc) en función del entorno de desarrollo utilizado:
		> Por ejemplo para mbed-os < 5x, cmsis-os, cmsis-os2,...

----------------------------------------------------------------------------------------------
##### 17.09.2017 ->commit:"Versión 1.0.0-build-17-sep-2017"
- [x] Funciones básicas creadas para su funcionamiento con mbed-os 5.x
- [ ] Incluir parámetros de configuración (thread, mutex, etc) en función del entorno de desarrollo utilizado:
		> Por ejemplo para mbed-os < 5x, cmsis-os, cmsis-os2,...

