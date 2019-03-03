#include "Arduino.h"
#include <LiquidCrystal.h>

#include "pitches.h"
#include "buzzer.h"
#include "EmulatedVal.h"

const int buzzerPin = A5;
Buzzer myBuzzer;

LiquidCrystal lcd(12, 8, 7, 4, 3, 2);
unsigned long int startTime = 0;
unsigned long int endTime = 0;
bool pressedLastCycle = false;
long int lastBuzzTimeStamp;
bool firstMillis = true;

const long SECOND = 1000;
const long MINUTE = SECOND * 60L;
const long TEN_MINUTES = MINUTE * 10;
const long HOUR = MINUTE * 60;


/* consts define which count down time to use, this can be a default, or manually configurable */
const long DEFAULT_BOMB_TIME = HOUR + (15 * MINUTE);
const bool MANUAL_TIME_SELECT = false;  // The default is used when this const is set to false
long countDownTime = MANUAL_TIME_SELECT ? 0: HOUR;

// number of attempts, NOTE: cannot be const since the number is decreased every attempt
int pogingen = 5;

// output values
const EmulatedVal* E1 = new EmulatedVal(254, 0, 40);
const EmulatedVal* E2 = new EmulatedVal(80, 60, 100);
const EmulatedVal* E3 = new EmulatedVal(120, 100, 140);
const EmulatedVal* E4 = new EmulatedVal(160, 140, 180);
const EmulatedVal* E5 = new EmulatedVal(200, 180, 220);

int powerSwitch = 1;
int wire1 = 11;
int wire2 = 10;
int wire3 = 9;
int wire4 = 6;
int wire5 = 5;
int socket1 = A0;
int socket2 = A1;
int socket3 = A2;
int socket4 = A3;
int socket5 = A4;
int onklaar = 0;
int buttonPin = 13;

// Tone heights for correct and incorrect codes
const int correctTones[] = { NOTE_B5, NOTE_A6 };
const int incorrectTones[] = { NOTE_GS3, NOTE_G3 };
const int explodeTone = NOTE_C7;
const int noteDurations[] = { 4, 4 };
const int buttonTone = NOTE_C7;
const int timerTone = NOTE_C7;

enum EClockState {
	INIT, RUNNING, DONE
};
EClockState clockState = EClockState::INIT;

enum EBombState {
	SHARP, DISMANTLED, EXPLODED
};
EBombState bombState = SHARP;

// NOTE: disable serial with logger on false when using the power switch
// (it uses pin 1 which is dedicated for serial communication)
void setup() {
	//Serial.begin(9600);
	pinMode(powerSwitch, OUTPUT);
	digitalWrite(powerSwitch, HIGH);
	delay(2000);
	myBuzzer.attach(buzzerPin);
	pinMode(wire1, OUTPUT);
	pinMode(wire2, OUTPUT);
	pinMode(wire3, OUTPUT);
	pinMode(wire4, OUTPUT);
	pinMode(wire5, OUTPUT);
	pinMode(socket5, INPUT_PULLUP);
	pinMode(socket1, INPUT_PULLUP);
	pinMode(socket2, INPUT_PULLUP);
	pinMode(socket3, INPUT_PULLUP);
	pinMode(socket4, INPUT_PULLUP);

	lcd.begin(16, 2);
	lcd.setCursor(0, 0);
	lcd.print("0:00:00:00");

	analogWrite(wire1, E1->outputValue);
	analogWrite(wire2, E2->outputValue);
	analogWrite(wire3, E3->outputValue);
	analogWrite(wire4, E4->outputValue);
	analogWrite(wire5, E5->outputValue);

}

void startTimer() {
	Serial.println("Start timer and set clock state to running");
	clockState = EClockState::RUNNING;
	startTime = millis();
	myBuzzer.singleTone(timerTone, 100);
	delay(1000);
}

void handleInitState() {
	int buttonState = digitalRead(buttonPin);
	if (buttonState == HIGH) {
		Serial.println("Button pushed during init state");
		// set button state for next cycle
		pressedLastCycle = true;
		//digitalWrite(powerSwitch, LOW);
	}
	if (buttonState == LOW && pressedLastCycle) {
		Serial.println("Button was pressed once");
		// button was pressed once, increase timer
		lcd.setCursor(2, 0);
		countDownTime += TEN_MINUTES;
		if (countDownTime > 3 * HOUR) {
			countDownTime = 0;
		}
		int secondhours = countDownTime / HOUR;
		int secondminutes = (countDownTime % HOUR) / MINUTE;
		lcd.print(secondminutes);
		lcd.setCursor(0, 0);
		lcd.print(secondhours);
		lcd.setCursor(0, 1);
		lcd.print("HOLD TO CONFIRM");
		pressedLastCycle = false;
		delay(200);
	}

	// start when the button is hold for a while
	int i = 0;
	while (buttonState == HIGH) {
		buttonState = digitalRead(buttonPin);
		if (i > 99) {
			Serial.println("User started the timer with button push");
			startTimer();
			return;
		}
		i += 1;
		delay(10);
	}
}

void timerBuzz(int timeStamp) {
	if (timeStamp != lastBuzzTimeStamp) {
		myBuzzer.singleTone(NOTE_C7, 100);
		lastBuzzTimeStamp = timeStamp;
	}
}

