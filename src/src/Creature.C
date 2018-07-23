/*
 * Creature.C
 *
 *  Created on: Jul 19, 2018
 *      Author: Gray
 */

#include "Creature.H"

Creature::Creature() {
	itsName = "";
	itsProbability = 0;
	itsDimensions = Rectangle::tlbrI(0, 0, 0, 0);
}

Creature::~Creature() {
	// TODO Auto-generated destructor stub
}

