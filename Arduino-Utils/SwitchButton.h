/*
 * SwitchButton.h
 *
 *  Created on: 3 Feb 2018
 *      Author: stephanzuiderwijk
 */

#ifndef SWITCHBUTTON_H_
#define SWITCHBUTTON_H_

class SwitchButton {
private:
	bool switchOn;
public:
	SwitchButton();
	virtual ~SwitchButton();

	void buttonClicked();
	bool isOn();
};

#endif /* SWITCHBUTTON_H_ */
