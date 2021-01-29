



#ifndef _AIRCRAFT_C_
#define _AIRCRAFT_C_


#include <stdlib.h>

#include "aircraft.h"
#include "airport.h"
#include "flight.h"
#include "main.c"






/** A type that represents a single aircraft. */

struct aircraft{
	enum aircraft_kind kind;
	const char* reg;
    unsigned int refs;
		



};






/**
 * Create a new aircraft object.
 *
 * @param   kind       kind of aircraft
 * @param   reg        registration code, of form "X-XXXX" (e.g., "C-ABCD")
 */
struct aircraft*	ac_create(enum aircraft_kind kind, const char *reg){

				struct aircraft* new_craft = malloc(sizeof(struct aircraft));
				new_craft->kind = kind;
				new_craft->reg = reg;
                new_craft->refs = 0;
                

};

/** Increase an aircraft's reference count. */
void			ac_hold(struct aircraft *craft){


                    craft ->refs ++;


                
};

/** Decrease and possibly free an aircraft. */
void			ac_release(struct aircraft *craft){

                    craft->refs--;
                    if(craft->refs = 0){
                        free(craft);
                    }


};




/** Retrieve an aircraft's refcount. */
unsigned int		ac_refcount(const struct aircraft *craft){


            return craft->refs;

};

/** Retrieve an aircraft's type (fixed-wing, rotary, etc.). */
enum aircraft_kind	ac_kind(const struct aircraft *craft){

            return craft->kind;

};

/**
 * Retrieve an aircraft's registration code.
 *
 * This string is owned by the structure, not the caller, and should not be
 * used beyond the lifetime of the aircraft object.
 */
const char*		ac_registration(const struct aircraft *craft){

                craft->reg;



};



#endif











