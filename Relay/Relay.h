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
    /** Tiempo en ms por defecto, que durará la activación a máxima corriente antes de bajar a corriente de mantenimiento */
    static const uint32_t DefaultMaxCurrentTime = 100;
        
    /** Configuración del nivel lógico de activación del relé */
	enum RelayLogicLevel{
		OnIsLowLevel,       /// El relé se activa con una salida a nivel bajo
		OnIsHighLevel       /// El relé se activa con una salida a nivel alto
	};
    
    /** Tipo de estado en el que se puede encontrar un relé */
    enum RelayStat{
        RelayIsOff,         /// Desactivado 
        RelayIsOn,          /// Activado        
    };
        
	/** Constructor
     *  Durante la construcción del objeto, se asignan los pines de control (1 o 2), la lógica de activación, y
     *  el tiempo que debe proporcionarse la corriente de pico antes de bajar a la corriente de mantenimiento, en 
     *  el caso de utilizar una configuración de doble nivel.
     *  @param id Identificador del relé
     *  @param gpio_high GPIO conectado al canal de máxima corriente
     *  @param gpio_low GPIO conectado al canal de corriente de mantenimiento. Si no se utiliza dejar valor
     *          NC por defecto (NC=not connected)
     *  @param level Nivel de activación lógico 
     *  @param time_lowcurr_us Tiempo para bajar a doble nivel en us
     */
    Relay(uint32_t id, PinName gpio_high, PinName gpio_low = NC, RelayLogicLevel level = OnIsHighLevel, uint32_t time_lowcurr_us = (1000 * DefaultMaxCurrentTime));
    ~Relay(){}
  
  
	/** turnOnHigh
     *  Inicia el encendido del relé a máxima potencia. Instala callback para notificar cuando debe pasar a doble nivel
     *  @param turnLowCb Callback a notificar cuando deba bajar a doble nivel, indicando el _id del relé.
     */
    void turnOnHigh(Callback<void(uint32_t)> *turnLowCb);
  
    
	/** turnOnLow
     *  Mantiene el relé activado con corriente de mantenimiento
     */
    void turnOnLow();
  
  
	/** turnOff
     *  Apaga el relé
     */
    void turnOff();
  
  
	/** getState
     *  Obtiene el estado actual del relé
     *  @return RelayStat
     */
    RelayStat getState(){ return _stat; }   
  
  
	/** getId
     *  Obtiene el identificador del relé
     *  @return Identificador
     */
    uint32_t getId(){ return _id; }       
             
  protected:    
    uint32_t _id;               /// Identificador del relé
    uint32_t _delay_lowcurr_us; /// Retado para el doble nivel en us
    DigitalOut* _out_high;      /// Salida de alta corriente
    DigitalOut* _out_low;       /// Salida de corriente de mantenimiento
    RelayLogicLevel _level;     /// Nivel de activación lógico
    RelayStat _stat;            /// Estado de ejecución (Off, On)
    Ticker _tick_lowcurr;       /// Temporizador para el doble nivel de corriente
    Callback<void(uint32_t)> *_turnLowCb;    /// Callback para notificar paso a bajo nivel
  
  
	/** isrTicker
     *  Rutina de interrupción del ticker asociado a la bajada a doble nivel
     */
    void isrTicker();  
};
     


#endif /*__Relay__H */

/**** END OF FILE ****/


