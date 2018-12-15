/*
 * EmulatedVal.h
 *
 *  Created on: 15 Oct 2018
 *      Author: stephanzuiderwijk
 */

#ifndef EMULATEDVAL_H_
#define EMULATEDVAL_H_

class EmulatedVal {
public:
	int outputValue;
	int min;
	int max;

	EmulatedVal(int outputValue, int min, int max);
	virtual ~EmulatedVal();

};

#endif /* EMULATEDVAL_H_ */
