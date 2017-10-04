/*
 * Zerocross.cpp
 *
 *  Created on: 20/04/2015
 *      Author: raulMrello
 */

#include "Zerocross.h"



//------------------------------------------------------------------------------------
//--- PRIVATE TYPES ------------------------------------------------------------------
//------------------------------------------------------------------------------------


#define NULL_CALLBACK   (void(*)(Zerocross::LogicLevel))0
 
//------------------------------------------------------------------------------------
//-- PUBLIC METHODS IMPLEMENTATION ---------------------------------------------------
//------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------
Zerocross::Zerocross(PinName gpio_zc){
    // Crea objeto
    _iizc = 0;
    if(gpio_zc != NC){
        _iizc = new InterruptIn(gpio_zc);
        
        // Asocia manejadores de interrupción del pin de entrada
        _iizc->rise(callback((void(*)())0));
        _iizc->fall(callback((void(*)())0));
    }
    // Desactiva las callbacks de notificación
    _isrRiseCb = callback(NULL_CALLBACK);
    _isrFallCb = callback(NULL_CALLBACK);
}


//------------------------------------------------------------------------------------
void Zerocross::enableEvents(LogicLevel level, Callback<void(LogicLevel)>isrZcCb){
    if(level == EdgeActiveIsFall){
        _isrFallCb = isrZcCb;
        if(_iizc){
            _iizc->fall(callback(this, &Zerocross::isrFallCallback));
        }
    }
    if(level == EdgeActiveIsRise){
        _isrRiseCb = isrZcCb;
        if(_iizc){
            _iizc->rise(callback(this, &Zerocross::isrRiseCallback));
        }
    }
    if(level == EdgeActiveAreBoth){
        _isrFallCb = isrZcCb;
        _isrRiseCb = isrZcCb;
        if(_iizc){
            _iizc->fall(callback(this, &Zerocross::isrFallCallback));
            _iizc->rise(callback(this, &Zerocross::isrRiseCallback));
        }
    }
}


//------------------------------------------------------------------------------------
void Zerocross::disableEvents(LogicLevel level){
    if(level == EdgeActiveIsFall){
        if(_iizc){
            _iizc->fall(callback((void(*)())0));
        }
        _isrFallCb = callback(NULL_CALLBACK);
    }
    if(level == EdgeActiveIsRise){
        if(_iizc){
            _iizc->rise(callback((void(*)())0));
        }
        _isrRiseCb = callback(NULL_CALLBACK);
    }
    if(level == EdgeActiveAreBoth){
        if(_iizc){
            _iizc->fall(callback((void(*)())0));            
            _iizc->fall(callback((void(*)())0));
        }
        _isrFallCb = callback(NULL_CALLBACK);
        _isrRiseCb = callback(NULL_CALLBACK);
    }
}


 
//------------------------------------------------------------------------------------
//-- PRIVATE METHODS IMPLEMENTATION --------------------------------------------------
//------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------
void Zerocross::isrRiseCallback(){
    _isrRiseCb.call(EdgeActiveIsRise);
}


//------------------------------------------------------------------------------------
void Zerocross::isrFallCallback(){
    _isrFallCb.call(EdgeActiveIsFall);
}