void checkTimerBuzz(long milliseconds) {
	int seconds = (milliseconds % MINUTE) / 1000;
	if (milliseconds > TEN_MINUTES) {
		// buzz every 10 seconds
		if (seconds % 10 == 0) {
			timerBuzz(seconds);
		}
	} else if (milliseconds > MINUTE * 5) {
		// buzz every 5 seconds
		if (seconds % 5 == 0) {
			timerBuzz(seconds);
		}
	} else if (milliseconds > MINUTE * 2) {
		// buzz every 3 seconds
		if (seconds % 3 == 0) {
			timerBuzz(seconds);
		}
	} else if (milliseconds > MINUTE) {
		// buzz every second
		if (seconds % 2 == 0) {
			timerBuzz(seconds);
		}

	} else if (milliseconds > SECOND * 10) {
		// buzz every second
		if (seconds % 1 == 0) {
			timerBuzz(seconds);
		}

	} else {
		// last 10 seconds
		if (firstMillis) {
			lastBuzzTimeStamp = milliseconds;
			firstMillis = false;
		}
		// buzz every 500 milliseconds
		if (milliseconds < lastBuzzTimeStamp - 500) {
			timerBuzz(milliseconds);
		}
	}
}

String formatOutput(int hours, int minutes, int seconds, int tenmils) {
	String result = "";
	result += hours;
	result += ":";
	if (minutes < 10) {
		result += "0";
	}
	result += minutes;
	result += ":";
	if (seconds < 10) {
		result += "0";
	}
	result += seconds;
	result += ":";
	if (tenmils < 10) {
		result += "0";
	}
	result += tenmils;
	return result;
}

void printVal(int val) {
	Serial.print(val);
	Serial.print(", ");
}

int readVal(int socket) {
	int delayTime = 10;
	int val = analogRead(socket);
	delay(delayTime);
	val = analogRead(socket);
	delay(delayTime);
	return val / 4;
}

boolean checkWire(int val, const EmulatedVal* ev) {
	return val > (ev->min) && val < (ev->max);
}

boolean checkWires() {
	// note: values are read twice here
	int val1 = readVal(socket1);
	printVal(val1);
	int val2 = readVal(socket2);
	printVal(val2);
	int val3 = readVal(socket3);
	printVal(val3);
	int val4 = readVal(socket4);
	printVal(val4);
	int val5 = readVal(socket5);
	printVal(val5);

	//Serial.println("");
	// 1 - 5 from left to right = red, green, yellow, black, white
	return checkWire(val1, E5) && checkWire(val2, E4) && checkWire(val3, E2)
			&& checkWire(val4, E1) && checkWire(val5, E3);
}

void done() {
	Serial.println("Done");
	endTime = millis();
	clockState = EClockState::DONE;
}

void explode() {
	Serial.println("Explode");
	myBuzzer.singleTone(explodeTone, 2000);
	lcd.setCursor(0, 0);
	lcd.print("VERLOREN!      ");
	lcd.setCursor(0, 1);
	lcd.print("               ");
	bombState = EBombState::EXPLODED;
	done();

}

void dismantle() {
	Serial.println("Dismantle");
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("BOM ONTMANTELD! ");

	for (int i = 0; i < 3; i++) {
		myBuzzer.playMelody(2, correctTones, noteDurations);
		myBuzzer.singleTone(NOTE_D7, 100);
		delay(200);
	}
	bombState = EBombState::DISMANTLED;
	delay(5000);
	lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Vernietig de");
    lcd.setCursor(0, 1);
    lcd.print("  plannen!!!  ");
	done();
}

void handleRunningState() {

	// Calculate the amount of milliseconds left
	long milliseconds = countDownTime - (millis() - startTime);
	int hours = milliseconds / HOUR;
	int minutes = (milliseconds % HOUR) / MINUTE;
	int seconds = (milliseconds % MINUTE) / SECOND;
	int tenmils = (milliseconds % SECOND) / 10;

	checkTimerBuzz(milliseconds);

	lcd.setCursor(0, 0);
	lcd.print(formatOutput(hours, minutes, seconds, tenmils));
	lcd.setCursor(0, 1);
	lcd.print("POGINGEN : ");
	lcd.print(pogingen);
	lcd.print("                 ");

	if (milliseconds > 0) {
		// there is still time if the button is pressed the wires are checked for the right combination
		int buttonState = digitalRead(buttonPin);
		if (buttonState == HIGH) {

			bool ontmanteld = checkWires();

			// there is still time
			if (ontmanteld) {
				dismantle();
			} else {
				pogingen -= 1;
				//Serial.println("!poging!");
				myBuzzer.playMelody(2, incorrectTones, noteDurations);
				delay(1000);
				countDownTime -= MINUTE;
				if (pogingen < 1) {
					countDownTime = 0;
				}
			}
		}
	} else {
		explode();
	}

}

void loop() {
	switch (clockState) {
	case INIT:
	    // NOTE: MANUAL_TIME_SELECT defines if the user can manually set the count down time
	    // the default time
	    if (MANUAL_TIME_SELECT) {
	        handleInitState();
	    } else {
	        startTimer();
	    }
		break;
	case RUNNING:
		if (bombState == EBombState::SHARP) {
			handleRunningState();
		}
		break;
	case DONE:
		// game is done, check whether we should turn off the arduino
		if (millis() - endTime > 1 * MINUTE) {
			// turn off
			digitalWrite(powerSwitch, LOW);
		}
		break;
	}

}

