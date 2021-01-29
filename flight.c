
#ifndef _FLIGHT_C_
#define _FLIGHT_C_



#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "flight.h"
#include "airport.h"
#include "aircraft.h"

/**
 * Create a new flight.
 *
 * The airline and flight number will be copied into the new structure.
 * Stops (stored in a null-terminated array) are copied in without referencing
 * the airports, which are assumed to live at least as long as the flight.
 */
struct flight*	flight_create(const char *airline, uint16_t number,
                              struct airport *stops[]){

                struct flight* new_flight = malloc(sizeof(struct flight));
                for(int i = 0; i < 3; i++){

                    new_flight->f_airline[i] = *airline + i;

                }


                int i = 0;
                void* original_addr = stops[0];
                while(stops[i] != NULL){
                
                    original_addr = original_addr + i;
                    original_addr = stops[i];
                
                    
                    i++;
                }

                

                new_flight->f_number = number;
                new_flight->f_stops = original_addr - i;

                return new_flight;
}

/**
 * Release a flight.
 *
 * Takes no action when passed a NULL flight.
 */
void		flight_free(struct flight* this_flight){

                if(this_flight != NULL){
                    free(this_flight);
                }


}

/**
 * Change (or set) a flight's aircraft.
 *
 * This takes a reference to the aircraft and stores it. Any existing
 * equipment's refcount is decremented. A flight can have its equipment cleared
 * by passing NULL to this function.
 */
void		flight_change_equipment(struct flight* this_flight, struct aircraft* craft){

                ac_release(this_flight->f_aircraft);
                this_flight->f_aircraft = craft;
                ac_hold(this_flight->f_aircraft);
                



}

/**
 * Construct a human-readable flight number.
 *
 * For example, a flight that was created with airline "AC\0\0" and number 860
 * would have a human-readable flight number "AC860". Returns a
 * dynamically-allocated string that the caller is responsible for freeing.
 */
char*		flight_code(const struct flight* this_flight){

            char* mem = malloc(5*sizeof(char));


            int i = 0;
            char* temp_addr = mem;
            while(this_flight->f_airline[i] != '\0'){
                temp_addr = temp_addr + 1;
                *temp_addr = this_flight->f_airline[i];
                i++;           

            }

            char* numChars = mem + i;
            sprintf(numChars, "%d", this_flight->f_number);



            return mem;                                                


}

#endif
