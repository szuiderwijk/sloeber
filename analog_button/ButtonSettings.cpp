/*
 * ButtonSettings.cpp
 *
 *  Created on: 11 Feb 2018
 *      Author: stephanzuiderwijk
 */

#include "ButtonSettings.h"

ButtonSettings::ButtonSettings(int low, int height, int ledPin) {
	this->height = height;
	this->low = low;
	this->ledPin = ledPin;

}

ButtonSettings::~ButtonSettings() {
	// TODO Auto-generated destructor stub
}

