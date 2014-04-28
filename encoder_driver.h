//======================================================================================
/** \file ecoder_driver.h
 *    NEEDS USEFUL COMMENTS.
 */
//======================================================================================

// This define prevents this .H file from being included multiple times in a .CPP file
#ifndef _AVR_ENCODER_DRIVER_H_
#define _AVR_ENCODER_DRIVER_H_

#include "emstream.h"                       // Header for serial ports and devices
#include "FreeRTOS.h"                       // Header for the FreeRTOS RTOS
#include "task.h"                           // Header for FreeRTOS task functions
#include "queue.h"                          // Header for FreeRTOS queues
#include "semphr.h"                         // Header for FreeRTOS semaphores


//-------------------------------------------------------------------------------------
/** \brief This class runs the encoder on the h-bridge chip.
 *  \details This class takes in several ports and their inputs and masks for a 
 *  specific motherboard, as well as a serial port.
 */

class encoder_driver {
   protected:
      /// The encoder driver class uses this pointer print to the serial port.
      emstream* ptr_to_serial;

   public:
      /// TODO: add commments.
      encoder_driver (emstream *p_serial_port, uint8_t bit, uint8_t trigger);
      int32_t get_count (void);
      void zero (void);
      void set_position (int32_t);

}; // end of class encoder driver


#endif // _AVR_ENCODER_DRIVER_H_
