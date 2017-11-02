/*
 * MPR121_CapTouch.h
 *
 *  Created on: Oct 2017
 *      Author: raulMrello
 *
 *	MPR121_CapTouch es el driver del chip MPR121 que mediante un bus I2C proporciona el acceso a 12 canales t�ctiles
 *  capacitivos. Adem�s el chip proporciona un pin de salida IRQ para notificar eventos, activo en el flanco de bajada,
 *  as� como un pin para selecci�n de direcci�n, que puede dejarse sin conectar para utilizar la direcci�n por defecto 5Ah.
 *
 *  El uso t�pico ser�: Crear objeto, ajustar par�metros, instalar callback, invocar a touched() fuera de contexto ISR para 
 *  leer estado de los pads.
 *
 *  Adem�s para trazar el estado anterior de los pads y saber si ha sido pulsado, si sigue pulsado o si acaba de ser liberado
 *  es necesario de un manager de m�s alto nivel (ver clase TouchManager)
 *
 *
 *  void onIrq(){ thread.signal_set(1); }
 *
 *  main(){
 *    MPR121_CapTouch* mpr = new MPR121_CapTouch(PA_x, PA_y, PA_z);
 *    mpr->attachIrqCb(callback(onIrq));
 *    for(;;){
 *       thread.signal_wait(0, osWaitForever);
 *       uint6_t touch_values = mpr->touched();
 *       if(touch_values & 1){ printf("Touched elec_0");}
 *  }
 */
 
#ifndef MPR121_CAPTOUCH_H
#define MPR121_CAPTOUCH_H
 
#include "mbed.h"


class MPR121_CapTouch {
 public:
    /** Direcci�n por defecto (ADDR sin conectar) */
    static const uint8_t DefaultAddress = 0x5A;
    static const uint8_t SensorCount = 12;              /// N�mero de sensores capacitivos
    
 
    /** Estado del dispositivo */
    enum Status{
        Ready, 
        NotPresent,
        PresentWithErrors,
    };
    
    
    /** Errores devueltos por los diferentes servicios */
     enum ErrorResult{
        Success = 0,
        InvalidArguments,
        WriteError,
        ReadError,
        DutyOutOfRange,
        DeviceUndetected,
    };
    

    /** Constructor por defecto
     * @param sda L�nea sda del bus i2c
     * @param scl L�nea scl del bus i2c
     * @param irq Entrada de interrupci�n
     * @param addr Direcci�n i2c, por defecto (5Ah)
     */
    MPR121_CapTouch(PinName sda, PinName scl, PinName irq, uint8_t addr = DefaultAddress);
    

    /** Instala callback para la notificaci�n de eventos hardware
     * @param irq_cb Callback a instalar en eventos pin IRQ
     */
    void attachIrqCb(Callback<void()> irq_cb) { _irq_cb = irq_cb; }    

    /** Ajusta los thresholds para la generaci�n de eventos
     * @param touch_th Threshold de pulsaci�n en num. cuentas
     * @param release_th Threshold de liberaci�n en num. cuentas
     * @return C�digo de error
     */
    ErrorResult setThresholds(uint8_t touch_th, uint8_t release_th);
    

    /** Obtiene los datos filtrados de un electrodo
     * @param elec Electrodo
     * @return Datos filtrados
     */
    uint16_t getFilteredData(uint8_t elec);
    

    /** Obtiene los datos del baseline de un electrodo
     * @param elec Electrodo
     * @return Datos del baseline asociado
     */
    uint16_t getBaselineData(uint8_t elec);
    

    /** Obtiene el estado de pulsaci�n de los electrodos, obtiene
     *  el bit activo de los electrodos pulsados bit0 = elec0, bit11 = elec11
     * @return Estado de pulsaci�n
     */
    uint16_t touched();

  
  protected:
    static const uint8_t DefaultTouchThreshold = 12;    /// threshold en num. cuentas para detecci�n pulsaci�n
    static const uint8_t DefaultReleaseThreshold = 6;   /// threshold en num. cuentas para detecci�n liberaci�n
  
    I2C* _i2c;                      /// Puerto i2c 
    InterruptIn* _iin_irq;          /// Entrada de interrupci�n
    uint8_t _addr;                  /// Direcci�n i2c asignada
    int8_t _i2caddr;
    Status _stat;                   /// Estado de ejecuci�n
    uint8_t _touch_thr;             /// Threshold actual de pulsaci�n
    uint8_t _release_thr;           /// Threshold actual de liberaci�n      
    Callback<void()> _irq_cb;       /// Callback a invocar tras evento irq
      

    /** Procesa una interrupci�n a nivel bajo en el pin irq
     */
    void onIrqFallingEdge();
     

    /** Lee un registro de 8 bits
     *  @param reg Registro a leer
     *  @param value Recibe el valor le�do
     *  @return C�digo de error
     */
    ErrorResult readRegister8(const char reg, uint8_t* value);
   

    /** Lee un registro de 16 bits
     *  @param reg Registro a leer
     *  @param value Recibe el valor le�do
     *  @return C�digo de error
     */
    ErrorResult readRegister16(const char reg, uint16_t* value);
   

    /** Escribe un registro de 8 bits
     *  @param reg Registro a escribir
     *  @param value Valor a escribir
     *  @return C�digo de error
     */
    ErrorResult writeRegister(const char reg, char value);
  
};

#endif // MPR121_CAPTOUCH_H

