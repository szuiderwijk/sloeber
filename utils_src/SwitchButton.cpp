/*
 * SwitchButton.cpp
 *
 *  Created on: 3 Feb 2018
 *      Author: stephanzuiderwijk
 */

#include "SwitchButton.h"


SwitchButton::SwitchButton() {
	// TODO Auto-generated constructor stub
	this->switchOn = false;
}

SwitchButton::~SwitchButton() {
	// TODO Auto-generated destructor stub
}

void SwitchButton::buttonClicked() {
	this->switchOn = !this->switchOn;
}

bool SwitchButton::isOn() {
	return this->switchOn;
}
