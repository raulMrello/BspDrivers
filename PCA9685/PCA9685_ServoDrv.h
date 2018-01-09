/*
 * PCA9685_ServoDrv.h
 *
 *  Created on: Oct 2017
 *      Author: raulMrello
 *
 *	PCA9685 es el driver del chip PCS9685 que mediante un bus I2C proporciona el acceso a 16 canales PWM de 12bit.
 *  Permite frecuencias pwm desde 24Hz hasta 1526Hz. Para el control de servos, la frecuencia t�pica suele ser de
 *  50Hz (20ms) y los pulsos pwm suelen ir de 1ms (�ngulo de 0�) a 2ms (�ngulo de 180�).
 *  
 */
 

#ifndef _PCA9685_H_
#define _PCA9685_H_
 
#include "mbed.h"


//------------------------------------------------------------------------------------
//- CLASE PCA9685_ServoDrv -----------------------------------------------------------
//------------------------------------------------------------------------------------


class PCA9685_ServoDrv{
    public:       
    
    
    /** Configuraciones fijas */
    static const uint8_t ServoCount = 16;       // N�mero de servos controlables
    static const uint8_t MaxAngleValue = 180;   /// �ngulo m�ximo de giro del servo
    static const uint8_t MinAngleValue = 0;     /// �ngulo m�nimo de giro del servo
    
    
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
     * @parm sda L�nea sda del bus i2c
     * @parm scl L�nea scl del bus i2c
     * @param numServos N�mero de servos controlados
     * @param addr Direcci�n i2c, por defecto = 0 (80h + 0)
     * @param pwm_period_us Periodo de la se�al pwm en us
     * @param oe L�nea /oe (opcional) para habilitar la salida pwm (por defecto NC sin uso)
     * @param hz Oscilador del chip i2c
     */
    PCA9685_ServoDrv(PinName sda, PinName scl, uint8_t numServos = ServoCount, uint8_t addr=0, uint32_t pwm_period_us=20000, PinName oe=NC, uint32_t hz=25000000);

    
    /** Destructor
    */
    ~PCA9685_ServoDrv();
    
    
    /** Devuelve el estado del chip
     *  @return Estado del chip
     */
    Status getState() { return _stat; }
    
    
    /** Establece los rangos m�nimo y m�ximo del pulso pwm en us para un canal pwm. No pueden exceder el valor
     *  m�ximo configurado en _period_us
     * @param servoId Servo id (0 to 15)
     * @param min_angle M�nimo �ngulo
     * @param max_angle Maximo �ngulo
     * @param min_duty M�nimo duty
     * @param max_duty Maximo duty
     * @return error code 
     */
    ErrorResult setServoRanges(uint8_t servoId, int16_t min_angle, int16_t max_angle, uint16_t min_duty, uint16_t max_duty);
    
    
    
    /** Establece un �ngulo para un servo concreto y opcionalmente actualiza el chip para que tome valor
     *  de forma instant�nea
     * @param servoId Servo id (0 to 15)
     * @param angle Angulo entre 0 y 180
     * @param update True (escribe en chip), False (s�lo actualiza variable)
     * @return error code 
     */
    ErrorResult setServoAngle(uint8_t servoId, uint8_t angle, bool update=false);
    

    /** Establece el pulso de un canal pwm
     * @param servoId Servo id (0 to 15)
     * @param duty  Duty value (0 to 4096) 
     * @param update True (escribe en chip), False (s�lo actualiza variable)
     * @return error code 
     */
    ErrorResult setServoDuty(uint8_t servoId, uint16_t duty, bool update=false);    
    
    
    /** Lee los rangos m�nimo y m�ximo del pulso pwm en us para un canal pwm
     * @param servoId Servo id (0 to 15)
     * @param min_angle M�nimo �ngulo
     * @param max_angle Maximo �ngulo
     * @param min_duty M�nimo duty
     * @param max_duty Maximo duty
     * @return error code 
     */
    ErrorResult getServoRanges(uint8_t servoId, int16_t* min_angle, int16_t* max_angle, uint16_t* min_duty, uint16_t* max_duty);
    
    
    /** Obtiene el �ngulo para un servo concreto
     * @param servoId Servo id (0 to 15)
     * @return angle Obtiene el �ngulo entre 0 y 180
     */
    uint8_t getServoAngle(uint8_t servoId);


