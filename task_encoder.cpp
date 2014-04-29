//**************************************************************************************
/** \file task_encoder.cpp
 *    TODO: fix comment.*/
//**************************************************************************************

#include "task_encoder.h"                   // Header for this encoder


//-------------------------------------------------------------------------------------

task_encoder::task_encoder (const char* a_name,
                            unsigned portBASE_TYPE a_priority,
                            size_t a_stack_size,
                            emstream* p_ser_dev,
                            uint8_t bit,
                            uint8_t trigger)
   : frt_task (a_name, a_priority, a_stack_size, p_ser_dev) {

    encoder = new encoder_driver(p_ser_dev, bit, trigger);
    runs = 0;
}


//-------------------------------------------------------------------------------------
/** This method is called once by the RTOS scheduler. Each time around the for (;;)
 *  loop, it reads the A/D converter and change the selected motors speed. Each loop
 *  also check the two additional buttons, which control the brakes of the individual
 *  motors.
 */

void task_encoder::run (void) {

   for (;;) {
     *print_ser_queue << PMS ("Error: ") << error->get() << PMS ("Count: ") << encoder->get_count() << endl;

     delay (100);
     runs++;
   }
}

/**
 * Self explanitory. This function takes in a serial port in the form of an
 * address and passes it straight to the parent constructor so it can print
 * out the status. Also prints out the number of runs
 */

void task_encoder::print_status (emstream& ser_thing) {
   // Call the parent task's printing function first
   frt_task::print_status (ser_thing);

   // Now add the additional data
   ser_thing << "\t " << runs << PMS (" runs");
}

