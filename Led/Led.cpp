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


#define NULL_CALLBACK   (void(*)())0
 
//------------------------------------------------------------------------------------
//-- PUBLIC METHODS IMPLEMENTATION ---------------------------------------------------
//------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------
Led::Led(PinName led, LedType type, LedLogicLevel level, uint32_t period_ms){
    // Crea objeto
    _type = type;
    _period_ms = period_ms;
    
    if(_type == LedOnOffType){
        _out_01 = new DigitalOut(led);
    }
    else{
        _out = new PwmOut(led);
        _out->period_ms(_period_ms);
    }    
    _level = level;    
    _temp_state = false;
    
    // Deja apagado por defecto
    stop();
}


//------------------------------------------------------------------------------------
void Led::start(float intensity, uint32_t ms_blink_on, uint32_t ms_blink_off, uint32_t ms_ramp){
    _max_intensity = (intensity > 1.0f)? 1.0f : intensity;
    _ms_blink_on = ms_blink_on;
    _ms_blink_off = ms_blink_off;
    _ms_ramp = ms_ramp;
    if(_type == LedOnOffType){
        _ms_ramp = 0;
        _max_intensity = 1.0f;
    }
    
    // encendido inmediato
    if(ms_ramp == 0){
        _tick.detach();
        if(_level == OnIsHighLevel){
            _intensity = _max_intensity;
        }
        else{
            _intensity = 1.0f - _max_intensity;            
        }   
        if(_type == LedOnOffType){
            _out_01->write((uint8_t)_intensity);
        }
        else{
            _out->write(_intensity);        
        }
        _stat = LedIsOn;  
        if(_ms_blink_on && _ms_blink_off){
            _tick.attach_us(callback(this, &Led::blinkCb), (_ms_blink_on * 1000)); 
        }
    }
    else{
        _stat = LedIsGoingOn;  
        _tick.attach_us(callback(this, &Led::rampOnCb), (ms_ramp * 1000)/10);
    }
}


//------------------------------------------------------------------------------------
void Led::stop(uint32_t ms_ramp){
    if(_type == LedOnOffType){
        _ms_ramp = 0;
        _max_intensity = 1.0f;
    }
    if(!ms_ramp){
        _tick.detach();
        if(_level == OnIsHighLevel){
            _intensity = 0;
        }
        else{
            _intensity = _max_intensity;            
        }   
        if(_type == LedOnOffType){
             _out_01->write((uint8_t)_intensity);
        }
        else{
            _out->write(_intensity);        
        }
        _stat = LedIsOff;
        return;
    }
    _stat = LedIsGoingOff;
    _tick.attach_us(callback(this, &Led::rampOffCb), (ms_ramp * 1000)/10);    
}


//------------------------------------------------------------------------------------
void Led::updateBlinker(uint32_t ms_blink_on, uint32_t ms_blink_off){
    _ms_blink_on = ms_blink_on;
    _ms_blink_off = ms_blink_off;
}    


//------------------------------------------------------------------------------------
void Led::temporalState(float intensity, uint32_t ms_temp){
    _tick.detach();
    _temp_state = true;    
    
    if(_level != OnIsHighLevel){
        intensity = 1.0f - intensity;            
    }   
    intensity = (intensity > 1.0f)? 1.0f : intensity;
    intensity = (intensity < 0)? 0 : intensity;  
    intensity = (_type == LedOnOffType && intensity > 0)? 1.0f : intensity;
    if(_type == LedOnOffType){
        _out_01->write((uint8_t)intensity);
    }
    else{
        _out->write(intensity);        
    }
    _tick.attach_us(callback(this, &Led::temporalCb), (ms_temp * 1000));     
}    

 
//------------------------------------------------------------------------------------
//-- PRIVATE METHODS IMPLEMENTATION --------------------------------------------------
//------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------
void Led::rampOffCb(){
    if(_level == OnIsHighLevel){
        _intensity -= (_max_intensity/10);
        if(_intensity < 0){
            _tick.detach();
            _intensity = 0;
            _stat = LedIsOff;
            _out->write(_intensity);
            return;
        }
    }
    else{
        _intensity += (_max_intensity/10);
        if(_intensity > _max_intensity){
            _tick.detach();
            _intensity = _max_intensity;
            _stat = LedIsOff;
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
        if(_intensity > _max_intensity){
            _tick.detach();
            _intensity = _max_intensity;
            _stat = LedIsOn;
            _out->write(_intensity);
            if(_ms_blink_on && _ms_blink_off){
                _tick.attach_us(callback(this, &Led::blinkCb), (_ms_blink_on * 1000)); 
            }
            return;
        }
    }
    else{
        _intensity -= (_max_intensity/10);
        if(_intensity < 0){
            _tick.detach();
            _intensity = 0;
            _stat = LedIsOn;
            _out->write(_intensity);
            if(_ms_blink_on && _ms_blink_off){
                _tick.attach_us(callback(this, &Led::blinkCb), (_ms_blink_on * 1000)); 
            }
            return;
        }
    }
    _out->write(_intensity);
}


//------------------------------------------------------------------------------------
void Led::blinkCb(){
    if(_type == LedOnOffType){
        _ms_ramp = 0;
        _max_intensity = 1.0f;
    }
    
    if(_stat == LedIsOn){
        if(_level == OnIsHighLevel){
            _intensity = 0;
        }
        else{
            _intensity = _max_intensity;
        }
        _stat = LedIsOff;
        if(_type == LedOnOffType){
            _out_01->write((uint8_t)_intensity);
        }
        else{
            _out->write(_intensity);        
        }
        if(_ms_blink_on && _ms_blink_off){
            _tick.attach_us(callback(this, &Led::blinkCb), (_ms_blink_off * 1000)); 
        }
    }
    else{
        if(_level == OnIsHighLevel){
            _intensity = _max_intensity;
        }
        else{
            _intensity = 0;
        }
        _stat = LedIsOn;
        if(_type == LedOnOffType){
            _out_01->write((uint8_t)_intensity);
        }
        else{
            _out->write(_intensity);        
        }
        if(_ms_blink_on && _ms_blink_off){
            _tick.attach_us(callback(this, &Led::blinkCb), (_ms_blink_on * 1000)); 
        }
    }
}


//------------------------------------------------------------------------------------
void Led::temporalCb(){
    _temp_state = false;
    _tick.detach();
    start(_max_intensity, _ms_blink_on, _ms_blink_off, _ms_ramp);
}

