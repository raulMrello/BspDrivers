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
    /** Tiempo en ms por defecto, que durar� la activaci�n a m�xima corriente antes de bajar a corriente de mantenimiento */
    static const uint32_t DefaultMaxCurrentTime = 100;
        
    /** Configuraci�n del nivel l�gico de activaci�n del rel� */
	enum RelayLogicLevel{
		OnIsLowLevel,       /// El rel� se activa con una salida a nivel bajo
		OnIsHighLevel       /// El rel� se activa con una salida a nivel alto
	};
    
    /** Tipo de estado en el que se puede encontrar un rel� */
    enum RelayStat{
        RelayIsOff,         /// Desactivado 
        RelayIsOn,          /// Activado        
    };
        
	/** Constructor
     *  Durante la construcci�n del objeto, se asignan los pines de control (1 o 2), la l�gica de activaci�n, y
     *  el tiempo que debe proporcionarse la corriente de pico antes de bajar a la corriente de mantenimiento, en 
     *  el caso de utilizar una configuraci�n de doble nivel.
     *  @param id Identificador del rel�
     *  @param gpio_high GPIO conectado al canal de m�xima corriente
     *  @param gpio_low GPIO conectado al canal de corriente de mantenimiento. Si no se utiliza dejar valor
     *          NC por defecto (NC=not connected)
     *  @param level Nivel de activaci�n l�gico 
     *  @param time_lowcurr_us Tiempo para bajar a doble nivel en us
     */
    Relay(uint32_t id, PinName gpio_high, PinName gpio_low = NC, RelayLogicLevel level = OnIsHighLevel, uint32_t time_lowcurr_us = (1000 * DefaultMaxCurrentTime));
    ~Relay(){}
  
  
	/** turnOnHigh
     *  Inicia el encendido del rel� a m�xima potencia. Instala callback para notificar cuando debe pasar a doble nivel
     *  @param turnLowCb Callback a notificar cuando deba bajar a doble nivel, indicando el _id del rel�.
     */
    void turnOnHigh(Callback<void(uint32_t)> *turnLowCb);
  
    
	/** turnOnLow
     *  Mantiene el rel� activado con corriente de mantenimiento
     */
    void turnOnLow();
  
  
	/** turnOff
     *  Apaga el rel�
     */
    void turnOff();
  
  
	/** getState
     *  Obtiene el estado actual del rel�
     *  @return RelayStat
     */
    RelayStat getState(){ return _stat; }   
  
  
	/** getId
     *  Obtiene el identificador del rel�
     *  @return Identificador
     */
    uint32_t getId(){ return _id; }       
             
  protected:    
    uint32_t _id;               /// Identificador del rel�
    uint32_t _delay_lowcurr_us; /// Retado para el doble nivel en us
    DigitalOut* _out_high;      /// Salida de alta corriente
    DigitalOut* _out_low;       /// Salida de corriente de mantenimiento
    RelayLogicLevel _level;     /// Nivel de activaci�n l�gico
    RelayStat _stat;            /// Estado de ejecuci�n (Off, On)
    Ticker _tick_lowcurr;       /// Temporizador para el doble nivel de corriente
    Callback<void(uint32_t)> *_turnLowCb;    /// Callback para notificar paso a bajo nivel
  
  
	/** isrTicker
     *  Rutina de interrupci�n del ticker asociado a la bajada a doble nivel
     */
    void isrTicker();  
};
     


#endif /*__Relay__H */

/**** END OF FILE ****/


