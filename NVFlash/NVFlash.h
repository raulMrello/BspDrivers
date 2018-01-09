/*
 * NVFlash.h
 *
 *  Created on: Nov 2017
 *      Author: raulMrello
 *
 *  NVFlash es el m�dulo C++ que proporciona acceso a zonas de memoria FLASH para almacenar datos no vol�tiles
 *  como configuraciones, par�metros de calibraci�n, etc...
 *  Por regla general, son zonas de memoria situadas al final de la flash de programa
 * 
 */
 
 
#ifndef _NVFlash_H
#define _NVFlash_H


#include "mbed.h"  
  
  
//---------------------------------------------------------------------------------
//- class NVFlash ----------------------------------------------------------
//---------------------------------------------------------------------------------


class NVFlash {

public:
        
    /** Lista de erores devueltos por el m�dulo */
    enum ErrorResult{
        Success,
        Locked,
        Busy,
        OutOfRange,
        InvalidArguments,
        ReadError,
        WriteError,
    };
    
    /** init()
     *  Inicializa reservando una zona de la flash
     *  @param base_addr P�ginas a reservar
     *  @param size Tama�o de p�gina.
     *  @return C�digo de error
     */
    static ErrorResult init(uint32_t base_addr = 0x0803F800, uint32_t size = 2048);
    
    
    /** getPageSize()
     *  Obtiene el tama�o de una p�gina
     *  @return Tama�o de p�gina
     */
    static uint32_t getPageSize() { return _page_size; }


    /** readPage()
     *  Lee una p�gina
     *  @param page P�gina a leer
     *  @param dest Recibe los datos le�dos
     *  @param size Tama�o a leer.
     *  @return C�digo de error
     */
    static ErrorResult readPage(uint8_t page, uint32_t* dest, uint32_t size = 2048);


    /** writePage()
     *  Escribe datos a una posici�n de memoria
     *  @param page P�gina a programar
     *  @param data Datos a escribir
     *  @return C�digo de error
     */
    static ErrorResult writePage(uint8_t page, uint32_t* data);


    /** erasePage()
     *  Borra una p�gina
     *  @param page P�gina a borrar
     */
    static ErrorResult erasePage(uint8_t page);

      
protected:

    static FlashIAP _flash;         /// Controlador de memoria
    static uint32_t _start_addr;    /// Direcci�n de origen
    static uint8_t _num_pages;      /// N�mero de p�ginas reservadas
    static uint32_t _page_size;     /// Tama�o de la p�gina

};

#endif  /** _NVFlash_H */

