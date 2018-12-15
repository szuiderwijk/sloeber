/*
 * ButtonSettings.h
 *
 *  Created on: 11 Feb 2018
 *      Author: stephanzuiderwijk
 */

#ifndef BUTTONSETTINGS_H_
#define BUTTONSETTINGS_H_

#include "SwitchButton.h"

class ButtonSettings {
public:
	SwitchButton button;
	int low;
	int height;
	int ledPin;

	ButtonSettings(int low, int height, int ledPin);
	virtual ~ButtonSettings();
};

#endif /* BUTTONSETTINGS_H_ */
