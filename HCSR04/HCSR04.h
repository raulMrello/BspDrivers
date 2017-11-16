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
        NoEvents,               /// No hay eventos
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
	
    /** @enum ErrorResult
     *  @brief Tipos de errores generados por el driver
     */	
    enum ErrorResult{
        NoErrors,               /// No hay ning�n error
        TriggerError,           /// Error al realizar el trigger
        EchoStartError,         /// Error al recibir un inicio de eco indebido
        EchoEndError,           /// Evento al recibir un fin de eco indebido
        EchoMissingError,       /// Error al no completar un echo recibido en el plazo establecido
    };    
		
    /** Callback definida para la notificaci�n de eventos de medida */
    typedef Callback<void(DistanceEvent, int16_t)> DistEventCallback;
    
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

	
    /** @fn config()
     *  @brief Establece los par�metros configurables como l�mite de variaci�n entre medidas para la notificaci�n de
     *  eventos y n�mero de medidas consecutivas para el filtrado antiglitch.
     *  @param max_dist M�xima distancia a la que responder� (limitada a MaxDetectableRange)
     *  @param approach_dist Diferencia de distancia al acercarse en cm
     *  @param goaway_dist Diferencia de distancia al alejarse en cm
     *  @param filt_count N�mero de medidas para el filtro antiglitch (=0 filtro desactivado). El valor m�ximo no puede
     *  @param filt_range Rango en cm para considerar medidas del filtro como similares meas[1] = meas[0]+-filt_range
     *  @param invalid_evt Flag para habilitar o no la publicaci�n de eventos de medidas instant�neas
     *  @param err_evt Flag para habilitar o no la publicaci�n de eventos de errores en la medida
     *  superar el valor MaxFilterSamples, de lo contrario lo limitar� a ese valor m�ximo.
     */
    void config(uint16_t max_dist, uint16_t approach_dist, uint16_t goaway_dist, uint8_t filt_count=0, uint16_t filt_range = (uint16_t)MaxDetectableRange, uint8_t invaild_evt=0, uint8_t err_evt=0);
	
        
    /** @fn start()
     *  @brief Inicia la medida.
     *  @param evCb Callback instalada para recibir los eventos de medida
     *  @param lapse_ms Lapso en ms para repetir la medida cont�nuamente. Si se deja a 0 s�lo realiza una medida.
     *  @param timeout_ms Tiempo en ms para completar la captura antes de generar error.
     */
    void start(DistEventCallback evCb, uint32_t lapse_ms, uint32_t timeout_ms = DefaultFailTimeout);

	
    /** @fn stop()
     *  @brief Detiene la medida
     */
    void stop();

	
    /** @fn getLastEvent()
     *  @brief Obtiene los par�metros del �ltimo evento generado
     *  @param ev Recibe el evento
     *  @param dist Recibe la distancia en cm
     */
    void getLastEvent(DistanceEvent* ev, int16_t* dist);
    
        
  protected:           
    
    static const uint8_t  MaxFilterSamples = 5;         /// M�ximo n�mero de muestras utilizadas en el filtro
    static const int16_t  MaxDetectableRange = 400;     /// Longitud m�xima que puede detectar (~400cm)
    static const uint32_t DefaultDifDistance = 50;      /// Eventos por defecto al detectar cambios de al menos 50cm
    static const uint32_t DefaultTimeoutUs = 2000000;   /// Tiempo de vuelo por defecto en modo one-shot (2sec)
    static const uint32_t DefaultFailTimeout = 100;     /// Tiempo para generar error en la medida

  /** Estructura para el filtro */
    struct FiltData_t{
        uint8_t count;
        uint8_t curr;
        uint16_t range;
        int16_t dist_cm[MaxFilterSamples];
    };
  
    State _stat;                            /// Estado de ejecuci�n
    ErrorResult _last_error;                /// �ltimo error 
    uint16_t _max_dist_cm;                  /// M�xima distancia a la que ser� receptivo
    uint16_t _approach_dist_cm;             /// Diferencia de distancia al aproximarse
    uint16_t _goaway_dist_cm;               /// Diferencia de distancia al alejarse
    uint8_t  _filt_count;                   /// N�mero de medidas para el filtro antiglitch
    uint32_t  _echo_time;                   /// Tiempo del echo en us
    DistEventCallback _callback;            /// Callback a invocar en cada evento
  
    DigitalOut* _out_trig;                  /// Salida digital trigger
    InterruptIn* _iin_echo;                 /// Entrada de interrupci�n de echo
    
    void echoStart();                       /// Callback a invocar al detectar inicio de echo
    void echoEnd();                         /// Callback a invocar al detectar fin de echo
    void echoMissing();                     /// Callback a invocar al detectar fallo por echo no devuelto
    void trigger();                         /// Callback a invocar al iniciar un trigger
  
    uint32_t _lapse_us;                     /// Cadencia en us para eventos del trigger
    uint32_t _timeout_us;                   /// Temporizaci�n de error por timeout
    Ticker _tick_trig;                      /// Ticker para la generaci�n de triggers
    Ticker _tick_fail;                      /// Ticker para la detecci�n de errores
    Timer _echo_tmr;                        /// Timer para el c�lculo del echo
    bool  _en_inv_evts;                     /// Flag para la notificaci�n de eventos de medidas instant�neas
    bool  _en_err_evts;                     /// Flag para la notificaci�n de eventos de medidas con errores
    
    DistanceEvent _last_event;              /// �ltimo evento generado
    int16_t _last_dist_cm;                  /// Distancia anterior del objeto    
    uint32_t _num_meas;                     /// N�mero de medidas realizadas
    FiltData_t _filter;                     /// Filtro de datos

	
    /** @fn restart()
     *  @brief Reinicia la medida (generalmente tras una condici�n de error)
     */
    void restart();
    
	
    /** @fn filtData()
     *  @brief Realiza el filtrado de la medida actual
     *  @param dist Medida actual
     *  @return True si el datos es v�lido, False si el dato es un glitch
     */
    bool filtData(int16_t dist);            /// Rutina para filtrar el �ltimo dato registrado
    
};    

#endif   /* HCSR04_H */
