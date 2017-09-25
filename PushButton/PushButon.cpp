/*
 * PushButton.cpp
 *
 *  Created on: 20/04/2015
 *      Author: raulMrello
 */

#include "PushButton.h"



//------------------------------------------------------------------------------------
//--- PRIVATE TYPES ------------------------------------------------------------------
//------------------------------------------------------------------------------------


#define NULL_CALLBACK   (void(*)(uint32_t))0
 
//------------------------------------------------------------------------------------
//-- PUBLIC METHODS IMPLEMENTATION ---------------------------------------------------
//------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------
PushButton::PushButton(PinName btn, uint32_t id, LogicLevel level){
    // Crea objeto
    _iibtn = new InterruptIn(btn);
    _level = level;
    _id = id;
    
    // Asocia manejadores de interrupción
    _iibtn->rise(callback(this, &PushButton::isrRiseFallCallback));
    _iibtn->fall(callback(this, &PushButton::isrRiseFallCallback));
    _tick.detach();
    _enable_ticker = false;
    
    // Desactiva las callbacks de notificación
    _pressCb = callback(NULL_CALLBACK);
    _holdCb = callback(NULL_CALLBACK);
    _releaseCb = callback(NULL_CALLBACK);    
}


//------------------------------------------------------------------------------------
void PushButton::enablePressEvents(Callback<void(uint32_t)>pressCb){
    _pressCb = pressCb;
}


//------------------------------------------------------------------------------------
void PushButton::enableHoldEvents(Callback<void(uint32_t)>holdCb, uint32_t millis){
    _holdCb = holdCb;
    _hold_us = 1000 * millis;
    _enable_ticker = true;
}


//------------------------------------------------------------------------------------
void PushButton::enableReleaseEvents(Callback<void(uint32_t)>releaseCb){
    _releaseCb = releaseCb;
}


//------------------------------------------------------------------------------------
void PushButton::disablePressEvents(){
    _pressCb = callback(NULL_CALLBACK);
}


//------------------------------------------------------------------------------------
void PushButton::disableHoldEvents(){
    _holdCb = callback(NULL_CALLBACK);
    _tick.detach();
    _enable_ticker = false;
}


//------------------------------------------------------------------------------------
void PushButton::disableReleaseEvents(){
    _releaseCb = callback(NULL_CALLBACK);
}



 
//------------------------------------------------------------------------------------
//-- PRIVATE METHODS IMPLEMENTATION --------------------------------------------------
//------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------
void PushButton::isrRiseFallCallback(){
    _tick.attach_us(callback(this, &PushButton::isrFilterCallback), GlitchFilterTimeoutUs);
}


//------------------------------------------------------------------------------------
void PushButton::isrFilterCallback(){
    // chequeo si es release
    if((_iibtn->read() == 1 && _level == PressIsLowLevel) || (_iibtn->read() == 0 && _level == PressIsHighLevel)){
        _tick.detach();
        _releaseCb.call(_id);
        return;
    }
    // chequeo si es press
    if((_iibtn->read() == 1 && _level == PressIsHighLevel) || (_iibtn->read() == 0 && _level == PressIsLowLevel)){
        if(_enable_ticker){
            _tick.attach_us(callback(this, &PushButton::isrTickCallback), _hold_us);
        }
        _pressCb.call(_id);
        return;
    }
}


//------------------------------------------------------------------------------------
void PushButton::isrTickCallback(){
    _holdCb.call(_id);
}

