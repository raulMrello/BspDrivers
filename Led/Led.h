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

#include "mbed.h"

   
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
  
  
	/** on
     *  Inicia el encendido del led, a un nivel de intensidad, con o sin rampa incial y opcionalmente
     *  con una duraci�n m�xima. Por defecto enciende el led instant�neamente
     *  @param intensity Intensidad en porcentaje 0-100%
	 *	@param ms_duration Tiempo de duraci�n, hasta volver al estado anterior
	 *	@param ms_ramp Tiempo hasta alcanzar la intensidad (0: instant�nea, !=0: millisegundos)
	 */
    void on(uint32_t ms_duration = 0, uint8_t intensity=100, uint32_t ms_ramp = 0);


    /** off
     *  Inicia el apagado del led, a un nivel de intensidad, con o sin rampa incial y opcionalmente
     *  con una duraci�n m�xima. Por defecto deja el led apagado instant�neamente.
     *	@param ms_duration Tiempo de duraci�n, hasta volver al estado anterior
	 *  @param intensity Intensidad en porcentaje 0-100%
	 *	@param ms_ramp Tiempo hasta alcanzar la intensidad (0: instant�nea, !=0: millisegundos)
	 */
    void off(uint32_t ms_duration = 0, uint8_t intensity=0, uint32_t ms_ramp = 0);


    /** blink
     *  Inicia el parpadeo del led, con intensidades m�xima y m�nima y opcionalmente
     *  con una duraci�n m�xima. Por defecto el led parpadea de 0 a 1.
     *	@param ms_blink_on Tiempo de encendido en ms
	 *	@param ms_blink_off Tiempo de apagado en ms
	 *	@param ms_duration Tiempo de duraci�n, hasta volver al estado anterior
	 *  @param intensity_on Intensidad de encendido en porcentaje 0-100%
	 *  @param intensity_off Intensidad de apagado en porcentaje 0-100%
	 */
    void blink(uint32_t ms_blink_on, uint32_t ms_blink_off, uint32_t ms_duration = 0, uint8_t intensity_on=100, uint8_t intensity_off=0);
  
  
	/** updateBlinker
     *  Modifica los tiempos de parpadeo
     *	@param ms_blink_on Tiempo de encendido en modo parpadeo
	 *	@param ms_blink_off Tiempo de apagado en modo parpadeo (si =0 modo parpadeo desactivado)
	 */
    void updateBlinker(uint32_t ms_blink_on, uint32_t ms_blink_off);
  
  
	/** setDebugChannel()
     *  Instala canal de depuraci�n
     *  @param dbg Logger
     */
    void setDebugChannel(bool dbg) { _debug = dbg; }
 
         
  private:
    enum LedStat{
        LedIsOff,        
        LedIsOn,
        LedIsBlinking        
    };
    enum LedAction{
        LedGoingOff,
        LedGoingOn,
        LedGoOffEnd,
        LedGoOnEnd,
    };
    
    static const uint32_t GlitchFilterTimeoutUs = 20000;    /// Por defecto 20ms de timeout antiglitch desde el cambio de nivel
    uint32_t _id;                                           /// Led id. Coincide con el PinName asociado
    PwmOut* _out;                                           /// Salida pwm
    DigitalOut* _out_01;                                    /// Salida binaria 0 1
    float _intensity;                                       /// Nivel de intensidad    
    float _max_intensity;                                   /// M�ximo nivel de intensidad
    float _min_intensity;                                   /// M�nimo nivel de intensidad
    LedType _type;                                          /// Tipo de led
    LedLogicLevel _level;                                   /// Nivel l�gico para la activaci�n
    LedStat _stat;                                          /// Estado del led
    LedAction _action;                                      /// Acci�n en ejecuci�n del led
    uint32_t _period_ms;                                    /// Periodo del pwm en milisegundos
    Ticker _tick_ramp;                                      /// Timer para la rampa
    Ticker _tick_blink;                                     /// Timer para el parpadeo
    Ticker _tick_duration;                                  /// Timer para la duraci�n
    uint32_t _ms_ramp;                                      /// Milisegundos de rampa
    uint32_t _ms_blink_on;                                  /// Milisegundos de encendido (parpadeo)
    uint32_t _ms_blink_off;                                 /// Miliegundos de apagado (parpadeo)
    uint32_t _ms_duration;                                  /// Milisegundos del estado temporal
    LedStat _bkp_stat;                                      /// Estado backup en modo temporal
    bool _istemp;                                           /// Flag para indicar si el modo temporal est� activo
    bool  _debug;                                           /// Canal de depuraci�n
  
    
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
  
    
	/** convertIntensity
     *  Convierte la intensidad 0-100% en un valor de 0-1 teniendo en cuenta la l�gica
     *  @param intensity Intensidad 0-100%
     *  @return Intensidad 0 - 1.0f
     */
    float convertIntensity(uint8_t intensity);  
};
     


#endif /*__Led__H */

/**** END OF FILE ****/


