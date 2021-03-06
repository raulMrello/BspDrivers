#include "mbed.h"
#include "MQLib.h"
#include "Logger.h"
#include "WS281xLedStrip.h"


// **************************************************************************
// *********** DEFINICIONES *************************************************
// **************************************************************************


/** Macro de impresi�n de trazas de depuraci�n */
#define DEBUG_TRACE(format, ...)    if(logger){logger->printf(format, ##__VA_ARGS__);}


// **************************************************************************
// *********** OBJETOS  *****************************************************
// **************************************************************************

/** Canal de depuraci�n */
static Logger* logger;
/** Driver control detector */
static WS281xLedStrip* leddrv;


// **************************************************************************
// *********** TEST  ********************************************************
// **************************************************************************



//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
void test_WS281x(){

            
    // --------------------------------------
    // Inicia el canal de comunicaci�n remota
    //  - Pines USBTX, USBRX a 115200bps 
    logger = new Logger(USBTX, USBRX, 16, 115200);
    DEBUG_TRACE("\r\nIniciando test_WS281x...\r\n");


    // --------------------------------------
    // Creo driver de control para los servos
    //  - Direcci�n I2C = 0h
    //  - N�mero de servos controlables = 12 (0 al 11)    
    DEBUG_TRACE("\r\nCreando Driver WS281x...");    
    leddrv = new WS281xLedStrip(PA_8, 800000, 3);
        
    // situo todos a 0� y doy la orden sincronizada
    DEBUG_TRACE("\r\nAjustando colores rojo, verde, azul... ");
    WS281xLedStrip::Color_t color;
    color.red = 255; color.green = 0; color.blue = 0;            
    leddrv->setRange(0, 1, color);
    color.red = 0; color.green = 255; color.blue = 0;            
    leddrv->setRange(1, 2, color);
    color.red = 0; color.green = 0; color.blue = 255;            
    leddrv->setRange(2, 3, color);
    // inicio
    DEBUG_TRACE("\r\nSTART... ");
    leddrv->start();
    for(;;){
        // espero 5 segundos
        Thread::wait(5000);
        DEBUG_TRACE("\r\nCambio a verde m�nimo... ");
        color.red = 0; color.green = 1; color.blue = 0;
        leddrv->setRange(0, 3, color);
        Thread::wait(5000);
        DEBUG_TRACE("\r\nCambio a azul cuarto... ");
        color.red = 0; color.green = 0; color.blue = 64;
        leddrv->setRange(0, 3, color);
        Thread::wait(5000);
        DEBUG_TRACE("\r\nCambio a rojo medio... ");
        color.red = 128; color.green = 0; color.blue = 0;     
        leddrv->setRange(0, 3, color);        
    }    
}

