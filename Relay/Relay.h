/*
 * Relay.h
 *
 *  Created on: Sep 2017
 *      Author: raulMrello
 *
 *	Relay es el m�dulo encargado de gestionar la activaci�n y apagado de un Rel�. Es posible generar patrones de encendido
 *  en caso de que se pueda controlar con dos l�neas distintas. De forma que active a m�xima potencia durante un tiempo
 *  y posteriormente desactive una de las l�neas para mantener una corriente de mantenimiento inferior.
 *  En el constructor, se puede configurar el funcionamiento del m�dulo para no tener que hacer nada m�s al encender o apagar
 *  el rel�
 *
 */
 
#ifndef __Relay__H
#define __Relay__H

#if __MBED__ == 1
#include "mbed.h"
#endif

   
class Relay{
  public:
        
    /** Configuraci�n del nivel l�gico de activaci�n del rel� */
	enum RelayLogicLevel{
		OnIsLowLevel,       /// El rel� se activa con una salida a nivel bajo
		OnIsHighLevel       /// El rel� se activa con una salida a nivel alto
	};
    
    /** Tipos de acciones que se pueden solicitar a un rel� */
    enum RelayAction{
        RelayTurnOff,       /// Apagar rel�
        RelayTurnOn,        /// Encender rel�
        ActionCompleted,    /// Acci�n completada (puede ser TurnOff o TurnOn previo)
        NoActions,          /// No hay acciones a realizar
    };
		
    /** Tipo de estado en el que se puede encontrar un rel� */
    enum RelayStat{
        RelayIsOff,         /// Desactivado 
        RelayIsOn,          /// Activado
    };
    
    /** Estructura de datos de prop�sito general para proporcionar informaci�n sobre un rel� */
    struct RelayInfo{
        uint32_t id;        /// Identificador del rel�
        RelayStat stat;     /// Estado actual del rel� (Off,On)
        RelayAction action; /// Acciones solicitadas al rel�
    };
    
	/** Constructor
     *  Durante la construcci�n del objeto, se asignan los pines de control (1 o 2), la l�gica de activaci�n, y
     *  el tiempo que debe proporcionarse la corriente de pico antes de bajar a la corriente de mantenimiento, en 
     *  el caso de utilizar una configuraci�n de doble nivel.
     *  @param gpio_high GPIO conectado al canal de m�xima corriente
     *  @param gpio_low GPIO conectado al canal de corriente de mantenimiento. Si no se utiliza dejar valor
     *          NC por defecto (NC=not connected)
     *  @param level Nivel de activaci�n l�gico 
     *  @param us_high_time Tiempo en microsegundos de activaci�n a m�xima intensidad (en caso de uso de doble nivel)
     */
    Relay(PinName gpio_high, PinName gpio_low = NC, RelayLogicLevel level = OnIsHighLevel, uint32_t us_high_time = DefaultHighCurrentTimeout_us);
    ~Relay();
  
  
	/** turnOn
     *  Inicia el encendido del rel�. 
     */
    void turnOn();
  
  
	/** turnOff
     *  Apaga el rel�
     */
    void turnOff();
  
  
	/** getState
     *  Obtiene el estado actual del rel�
     *  @return RelayStat
     */
    RelayStat getState(){ return _stat; }   
    
             
  protected:
    static const uint32_t DefaultHighCurrentTimeout_us = 100000;    /// Tiempo de corriente de pico por defecto (100ms)
    
    /** Configuraci�n del tipo de control, en funci�n de los pines gpio cargados en el constructor */
    enum RelaySetup{
        RelayIsSingleLevel,     /// Configuraci�n de un �nico nivel (siempre activa con corriente de pico)
        RelayIsDoubleLevel,     /// Configuraci�n a doble nivel (corriente de pico y luego de mantenimiento)
    };
    
    DigitalOut* _out_high;      /// Salida de alta corriente
    DigitalOut* _out_low;       /// Salida de corriente de mantenimiento
    RelayLogicLevel _level;     /// Nivel de activaci�n l�gico
    Ticker _tick;               /// Timer para la duraci�n de la activaci�n con corriente de pico
    RelayStat _stat;            /// Estado de ejecuci�n (Off, On)
    RelaySetup _config;         /// Configuraci�n (single, double)
    uint32_t _us_high_time;     /// Tiempo de corriente de pico


	/** highTimeCb
     *  Callback a invocar cuando finalice el tiempo de corriente de pico y haya que bajara corriente de mantenimiento
     */
    void highTimeCb();

  
};
     


#endif /*__Relay__H */

/**** END OF FILE ****/


