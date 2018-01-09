#include "mbed.h"
#include "MQLib.h"
#include "Logger.h"
#include "HCSR04.h"


// **************************************************************************
// *********** DEFINICIONES *************************************************
// **************************************************************************


/** Macro de impresi�n de trazas de depuraci�n */
#define DEBUG_TRACE(format, ...)    if(logger){logger->printf(format, ##__VA_ARGS__);}


// **************************************************************************
// *********** OBJETOS  *****************************************************
// **************************************************************************

/** Canal de comunicaci�n remota */
static Logger* logger;

/** Driver control detector */
static HCSR04* distdrv;


// **************************************************************************
// *********** TEST  ********************************************************
// **************************************************************************



//------------------------------------------------------------------------------------
static osThreadId th;
static HCSR04::DistanceEvent g_ev;
static uint16_t g_dist;

//------------------------------------------------------------------------------------
static void distEventCallback(HCSR04::DistanceEvent ev, int16_t dist){
    g_ev = ev;
    g_dist = dist;
    osSignalSet(th, 1);
}




//------------------------------------------------------------------------------------
void test_HCSR04(){

    th = Thread::gettid();
            
    // --------------------------------------
    // Inicia el canal de comunicaci�n remota
    logger = new Logger(USBTX, USBRX, 16, 115200);    
    DEBUG_TRACE("\r\nIniciando test_PCA9685...\r\n");


    // --------------------------------------
    // Creo driver de control para los servos
    //  - Direcci�n I2C = 0h
    //  - N�mero de servos controlables = 12 (0 al 11)    
    DEBUG_TRACE("\r\nCreando Driver HCSR04...");    
    distdrv = new HCSR04(PA_0, PA_1);
        
    // situo todos a 0� y doy la orden sincronizada
    DEBUG_TRACE("\r\nAjustando distancias a 10cm... ");
    distdrv->config(10, 10, 3);
    
    // inicio
    DEBUG_TRACE("\r\nSTART... ");
    distdrv->start(callback(&distEventCallback), 2000);
    
    for(;;){
        osSignalWait(0, osWaitForever);
        DEBUG_TRACE("\r\nEvent=%d, Dist=%dcm", g_ev, g_dist);
    }    
}

