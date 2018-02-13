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
/** Macro para imprimir trazas de depuración, siempre que se haya configurado un objeto
 *	Logger válido (ej: _debug)
 */

#define DEBUG_TRACE(format, ...)			\
if(_defdbg){								\
	printf(format, ##__VA_ARGS__);			\
}											\


//------------------------------------------------------------------------------------
static void nullCallback(uint32_t id){
}


//------------------------------------------------------------------------------------
//-- PUBLIC METHODS IMPLEMENTATION ---------------------------------------------------
//------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------
PushButton::PushButton(PinName btn, uint32_t id, LogicLevel level, PinMode mode, bool defdbg) : _defdbg(defdbg) {
    // Crea objeto
	DEBUG_TRACE("\r\n[PushBtn]....... Creando PushButton en pin %d", btn);
	_iin = new InterruptIn(btn);
	MBED_ASSERT(_iin);
	_iin->mode(mode);
    _level = level;
    _id = id;
    
    // Desactiva las callbacks de notificación
    DEBUG_TRACE("\r\n[PushBtn]....... Desactivando callbacks");
    _pressCb = callback(&nullCallback);
    _holdCb = callback(&nullCallback);
    _releaseCb = callback(&nullCallback);

    // Asocia manejadores de interrupción
    DEBUG_TRACE("\r\n[PushBtn]....... Deteniendo ticker");
    _tick_filt = new RtosTimer(callback(this, &PushButton::isrFilterCallback), osTimerOnce, "BtnTmrFilt");
    _tick_hold = new RtosTimer(callback(this, &PushButton::isrTickCallback), osTimerPeriodic, "BtnTmrHold");
//    _tick.detach();
    _enable_ticker = false;


    DEBUG_TRACE("\r\n[PushBtn]....... Asignando RISE & FALL callbacks");
    _curr_value = _iin->read();
    enableRiseFallCallbacks();
}


//------------------------------------------------------------------------------------
void PushButton::enablePressEvents(Callback<void(uint32_t)>pressCb){
	if(pressCb){
		_pressCb = pressCb;
	}
	else{
		_pressCb = callback(&nullCallback);
	}
}


//------------------------------------------------------------------------------------
void PushButton::enableHoldEvents(Callback<void(uint32_t)>holdCb, uint32_t millis){
	if(holdCb && millis > 0){
		_holdCb = holdCb;
		_hold_us = 1000 * millis;
		_enable_ticker = true;
	}
	else{
		_holdCb = callback(&nullCallback);
		_tick.detach();
		_enable_ticker = false;
	}
}


//------------------------------------------------------------------------------------
void PushButton::enableReleaseEvents(Callback<void(uint32_t)>releaseCb){
	if(releaseCb){
		_releaseCb = releaseCb;
	}
	else{
		releaseCb = callback(&nullCallback);
	}
}


//------------------------------------------------------------------------------------
void PushButton::disablePressEvents(){
    _pressCb = callback(&nullCallback);
}


//------------------------------------------------------------------------------------
void PushButton::disableHoldEvents(){
    _holdCb = callback(&nullCallback);
    _tick.detach();
    _enable_ticker = false;
}


//------------------------------------------------------------------------------------
void PushButton::disableReleaseEvents(){
    _releaseCb = callback(&nullCallback);
}



 
//------------------------------------------------------------------------------------
//-- PRIVATE METHODS IMPLEMENTATION --------------------------------------------------
//------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------
void PushButton::isrRiseCallback(){
	_iin->rise(NULL);
	_curr_value = 1;
//	Thread::wait(GlitchFilterTimeoutUs/1000);
//	isrFilterCallback();
	_tick_filt->start(GlitchFilterTimeoutUs/1000);
//    _tick.attach_us(callback(this, &PushButton::isrFilterCallback), GlitchFilterTimeoutUs);
}


//------------------------------------------------------------------------------------
void PushButton::isrFallCallback(){
	_iin->fall(NULL);
	_curr_value = 0;
//	Thread::wait(GlitchFilterTimeoutUs/1000);
//	isrFilterCallback();
	_tick_filt->start(GlitchFilterTimeoutUs/1000);
//    _tick.attach_us(callback(this, &PushButton::isrFilterCallback), GlitchFilterTimeoutUs);
}

//------------------------------------------------------------------------------------
void PushButton::isrFilterCallback(){
	// desactivo timming
	// leo valor del pin
    uint8_t pin_level = (uint8_t)_iin->read();

	// En caso de glitch, descarto y vuelvo a habilitar interrupciones
	if(_curr_value != pin_level){
		_curr_value = pin_level;
		_tick_hold->stop();
//		_tick.detach();
		enableRiseFallCallbacks();
        return;
	}

	// En caso de evento RELEASE
	if((pin_level == 1 && _level == PressIsLowLevel) || (pin_level == 0 && _level == PressIsHighLevel)){
		DEBUG_TRACE("\r\n[PushBtn]....... EV_RELEASE");
//		_tick.detach();
		_tick_hold->stop();
		_releaseCb.call(_id);
		enableRiseFallCallbacks();
		return;
	}

    // En caso de evento PRESS
    if((pin_level == 1 && _level == PressIsHighLevel) || (pin_level == 0 && _level == PressIsLowLevel)){
    	DEBUG_TRACE("\r\n[PushBtn]....... EV_PRESS");
        if(_enable_ticker){
        	_tick_hold->start(_hold_us/1000);
//            _tick.attach_us(callback(this, &PushButton::isrTickCallback), _hold_us);
        }
        _pressCb.call(_id);
        enableRiseFallCallbacks();
        return;
    }

    // No debería llegar a este punto nunca, pero por si acaso, reasigna isr's
    DEBUG_TRACE("\r\n[PushBtn]....... ERR_LEVEL");
    _tick_hold->stop();
//    _tick.detach();
    enableRiseFallCallbacks();
}


//------------------------------------------------------------------------------------
void PushButton::isrTickCallback(){
	DEBUG_TRACE("\r\n[PushBtn]....... EV_HOLD");
    _holdCb.call(_id);
}


//------------------------------------------------------------------------------------
void PushButton::enableRiseFallCallbacks(){
	if(_curr_value){
		_iin->rise(NULL);
		_iin->fall(callback(this, &PushButton::isrFallCallback));
	}
	else{
		_iin->rise(callback(this, &PushButton::isrRiseCallback));
		_iin->fall(NULL);
	}
}