    /** Lee el pulso de un canal pwm
     * @param servoId Servo id (0 to 15)
     * @return duty  Recibe el valor del pulso (0...4096)
     */
    uint16_t getServoDuty(uint8_t servoId);
    
    
    /** Env�a los pulsos en _dutyValue al chip i2c
     * @return C�digo de error
     */    
    ErrorResult updateAll();  
    
    
    /** Lee del chip i2c, el valor real del duty correspondiente a un servo
     *  @param servoId Servo 
     *  @param duty Recibe el valor del duty le�do
     *  @return C�digo de error
     */    
    ErrorResult readServoDuty(uint8_t servoId, uint16_t* duty);


    /** Obtiene el duty equivalente a un �ngulo de giro dado para un servo en concreto
     * @param servoId Identificador del servo
     * @param angle �ngulo en grados
     * @return Recibe el valor del pulso (0...4096)
     */
    uint16_t getDutyFromAngle(uint8_t servoId, uint8_t angle);    


    /** Obtiene el �ngulo equivalente a un duty para un servo dado, cuyos rangos han sido ajustados
     * @param servoId Identificador del servo
     * @param duty Duty en n�mero de cuentas
     * @return Recibe el valor del �ngulo (0..180)
     */
    uint16_t getAngleFromDuty(uint8_t servoId, uint16_t duty); 
    
  
	/** getNVDataSize()
     *  Obtiene el tama�o de los datos NV
     */
    uint32_t getNVDataSize(){ return(sizeof(NVData_t)); }
    
  
	/** setNVData()
     *  Actualiza los datos NVData
     *  @param data Datos de recuperaci�n
     *  @return 0 si es correcto, !=0 si los datos son incorrectos o inv�lidos
     */
    int setNVData(void* data);
    
  
	/** getNVData()
     *  Lee los datos NVData
     *  @param data Recibe los datos NV de recuperaci�n
     */
    void getNVData(uint32_t* data); 
    
    
  protected:
    static const uint16_t MaxAllowedDuty = 4095;    /// Valor m�ximo del duty
        
    /** Estructura de datos con los par�metros de calibraci�n */    
    struct NVData_t{
        int16_t minAngle[ServoCount];
        int16_t maxAngle[ServoCount];
        uint16_t minDuty[ServoCount];
        uint16_t maxDuty[ServoCount];
        uint32_t crc32;
    };

  
    int16_t     _minAngle[ServoCount];          /// Rango inferior en grados para cada servo
    int16_t     _maxAngle[ServoCount];          /// Rango superior en grados por servo
    uint16_t    _minDuty[ServoCount];          /// Duty inferior por servo
    uint16_t    _maxDuty[ServoCount];          /// Duty superior por servo
    uint16_t    _dutyValue[ServoCount];         /// Duty actual por servo
    uint16_t    _angleValue[ServoCount];        /// �ngulo actual por servo
    uint8_t     _addr;                          /// Direcci�n driver I2C
    uint32_t    _hz;                            /// Frecuencia clock chip PCA
    uint32_t    _period_us;                     /// Periodo pwm
    uint32_t    _freq;                          /// Frecuencia
    I2C*        _i2c;                           /// Driver i2c
    DigitalOut* _oe;                            /// Salida /OE para el chip PCA
    Status       _stat;                         /// Estado de funcionamiento
    uint8_t     _num_servos;                    /// N�mero de servos  
  
  
    /** Obtiene el valor de los duty en el chip y los copia a la variable _dutyValue
     * @return C�digo de error
     */    
    ErrorResult getAllDuty();    

    
    /** Get driver content, by register
     * @returns error code <= 0
     */
    ErrorResult getDriverContent(void);    
    
};

#endif

