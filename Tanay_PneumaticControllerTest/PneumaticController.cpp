/****************************************************
 * The PneumaticController class developed by Tanay	*
 * Nathan of Team 1351. Used to make the task of 	*
 * keeping track of compressing pistons easier.		*
 * Last edited on Sunday, April 15, 2012 to add the	*
 * documentation and GetState() function.			*
 ****************************************************/

#include "PneumaticController.h"

/****************************************************
 * Constructor for the Controller					*
 * Precondition: Three integers, one representing 	*
 * the channel of the ports and the other two are 	*
 * to show the ports of the Solenoids in.			*
 * The first Solenoid is the compressing Solenoid,	*
 * while the second Solenoid the decompression.		*
 * -------------------------------------------------*
 * Postcondition: A PneumaticController object is 	*
 * constructed and is not compressed at the start.	*
 * **************************************************/

PneumaticController::PneumaticController(int port1, int port2, int port3) {
	s->push_back(new Solenoid (port1, port2));
	s->push_back(new Solenoid (port1, port3));
	s->at(0)->Set(false);
	s->at(1)->Set(false);
}

/****************************************************
 * Destructor for the controller					*
 * Precondition: No parameters, constructed object.	*
 * -------------------------------------------------*
 * Postcondition: Deletes the contents of the object*
 * and terminates the instances of the controller	*
 * including the vector of Solenoids.				*
 * **************************************************/

PneumaticController::~PneumaticController() {
	delete s->at(0);
	delete s->at(1);
	delete s;
}

/****************************************************
 * Extension function for the Controller			*
 * Precondition: No parameters, constructed object.	*
 * -------------------------------------------------*
 * Postcondition: Piston extends, if constructed 	*
 * properly. Sets the compression Solenoid to true, *
 * which compresses the piston, and sets the		*
 * decompression piston to false and extends it out.* 
 ****************************************************/

void PneumaticController::Extend() {
	s->at(0)->Set(true);
	s->at(1)->Set(false);
}

/****************************************************
 * Retraction function for the Controller			*
 * Precondition: No parameters, constructed object.	*
 * -------------------------------------------------*
 * Postcondition: Piston retracts, if constructed 	*
 * properly. Sets the decompression piston to true	*
 * and the compression piston to false, which 		*
 * decompresses the piston into a state of			*
 * retraction.
 ****************************************************/

void PneumaticController::Retract() {
	s->at(1)->Set(true);
	s->at(0)->Set(false);
}

/****************************************************
 * An accessor function								*
 * Precondition: An integer from 0 to 1 indicating	*
 * a solenoid memory allocation to be returned from *
 * the function.									*
 * -------------------------------------------------*
 * Postcondition: The Solenoid memory allocation is *
 * returned to the user for use in the program.		*
 ****************************************************/

Solenoid* PneumaticController::GetSolenoid(int i) {
	if (i > 1) { i = 1; }
	if (i < 0) { i = 0; }
	return s->at(i);

}

/****************************************************
 * An accessor function								*
 * Precondition: No parameters, constructed object.	*
 * -------------------------------------------------*
 * Postcondition: Returns a boolean data value to 	*
 * the user to indicate the state of the piston:	*
 * true means compressed and extended, while false 	*
 * means that it is decompressed. This means that	*
 * the solenoids are either both false or the 		*
 * decompression solenoid is false.					*
 ****************************************************/

bool PneumaticController::GetState() {
	if (s->at(0)->Get() && !s->at(1)->Get())
		return true;
	return false;
}
