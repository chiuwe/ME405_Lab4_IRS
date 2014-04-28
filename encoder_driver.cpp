//*************************************************************************************
/** \file encoder_driver.cpp
 *    NEED USEFUL COMMENTS.
 */
//*************************************************************************************

#include <stdlib.h>                         // Include standard library header files
#include <avr/io.h>

#include "rs232int.h"                       // Include header for serial port class
#include "encoder_driver.h"                 // Include header for the encoder class

//-------------------------------------------------------------------------------------
/** \brief This constructor sets up a encoder driver. 
 *  \details \b Details: TO BE FILLED.
 */

 int32_t count = 0;

encoder_driver::encoder_driver (emstream *p_serial_port, uint8_t bit, uint8_t trigger) {  

   ptr_to_serial = p_serial_port;
   count = 0;
   
   sei ();
   PORTE |= 1 << bit;
   EICRB = trigger;
   EIMSK |= 1 << bit;


   DBG(ptr_to_serial, "Encoder driver constructor OK" << endl);
}

int32_t encoder_driver::get_count (void) {
   DBG(ptr_to_serial, count << endl);
   return count;
}

void encoder_driver::zero (void) {
   count = 0;
}

void encoder_driver::set_position (int32_t position) {
   count = position;
}

ISR (INT5_vect) {
   count++;
}

ISR (INT4_vect) {
   count++;
}