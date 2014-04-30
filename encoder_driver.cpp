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

encoder_driver::encoder_driver (emstream *p_serial_port, uint8_t bit, uint8_t trigger) {  

   ptr_to_serial = p_serial_port;
   count->put(0);
   error->put(0);
   
   sei ();
   PORTE |= 1 << bit;
   EICRB = trigger;
   EIMSK |= 1 << bit;

   DBG(ptr_to_serial, "Encoder driver constructor OK" << endl);
}

int32_t encoder_driver::get_count (void) {
   return count->get();
}

void encoder_driver::zero (void) {
   count->put(0);
}

void encoder_driver::set_position (int32_t position) {
   count->put(position);
}

ISR (INT4_vect) {
   static uint8_t lastA = 0, lastB = 0;
   uint8_t currentA, currentB;

   currentA = PINE & (1 << PE4);
   currentB = PINE & (1 << PE5);

   switch(currentB | currentA){
      case 0b000000:
         switch(lastB | lastA){
            case 0b010000:
               count->put(count->get() - 1); //reverse
               break;
            case 0b100000:
               count->put(count->get() + 1); //forward
               break;
            default:
               error->put(error->get() + 1);
               break;
         }
         break;
      case 0b010000: 
         switch(lastB | lastA){
            case 0b000000:
               count->put(count->get() + 1); //forward
               break;
            case 0b110000:
               count->put(count->get() - 1); //reverse
               break;
            default:
               error->put(error->get() + 1);
               break;
         }
         break;
      case 0b100000: 
         switch(lastB | lastA){
            case 0b000000:
               count->put(count->get() - 1); //reverse
               break;
            case 0b110000:
               count->put(count->get() + 1); //forward
               break;
            default:
               error->put(error->get() + 1);
               break;
         }
         break;
      case 0b110000: 
         switch(lastB | lastA){
            case 0b010000:
               count->put(count->get() + 1); //forward
               break;
            case 0b100000:
               count->put(count->get() - 1); //reverse
               break;
            default:
               error->put(error->get() + 1);
               break;
         }
         break;
   }
   lastA = currentA;
   lastB = currentB;
}

ISR (INT5_vect, ISR_ALIASOF(INT4_vect));