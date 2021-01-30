#ifndef _AIRPORT_C_
#define _AIRPORT_C_

#include <stdlib.h>
#include <stdio.h>
#include "airport.h"
#include "flight.h"
#include "aircraft.h"


 /** Opaque airport type. */
 struct airport{

        const char *icao_code;
        int flight_size;
        unsigned int refs;
        struct flight* flights[];
 };


 /** Create a new airport. */
 struct airport* ap_create(const char *icao_code){
        printf("some stuff");
        struct airport* new_port = malloc(sizeof(struct airport));
        
        char* temp = malloc(4*sizeof(char));

        *temp = *icao_code;
        for(int i = 1; i < 4; i++){
            temp = temp + 1;
            *temp = *(icao_code + i);

        }

        new_port->icao_code = temp - 3;
        new_port->flight_size = 0;
        new_port->refs = 1;


        return new_port;

 }

 /** Retrieve a airport's ICAO code. */
 const char* ap_code(const struct airport* port){
            return port->icao_code;


 }

 /**
  * Add a flight to an airport's list of flights.
  *
  * @returns     0 on success, -1 if flight is invalid (NULL pointer or does
  *              not include this airport as a stop) or on failure to
  *              allocate memory
  */
 int     ap_add_flight(struct airport* port, struct flight* new_flight){
            
                
/*
                bool validStop = false;


                for(int i = 0; i < new_flight->f_stop_count; i++){
                    if(*(new_flight->f_stops+i) == port)
                        validStop = true;

                }


            if(validStop){
                port->flight_size++;
                port = realloc(port, sizeof(struct airport) + port->flight_size);
                port->flights[port->flight_size] = new_flight;


                return 0;

            }
*/
            return -1;


 }

 /**
  * Copy pointers to flights into a caller-provided array.
  *
  * @param   ap      The airport to query
  * @param   fpp     Array of flight pointers to copy flight data into
  * @param   n       Number of elements in the passed-in array. If too small
  *                  to hold all of the flights, this will be set to the minimum
  *                  size required.
  */
 void        ap_flights(struct airport *ap, struct flight **fpp, size_t *n){

                if(*n < ap->flight_size){
                    *n = ap->flight_size;
                }

                struct flight* flights_copy = malloc(*n*sizeof(struct flight));
                void* original_spot = flights_copy;
                for(int i = 0; i < ap->flight_size; i++){
                    flights_copy = flights_copy + 1;
                    
                    flights_copy = ap->flights[i];

                }
                
                flights_copy = original_spot;

                fpp = &flights_copy;




 }

 /** Increment a airport's refcount. */
 void        ap_hold(struct airport* port){

            port->refs++;


 };

 /** Decrement a airport's refcount (optionally freeing it). */
 void        ap_release(struct airport* port){

                if(port->refs > 0){
                    port->refs--;
                    return;
                }

                free(port);

 }

 /** Retrieve the current reference count of a airport. */
 unsigned int    ap_refcount(const struct airport* port){

                return port->refs;


 };

#endif
