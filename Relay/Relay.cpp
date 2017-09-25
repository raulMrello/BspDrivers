/*
 * Relay.cpp
 *
 *  Created on: 20/04/2015
 *      Author: raulMrello
 */

#include "Relay.h"



//------------------------------------------------------------------------------------
//--- PRIVATE TYPES ------------------------------------------------------------------
//------------------------------------------------------------------------------------



//------------------------------------------------------------------------------------
//-- PUBLIC METHODS IMPLEMENTATION ---------------------------------------------------
//------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------
Relay::Relay(PinName gpio_high, PinName gpio_low, RelayLogicLevel level, uint32_t us_high_time){
    // Crea objetos
    _out_high = new DigitalOut(gpio_high);
    _out_low = new DigitalOut(gpio_low);
    _level = level;
    _us_high_time = us_high_time;
    _config = RelayIsDoubleLevel;
    if(gpio_low == NC){
        _config = RelayIsSingleLevel;
    }
    
    // Deja apagado por defecto
    turnOff();
}


//------------------------------------------------------------------------------------
void Relay::turnOn(){
    uint8_t value = (_level == OnIsHighLevel)? 1 : 0;
    _tick.detach();
    _stat = RelayIsOn;          
    _out_high->write(value);
    _out_low->write(value);
    if(_config == RelayIsDoubleLevel){
        _tick.attach_us(callback(this, &Relay::highTimeCb), _us_high_time);
    }
}


//------------------------------------------------------------------------------------
void Relay::turnOff(){
    uint8_t value = (_level == OnIsHighLevel)? 0 : 1;
    _tick.detach();
    _stat = RelayIsOff;          
    _out_high->write(value);
    _out_low->write(value);
}
 

 
//------------------------------------------------------------------------------------
//-- PRIVATE METHODS IMPLEMENTATION --------------------------------------------------
//------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------
void Relay::highTimeCb(){
    uint8_t value = (_level == OnIsHighLevel)? 0 : 1;
    _tick.detach();
    _out_high->write(value);    
}


