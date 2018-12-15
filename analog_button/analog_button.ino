#include "Arduino.h"
#include "SwitchButton.h"
#include "ButtonSettings.h"

const int CODE = 63;
const int ANALOG_THRESHOLD = 5;
const int BUTTON_SIZE = 6;

ButtonSettings *buttons[BUTTON_SIZE];
const int buttonIn = A5;
const int greenLed = 7;

//The setup function is called once at startup of the sketch
void setup() {
// Add your initialization code here
	initSwitchButtons();
	pinMode(greenLed, OUTPUT);
	Serial.begin(9600);
	Serial.println("start");
}

// The loop function is called in an endless loop
void loop() {

	int sensorVal = analogRead(buttonIn);
	if (sensorVal > ANALOG_THRESHOLD) {
		Serial.println(sensorVal);

		ButtonSettings *button = getSwitchButton(sensorVal);
		if (button != NULL) {
			button->button.buttonClicked();
			setLed(button);
			checkCode();
		}
		delay(250);
	}
}

void initSwitchButtons() {
	buttons[0] = new ButtonSettings(998, 1010, 10);
	buttons[1] = new ButtonSettings(970, 985, 11);
	buttons[2] = new ButtonSettings(920, 940, 12);
	buttons[3] = new ButtonSettings(835, 855, 13);
	buttons[4] = new ButtonSettings(1012, 1024, 9);
	buttons[5] = new ButtonSettings(510, 525, 8);

	for (int i = 0; i < BUTTON_SIZE; i++) {
		pinMode(buttons[i]->ledPin, OUTPUT);
	}
}

ButtonSettings *getSwitchButton(int sensorVal) {

	for (int i = 0; i < BUTTON_SIZE; i++) {
		if (sensorVal >= buttons[i]->low && sensorVal <= buttons[i]->height) {
			return buttons[i];
		}
	}
	return NULL;
}

void setLed(ButtonSettings *button) {
	digitalWrite(button->ledPin, button->button.isOn() ? HIGH : LOW);
}

void checkCode() {
	int enteredCode = 0;
	for (int i = 0; i < BUTTON_SIZE; i++) {
		if (buttons[i]->button.isOn()) {
			enteredCode += round(pow(2, i));
		}
	}

	Serial.print("Entered code: ");
	Serial.println(enteredCode);
	if (enteredCode == CODE) {
		Serial.println("Entered code is correct");
		digitalWrite(greenLed, HIGH);
	} else {
		digitalWrite(greenLed, LOW);
	}

}

