/*
 * HCSR04.h
 *
 *  Created on: Oct 2017
 *      Author: raulMrello
 *
 *  HCSR04 es un m�dulo C++ que gestiona la detecci�n de objetos cercanos mediante el sensor HC-SR04 de ultrasonidos.
 *  Se requiere una salida digital que emita un pulso de 10us y una entrada de interrupci�n, para conocer la distancia
 *  del objeto (ver https://os.mbed.com/components/HC-SR04/). Para ello se medir� la anchura del semiciclo activo del
 *  pin de entrada y se aplicar� la f�rmula: Distancia (cm) = tON(us)/58.
 *
 *  El driver se puede configurar con los siguientes par�metros:
 *
 *  - Cadencia de chequeo de presencia (cada X ms)
 *  - Rango de alerta, cuando dos medidas de distancia var�an m�s de +-Dif. notificando el evento ACERCANDOSE|ALEJANDOSE y
*     la distancia medida. 
 *
 */
 
 
#ifndef HCSR04_H
#define HCSR04_H

 
#include "mbed.h"



//------------------------------------------------------------------------------------
//- CLASS HCSR04 ------------------------------------------------------------
//------------------------------------------------------------------------------------


class HCSR04 {
  public:
	
    /** @enum DistanceEvent
     *  @brief Tipos de eventos generados por el driver
     */	
    enum DistanceEvent{
        Approaching,            /// Evento al acercarse
        MovingAway,             /// Evento al alejarse
        MeasureError,           /// Error en la operaci�n de medida
    };
	
    /** @enum State
     *  @brief Estado de ejecuci�n del driver
     */	
    enum State{
        Stopped,            /// Detenido, no hace nada
        WaitingTrigger,     /// Esperando evento del ticker para iniciar un nuevo trigger
        Triggered,          /// Trigger iniciado
        WaitingEcho,        /// Trigger concluido, esperando echo
        WaitingEchoEnds,    /// Echo recibido, esperando final de echo
    };
		
    
    /** @fn HCSR04()
     *  @brief Constructor, que asocia una salida digital y una interrupci�n entrante
     *  @param trig Pin de salida digital
     *  @param echo Pin de entrada con el echo devuelto
     */
    HCSR04(PinName trig, PinName echo);

	
    /** @fn ~HCSR04()
     *  @brief Destructor por defecto
     */
    virtual ~HCSR04(){}

	
    /** @fn setDistRange()
     *  @brief Establece la diferencia de distancia para notificar eventos al acercarse y al alejarse
     *  @param approach_dist Diferencia de distancia al acercarse en cm
     *  @param goaway_dist Diferencia de distancia al alejarse en cm
     */
    void setDistRange(int16_t approach_dist, int16_t goaway_dist);
	
        
    /** @fn start()
     *  @brief Inicia la medida.
     *  @param evCb Callback instalada para recibir los eventos de medida
     *  @param lapse_ms Lapso en ms para repetir la medida cont�nuamente. Si se deja a 0 s�lo realiza una medida.
     */
    void start(Callback<void(DistanceEvent, uint16_t)> evCb, uint32_t lapse_ms = 0);

	
    /** @fn stop()
     *  @brief Detiene la medida
     */
    void stop();

    
        
  protected:           
    static const uint32_t DefaultDifDistance = 50;      // Eventos por defecto al detectar cambios de al menos 50cm
    static const uint32_t DefaultTimeoutUs = 2000000;  // Tiempo de vuelo por defecto en modo one-shot (2sec)
  
    State _stat;                            /// Estado de ejecuci�n
    uint16_t _approach_dist_cm;             /// Diferencia de distancia al aproximarse
    uint16_t _goaway_dist_cm;               /// Diferencia de distancia al alejarse
    uint32_t  _echo_time;                   /// Tiempo del echo en us
    Callback<void(DistanceEvent, uint16_t)> _callback;   /// Callback a invocar en cada evento
  
    DigitalOut* _out_trig;                  /// Salida digital trigger
    InterruptIn* _iin_echo;                 /// Entrada de interrupci�n de echo
    
    void echoStart();                       /// Callback a invocar al detectar inicio de echo
    void echoEnd();                         /// Callback a invocar al detectar fin de echo
    void trigger();                         /// Callback a invocar al iniciar un trigger
  
    uint32_t _lapse_us;                     /// Cadencia en us para eventos del trigger
    Ticker _tick_trig;                      /// Ticker para la generaci�n de triggers
    Timer _echo_tmr;                        /// Timer para el c�lculo del echo
  
    uint16_t _last_dist_cm;                 /// Distancia anterior del objeto
    uint32_t _num_meas;                     /// N�mero de medidas realizadas
};    



#endif   /* HCSR04_H */
