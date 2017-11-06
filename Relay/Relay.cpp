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
Relay::Relay(uint32_t id, PinName gpio_high, PinName gpio_low, RelayLogicLevel level, uint32_t time_lowcurr_us){
    // Crea objetos
    _id = id;
    _delay_lowcurr_us = time_lowcurr_us;

    _out_high = new DigitalOut(gpio_high);
    _out_low = NULL;
    if(gpio_low != NC){
        _out_low = new DigitalOut(gpio_low);
    }
    _level = level;

    // Deja apagado por defecto
    turnOff();
}


//------------------------------------------------------------------------------------
void Relay::turnOnHigh(Callback<void(uint32_t)> *turnLowCb){    
    uint8_t value = (_level == OnIsHighLevel)? 1 : 0;
    _stat = RelayIsOn;          
    _out_high->write(value);    
    if(_out_low){
        _out_low->write(value);
        _turnLowCb = turnLowCb;
        _tick_lowcurr.attach_us(callback(this, &Relay::isrTicker), _delay_lowcurr_us);
    }
}


//------------------------------------------------------------------------------------
void Relay::turnOnLow(){
    uint8_t value = (_level == OnIsHighLevel)? 0 : 1;
    _stat = RelayIsOn;          
    if(_out_low){
        _out_low->write(value);
    }
}


//------------------------------------------------------------------------------------
void Relay::turnOff(){
    uint8_t value = (_level == OnIsHighLevel)? 0 : 1;
    _stat = RelayIsOff;          
    _out_high->write(value);
    if(_out_low){
        _out_low->write(value);
    }
}
 

 
//------------------------------------------------------------------------------------
//-- PRIVATE METHODS IMPLEMENTATION --------------------------------------------------
//------------------------------------------------------------------------------------



//------------------------------------------------------------------------------------
void Relay::isrTicker(){ 
    _tick_lowcurr.detach();
    _turnLowCb->call(_id);
}
