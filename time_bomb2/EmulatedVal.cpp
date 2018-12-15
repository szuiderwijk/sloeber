/*
 * EmulatedVal.cpp
 *
 *  Created on: 15 Oct 2018
 *      Author: stephanzuiderwijk
 */

#include "EmulatedVal.h"


EmulatedVal::EmulatedVal(int outputValue, int min, int max) {
	this->outputValue = outputValue;
	this->min = min;
	this->max = max;
}

EmulatedVal::~EmulatedVal() {
	// TODO Auto-generated destructor stub
}

