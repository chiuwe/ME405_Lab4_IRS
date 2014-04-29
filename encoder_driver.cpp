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
   PIND |= 1 << bit;
   EICRB = trigger;
   EIMSK |= 1 << bit;
   DDRD |= 0b11;

   DBG(ptr_to_serial, "Encoder driver constructor OK" << endl);
}

void encoder_driver::print_info(void){
   DBG(ptr_to_serial, count->get() << endl);
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

/*2000 count per full rotation*/
ISR (INT4_vect) {
   static uint8_t lastA = 0, lastB = 0;
   uint8_t currentA, currentB;

   currentA = PORTD & 1;            //TODO PORTD is not how you read the value from the sensor
   currentB = (PORTD & (1 << 1));


   switch(currentB | currentA){
      case 0b00: 
         switch(lastB | lastA){
            case 0b00:
               error->put(error->get() + 1);
               break;
            case 0b01:
               count->put(count->get() - 1); //reverse
               break;
            case 0b10:
               count->put(count->get() + 1); //forward
               break;
            case 0b11:
               error->put(error->get() + 1);
               break;
            default:
               break;
         }
         break;
      case 0b01: 
         switch(lastB | lastA){
            case 0b00:
               count->put(count->get() + 1); //forward
               break;
            case 0b01:
               error->put(error->get() + 1);
               break;
            case 0b10:
               error->put(error->get() + 1);
               break;
            case 0b11:
               count->put(count->get() - 1); //reverse
               break;
            default:
               break;
         }
         break;
      case 0b10: 
         switch(lastB | lastA){
            case 0b00:
               count->put(count->get() - 1); //reverse
               break;
            case 0b01:
               error->put(error->get() + 1);
               break;
            case 0b10:
               error->put(error->get() + 1);
               break;
            case 0b11:
               count->put(count->get() + 1); //forward
               break;
            default:
               break;
         }
         break;
      case 0b11: 
         switch(lastB | lastA){
            case 0b00:
               error->put(error->get() + 1);
               break;
            case 0b01:
               count->put(count->get() + 1); //forward
               break;
            case 0b10:
               count->put(count->get() - 1); //reverse
               break;
            case 0b11:
               error->put(error->get() + 1);
               break;
            default:
               break;
         }
         break;
      default:
         break;
   }
   lastA = currentA;
   lastB = currentB;
}

ISR (INT5_vect, ISR_ALIASOF(INT4_vect));