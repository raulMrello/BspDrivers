/*
 * Led.cpp
 *
 *  Created on: 20/04/2015
 *      Author: raulMrello
 */

#include "Led.h"



//------------------------------------------------------------------------------------
//--- PRIVATE TYPES ------------------------------------------------------------------
//------------------------------------------------------------------------------------

#define DEBUG_TRACE(format, ...)    if(_debug){printf(format, ##__VA_ARGS__);}
#define NULL_CALLBACK               (void(*)())0
 
//------------------------------------------------------------------------------------
//-- PUBLIC METHODS IMPLEMENTATION ---------------------------------------------------
//------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------
Led::Led(PinName led, LedType type, LedLogicLevel level, uint32_t period_ms){
    // Crea objeto
    _id = (uint32_t)led;
    _debug = false;
    _type = type;
    _period_ms = period_ms;
    _max_intensity = 1.0f;
    _min_intensity = 0;
    
    if(_type == LedOnOffType){
        _out_01 = new DigitalOut(led);
    }
    else{
        _out = new PwmOut(led);
        _out->period_ms(_period_ms);
    }    
    _level = level;      
    
    // Deja apagado por defecto
    off(0, 0, 0);
}


//------------------------------------------------------------------------------------
void Led::on(uint32_t ms_duration, uint8_t intensity, uint32_t ms_ramp){
    _tick_blink.detach();
    _tick_ramp.detach();
    _tick_duration.detach();    
    _istemp = false;
    // si es temporal...
    if(ms_duration > 0){
        // hace un backup del estado actual y lanza el timer
        _istemp = true;
        _bkp_stat = _stat;
        _tick_duration.attach_us(callback(this, &Led::temporalCb), (ms_duration * 1000));
    }
    _stat = LedIsOn;
    // Si no hay rampa...
    if(ms_ramp == 0){
        _action = LedGoOnEnd;
        if(_type == LedOnOffType){
            _max_intensity = convertIntensity(100);
            _intensity = _max_intensity;
            _out_01->write((uint8_t)_intensity);
        }
        else{
            _max_intensity = convertIntensity(intensity);
            _intensity = _max_intensity;
            _out->write(_intensity);        
        }
    }
    // si hay rampa, la inicia
    else{
        _action = LedGoingOn;
        _tick_ramp.attach_us(callback(this, &Led::rampOnCb), (ms_ramp * 1000));
    }    
}


//------------------------------------------------------------------------------------
void Led::off(uint32_t ms_duration, uint8_t intensity, uint32_t ms_ramp){
    _tick_blink.detach();
    _tick_ramp.detach();
    _tick_duration.detach();    
    _istemp = false;
    // si es temporal...
    if(ms_duration > 0){
        // hace un backup del estado actual y lanza el timer
        _istemp = true;
        _bkp_stat = _stat;
        _tick_duration.attach_us(callback(this, &Led::temporalCb), (ms_duration * 1000));
    }
    _stat = LedIsOff;
    // Si no hay rampa...
    if(ms_ramp == 0){        
        _action = LedGoOffEnd;
        if(_type == LedOnOffType){
            _min_intensity = convertIntensity(0);
            _intensity = _min_intensity;
            _out_01->write((uint8_t)_intensity);
        }
        else{
            _min_intensity = convertIntensity(intensity);
            _intensity = _min_intensity;
            _out->write(_intensity);        
        }
    }
    // si hay rampa, la inicia
    else{
        _action = LedGoingOff;
        _tick_ramp.attach_us(callback(this, &Led::rampOffCb), (ms_ramp * 1000));
    }    
}


