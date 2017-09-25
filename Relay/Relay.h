/*
 * Relay.h
 *
 *  Created on: Sep 2017
 *      Author: raulMrello
 *
 *	Relay es el módulo encargado de gestionar la activación y apagado de un Relé. Es posible generar patrones de encendido
 *  en caso de que se pueda controlar con dos líneas distintas. De forma que active a máxima potencia durante un tiempo
 *  y posteriormente desactive una de las líneas para mantener una corriente de mantenimiento inferior.
 *  En el constructor, se puede configurar el funcionamiento del módulo para no tener que hacer nada más al encender o apagar
 *  el relé
 *
 */
 
#ifndef __Relay__H
#define __Relay__H

#if __MBED__ == 1
#include "mbed.h"
#endif

   
class Relay{
  public:
        
    /** Configuración del nivel lógico de activación del relé */
	enum RelayLogicLevel{
		OnIsLowLevel,       /// El relé se activa con una salida a nivel bajo
		OnIsHighLevel       /// El relé se activa con una salida a nivel alto
	};
    
    /** Tipos de acciones que se pueden solicitar a un relé */
    enum RelayAction{
        RelayTurnOff,       /// Apagar relé
        RelayTurnOn,        /// Encender relé
        ActionCompleted,    /// Acción completada (puede ser TurnOff o TurnOn previo)
        NoActions,          /// No hay acciones a realizar
    };
		
    /** Tipo de estado en el que se puede encontrar un relé */
    enum RelayStat{
        RelayIsOff,         /// Desactivado 
        RelayIsOn,          /// Activado
    };
    
    /** Estructura de datos de propósito general para proporcionar información sobre un relé */
    struct RelayInfo{
        uint32_t id;        /// Identificador del relé
        RelayStat stat;     /// Estado actual del relé (Off,On)
        RelayAction action; /// Acciones solicitadas al relé
    };
    
	/** Constructor
     *  Durante la construcción del objeto, se asignan los pines de control (1 o 2), la lógica de activación, y
     *  el tiempo que debe proporcionarse la corriente de pico antes de bajar a la corriente de mantenimiento, en 
     *  el caso de utilizar una configuración de doble nivel.
     *  @param gpio_high GPIO conectado al canal de máxima corriente
     *  @param gpio_low GPIO conectado al canal de corriente de mantenimiento. Si no se utiliza dejar valor
     *          NC por defecto (NC=not connected)
     *  @param level Nivel de activación lógico 
     *  @param us_high_time Tiempo en microsegundos de activación a máxima intensidad (en caso de uso de doble nivel)
     */
    Relay(PinName gpio_high, PinName gpio_low = NC, RelayLogicLevel level = OnIsHighLevel, uint32_t us_high_time = DefaultHighCurrentTimeout_us);
    ~Relay();
  
  
	/** turnOn
     *  Inicia el encendido del relé. 
     */
    void turnOn();
  
  
	/** turnOff
     *  Apaga el relé
     */
    void turnOff();
  
  
	/** getState
     *  Obtiene el estado actual del relé
     *  @return RelayStat
     */
    RelayStat getState(){ return _stat; }   
    
             
  protected:
    static const uint32_t DefaultHighCurrentTimeout_us = 100000;    /// Tiempo de corriente de pico por defecto (100ms)
    
    /** Configuración del tipo de control, en función de los pines gpio cargados en el constructor */
    enum RelaySetup{
        RelayIsSingleLevel,     /// Configuración de un único nivel (siempre activa con corriente de pico)
        RelayIsDoubleLevel,     /// Configuración a doble nivel (corriente de pico y luego de mantenimiento)
    };
    
    DigitalOut* _out_high;      /// Salida de alta corriente
    DigitalOut* _out_low;       /// Salida de corriente de mantenimiento
    RelayLogicLevel _level;     /// Nivel de activación lógico
    Ticker _tick;               /// Timer para la duración de la activación con corriente de pico
    RelayStat _stat;            /// Estado de ejecución (Off, On)
    RelaySetup _config;         /// Configuración (single, double)
    uint32_t _us_high_time;     /// Tiempo de corriente de pico


	/** highTimeCb
     *  Callback a invocar cuando finalice el tiempo de corriente de pico y haya que bajara corriente de mantenimiento
     */
    void highTimeCb();

  
};
     


#endif /*__Relay__H */

/**** END OF FILE ****/


