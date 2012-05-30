/****************************************************
 * The Pneumatics Class developed by Tanay Nathan	*
 * of Team 1351. Used in conjunction with the 		*
 * PneumaticController class to create an easy way	*
 * to keep track of the Pneumatic systems for FRC.	*
 * Last edited on Sunday, April 15, 2012 to add the *
 * documentation, GetState(int i) function and the 	*
 * StopCompressor() function.						*
 ****************************************************/

#include "Pneumatics.h"

/****************************************************
 * Constructor for the system						*
 * Precondition: Five integers as parameters: first	*
 * one is the channel of the Pneumatics system.	The	*
 * following two integers indicate the starting and	*
 * ending points for the ports of the system. It	*
 * takes the ports and puts them into a vector of	*
 * PneumaticController objects. The ports of the 	*
 * solenoids for the same piston must be next to 	*
 * each other in the order of compression then		*
 * decompression. Last two integers are for the 	*
 * channel and port of the compressor.				*
 * -------------------------------------------------*
 * Postcondition: A constructed Pneumatics object 	*
 * with a vector of the PneumaticController objects	*
 * and the compressor object is created and started.*
 * **************************************************/

Pneumatics::Pneumatics(int port1, int start, int end, int compressch, int compressp) {
	for (int i = start; i < end - 1; i += 2) {
		p->push_back(PneumaticController(port1, i, i + 1));
	}
	max = p->size();
	compress = new Compressor(compressch, compressp);
	compress->Start();
}

/****************************************************
 * Destructor for the Pneumatics					*
 * Precondition: no parameters, constructed object	*
 * -------------------------------------------------*
 * Postcondition: Decompressed all the solenoids	*
 * first, the clears the vector. Stops the 			*
 * compressor and deletes the memory allocations for*
 * the vector and the compressor.					*
 ****************************************************/

Pneumatics::~Pneumatics() {
	for (int i = 0; i < max; i++) {
		GetSolenoid(i, 0)->Set(false);
		GetSolenoid(i, 1)->Set(false);
	}
	p->clear();
	compress->Stop();
	delete p;
	delete compress;
}

/****************************************************
 * Extension function for the system				*
 * Precondition: An integer representing the index	*
 * of the PneumaticController in the vector.		*
 * -------------------------------------------------*
 * Postcondition: The selected piston is extended 	*
 * using the PneumaticController class's function	*
 * Extend.											*
 ****************************************************/

void Pneumatics::Extend(int num) {
	if (num > 0 && num < max)
		p->at(num).Extend();
}

/****************************************************
 * Retraction function for the system				*
 * Precondition: An integer representing the index	*
 * of the PneumaticController in the vector.		*
 * -------------------------------------------------*
 * Postcondition: The selected piston is retracted	*
 * using the PneumaticController class's function	*
 * Retract.											*
 ****************************************************/

void Pneumatics::Retract(int num) {
	if (num > 0 && num < max)
		p->at(num).Retract();
}

/****************************************************
 * Accessor function for the system					*
 * Precondition: An integer representing the index	*
 * of the PneumaticController in the vector.		*
 * -------------------------------------------------*
 * Postcondition: The selected PneumaticController	*
 * object is returned to the user from the vector.	*  
 ****************************************************/

PneumaticController Pneumatics::GetPneumaticController(int i) {
	return p->at(i);
}

/****************************************************
 * Accessor function for the system					*
 * Precondition: Two integers representing the index*
 * of the PneumaticController in the vector and one	*
 * of the two Solenoids in a controller.			*
 * -------------------------------------------------*
 * Postcondition: The selected Solenoid address is	*
 * returned to the user from the PneumaticController*
 * in the vector.									*
 ****************************************************/

Solenoid* Pneumatics::GetSolenoid(int i, int i2) {
	return p->at(i).GetSolenoid(i2);
}

/****************************************************
 * Accessor function for the system					*
 * Precondition: An integer representing the index	*
 * of the PneumaticController in the vector.		*
 * -------------------------------------------------*
 * Postcondition: Returns the state of the piston to*
 * the user at the selected index.					*
 ****************************************************/

bool Pneumatics::GetState(int i) {
	return p->at(i).GetState();
}

/****************************************************
 * Emergency compressor stop function				*
 * Precondition: no parameters, constructed object.	*
 * -------------------------------------------------*
 * Postcondition: Stops the compressor from working	*
 * until the Pneumatics is reconstructed.			*
 ****************************************************/

void Pneumatics::StopCompressor() {
	compress->Stop();
}