//------------------------------------------------------------------------------------
void Led::blink(uint32_t ms_blink_on, uint32_t ms_blink_off, uint32_t ms_duration, uint8_t intensity_on, uint8_t intensity_off){
    // si no hay temporizaciones de On y Off, no permite la ejecución
    if(ms_blink_on == 0 && ms_blink_off == 0){
        return;
    }
    
    _tick_blink.detach();
    _tick_ramp.detach();
    _tick_duration.detach();    
    _istemp = false;
    _ms_blink_on = ms_blink_on;
    _ms_blink_off = ms_blink_off;
    // si es temporal...
    if(ms_duration > 0){
        // hace un backup del estado actual y lanza el timer
        _istemp = true;
        _bkp_stat = _stat;
        _tick_duration.attach_us(callback(this, &Led::temporalCb), (ms_duration * 1000));
    }
    _stat = LedIsBlinking;    
    _action = LedGoOnEnd;
    if(_type == LedOnOffType){
        _max_intensity = convertIntensity(100);
        _min_intensity = convertIntensity(0);
        _intensity = _max_intensity;
        _out_01->write((uint8_t)_intensity);
    }
    else{
        _max_intensity = convertIntensity(intensity_on);
        _min_intensity = convertIntensity(intensity_off);
        _intensity = _max_intensity;
        _out->write(_intensity);        
    }
    _tick_blink.attach_us(callback(this, &Led::blinkCb), (_ms_blink_on * 1000));
}


//------------------------------------------------------------------------------------
void Led::updateBlinker(uint32_t ms_blink_on, uint32_t ms_blink_off){
    _ms_blink_on = ms_blink_on;
    _ms_blink_off = ms_blink_off;
}    

 
//------------------------------------------------------------------------------------
//-- PRIVATE METHODS IMPLEMENTATION --------------------------------------------------
//------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------
void Led::rampOffCb(){
    if(_level == OnIsHighLevel){
        _intensity -= (_max_intensity/10);
        if(_intensity <= _min_intensity){
            _tick_ramp.detach();
            _intensity = _min_intensity;
            _action = LedGoOffEnd;
            _out->write(_intensity);
            return;
        }
    }
    else{
        _intensity += (_max_intensity/10);
        if(_intensity >= _max_intensity){
            _tick_ramp.detach();
            _intensity = _max_intensity;
            _action = LedGoOffEnd;
            _out->write(_intensity);
            return;
        }
    }
    _out->write(_intensity);
}


//------------------------------------------------------------------------------------
void Led::rampOnCb(){
    if(_level == OnIsHighLevel){
        _intensity += (_max_intensity/10);
        if(_intensity >= _max_intensity){
            _tick_ramp.detach();
            _intensity = _max_intensity;
            _action = LedGoOnEnd;
            _out->write(_intensity);
            return;
        }
    }
    else{
        _intensity -= (_max_intensity/10);
        if(_intensity <= _min_intensity){
            _tick_ramp.detach();
            _intensity = _min_intensity;
            _action = LedGoOnEnd;
            _out->write(_intensity);
            return;
        }
    }
    _out->write(_intensity);
}


//------------------------------------------------------------------------------------
void Led::blinkCb(){
    if(_action == LedGoOnEnd){
        _intensity = (_level == OnIsHighLevel)? _min_intensity : _max_intensity;
        _action = LedGoOffEnd;
        if(_type == LedOnOffType){
            _out_01->write((uint8_t)_intensity);
        }
        else{
            _out->write(_intensity);        
        }   
        _tick_blink.attach_us(callback(this, &Led::blinkCb), (_ms_blink_off * 1000)); 
                
    }
    else{
        _intensity = (_level == OnIsHighLevel)? _max_intensity : _min_intensity;
        _action = LedGoOnEnd;
        if(_type == LedOnOffType){
            _out_01->write((uint8_t)_intensity);
        }
        else{
            _out->write(_intensity);        
        }
        _tick_blink.attach_us(callback(this, &Led::blinkCb), (_ms_blink_on * 1000));         
        
    }
}


//------------------------------------------------------------------------------------
void Led::temporalCb(){
    _tick_blink.detach();
    _tick_ramp.detach();
    _tick_duration.detach();    
    _stat = _bkp_stat;
    if(_stat == LedIsOff){
        off(0, (uint8_t)(_min_intensity*100), 0);
    }
    else if (_stat == LedIsOn){
        on(0, (uint8_t)(_max_intensity*100), 0);
    }
    else{
        blink(_ms_blink_on, _ms_blink_off, 0, (uint8_t)(_max_intensity*100), (uint8_t)(_min_intensity*100));
    }
}


//------------------------------------------------------------------------------------
float Led::convertIntensity(uint8_t intensity){
    intensity = (intensity > 100)? 100 : intensity;
    if(_level == OnIsHighLevel){
        return(((float)intensity)/100);
    }
    else{
        return(1.0f - (((float)intensity)/100));
    }
}
