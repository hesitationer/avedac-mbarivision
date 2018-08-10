//=============================================================================
// Name        : Creature.H
// Author      : Mayra Ochoa & Raymond Esteybar
// Version     :
// Copyright   :
// Description : Contains animal's Name, Probability, & Bounding Box Dimensions
//=============================================================================

#ifndef SRC_CREATURE_H_
#define SRC_CREATURE_H_

#include <string>

#include "Image/Rectangle.H"

class Creature {
public:
	Creature();

	// Getters
	std::string getName()const 		{return itsName;}
	float getProbability()const 	{return itsProbability;}
	Rectangle getDimensions()const	{return itsDimensions;}

	// Setters
	void setName(const std::string& newName) 			{itsName = newName;}
	void setProbability(const float& newProbability) 	{itsProbability = newProbability;}
	void setDimensions(const Rectangle& newDimensions)	{itsDimensions = newDimensions;}

private:
	std::string itsName;
	float itsProbability;
	Rectangle itsDimensions;
};

#endif /* SRC_CREATURE_H_ */
