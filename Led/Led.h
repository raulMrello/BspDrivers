/*
 * Led.h
 *
 *  Created on: Sep 2017
 *      Author: raulMrello
 *
 *	Led es el m�dulo encargado de gestionar la activaci�n y apagado de un LED. Es posible generar patrones de parpadeo a diferentes
 *  velocidades, con diferente nivel de intensidad, con encendidos y apagados instant�neos o en rampa.
 *
 */
 
#ifndef __Led__H
#define __Led__H

#if __MBED__ == 1
#include "mbed.h"
#endif

   
class Led{
  public:

    /** Configuraci�n para establecer el tipo de led */
    enum LedType{
        LedOnOffType,
        LedDimmableType,
    };
  
    /** Configuraci�n para establecer la l�gica de activaci�n */
	enum LedLogicLevel{
		OnIsLowLevel,
		OnIsHighLevel
	};
		
	/** Constructor
     *  @param led GPIO conectado al led
     *  @param type Tipo de led (DigitalOut o PwmOut)
     *  @param level Nivel de activaci�n l�gico 
     *  @param period Periodo del del pwm en milisegundos
     */
    Led(PinName led, LedType type, LedLogicLevel level = OnIsHighLevel, uint32_t period_ms = 1);
    ~Led();
  
  
	/** start
     *  Inicia el encendido del led, hasta un nivel de intensidad dado. Permite realizar las siguientes funciones:
     *      Encendido normal instant�neo: (ms_blink_on = 0, ms_blink_off = 0, ms_ramp = 0)
     *                                      _____ 
     *                                  ___|
     *      Encendido normal en rampa: (ms_blink_on = 0, ms_blink_off = 0, ms_ramp > 0)
     *                                       _____
     *                                  ____/
     *      Encendido temporizado (tanto instant�neo como en rampa (ms_blink_on > 0, ms_blink_off = 0)
     *                                       _____
     *                                  ____/     \___
     *      Encendido en parpadeo (tanto instant�neo como en rampa) (ms_blink_on > 0, ms_blink_off > 0)
     *                                       ___     ___     ___
     *                                  ____/   \___/   \___/   \__...
     *
     *  @param intensity Intensidad en porcentaje 0-100%
	 *	@param ms_blink_on Tiempo de encendido en modo parpadeo
	 *	@param ms_blink_off Tiempo de apagado en modo parpadeo (si =0 modo parpadeo desactivado)
	 *	@param ms_ramp Tiempo hasta alcanzar la intensidad (0: instant�nea, !=0: millisegundos)
     */
    void start(float intensity=1.0f, uint32_t ms_blink_on = 0, uint32_t ms_blink_off = 0, uint32_t ms_ramp = 0);
  
  
	/** stop
     *  Inicia el apagado del led
     *	@param ms_ramp Tiempo hasta apagar (0: instant�nea, !=0: millisegundos)
     */
    void stop(uint32_t ms_ramp = 0);
  
  
	/** updateBlinker
     *  Modifica los tiempos de parpadeo
     *	@param ms_blink_on Tiempo de encendido en modo parpadeo
	 *	@param ms_blink_off Tiempo de apagado en modo parpadeo (si =0 modo parpadeo desactivado)
	 */
    void updateBlinker(uint32_t ms_blink_on, uint32_t ms_blink_off);
  
  
	/** temporalState
     *  Cambia temporalmente el estado del led y despu�s vuelve al modo en el que se encontraba
     *  @param intensity Intensidad del estado temporal
     *	@param ms_temp Tiempo en el nuevo estado temporal (ms)
	 */
    void temporalState(float intensity, uint32_t ms_temp);
 
         
  protected:
    enum LedStat{
        LedIsOff,        
        LedIsOn,
        LedIsGoingOff,
        LedIsGoingOn
    };
    static const uint32_t GlitchFilterTimeoutUs = 20000;    /// Por defecto 20ms de timeout antiglitch desde el cambio de nivel
    PwmOut* _out;                                           /// Salida pwm
    DigitalOut* _out_01;                                    /// Salida binaria 0 1
    float _intensity;                                       /// Nivel de intensidad    
    float _max_intensity;                                   /// M�ximo nivel de intensidad
    LedType _type;                                          /// Tipo de led
    LedLogicLevel _level;                                   /// Nivel l�gico para la activaci�n
    LedStat _stat;                                          /// Estado del led
    uint32_t _period_ms;                                    /// Periodo del pwm en milisegundos
    Ticker _tick;                                           /// Timer para el parpadeo
    uint32_t _ms_ramp;                                      /// Milisegundos de rampa
    uint32_t _ms_blink_on;                                  /// Milisegundos de encendido (parpadeo)
    uint32_t _ms_blink_off;                                 /// Miliegundos de apagado (parpadeo)
    bool _temp_state;                                       /// Flag para indicar que est� en modo temporal
  
    
	/** rampOffCb
     *  Callback para apagar de forma gradual
     */
    void rampOffCb();
  
    
	/** rampOnCb
     *  Callback para encender de forma gradual
     */
    void rampOnCb();
  
    
	/** blinkCb
     *  Callback para parpadear
     */
    void blinkCb();
  
    
	/** temporalCb
     *  Callback para ejecutar la acci�n temporal
     */
    void temporalCb();
  
};
     


#endif /*__Led__H */

/**** END OF FILE ****/


