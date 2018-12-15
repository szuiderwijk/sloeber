#include "Arduino.h"
#include <Servo.h>
#include <Buzzer.h>
#include <pitches.h>

//define pins
const int firstButton = 8;
const int secondButton = 9;
const int thirdButton = 10;
const int redLed = 11;
const int greenLed = 12;
const int buzzerPin = 13;

const int BUTTON_DELAY = 300;

// Define the secret code to be used, can be any array length as long as the numbers range between 1-3
const int code[] = { 2, 3, 1, 1, 3, 2, 1, 2, 3, 1}; //this is the code that we set for our lock
const int CODE_SIZE = sizeof(code) / sizeof(int);

// Tone heights for correct and incorrect codes
const int correctTones[] = { NOTE_B5, NOTE_A6 };
const int incorrectTones[] = { NOTE_GS3, NOTE_G3 };
const int noteDurations[] = { 4, 4 };
const int correctTone = NOTE_C7;
int seq = 0;

// variables
Servo myServo;
Buzzer myBuzzer;

// Empty array to store the values entered by the user
int userEntered[CODE_SIZE];

//counter to store number of times buttons have been pressed
int buttonCheck;

void setup() {
	myServo.attach(7);
	myBuzzer.attach(buzzerPin);

	// set up the serial monitor
	Serial.begin(9600);
	Serial.println();

	// Setup input pins
	pinMode(firstButton, INPUT);
	pinMode(secondButton, INPUT);
	pinMode(thirdButton, INPUT);

	// Set up output pins
	pinMode(redLed, OUTPUT);
	pinMode(greenLed, OUTPUT);

	// Initialize with red led on to denote "locked" status
	digitalWrite(redLed, HIGH);

	// Initialize Servo in locked position
	lockServo();
	Serial.println("Please enter a code to unlock the safe.");
}

void loop() {

	// check the buttons
	// wait some time after each button press, otherwise a button push will be detected as multiple pushes

	if (digitalRead(firstButton) == HIGH) {
		checkEntered(1);
		delay(BUTTON_DELAY);

	} else if (digitalRead(secondButton) == HIGH) {
		checkEntered(2);
		delay(BUTTON_DELAY);

	} else if (digitalRead(thirdButton) == HIGH) {
		checkEntered(3);
		delay(BUTTON_DELAY);
	}
}

// Receives input from the user, checks if an index is 0 i.e. empty
// if empty, sets that index to the user input, stores the index and increments check
void checkEntered(int button) {

	if (seq < CODE_SIZE) {
		if (code[seq] == button) {
			// correct sequence
			myBuzzer.singleTone(correctTone, 250);
			seq++;
		} else {
			// incorrect go back to the start
			Serial.println("Entered code is wrong. Please try again.");
			myBuzzer.playMelody(2, incorrectTones, noteDurations);
			seq = 0;
		}

		if (seq == CODE_SIZE) {
			// code entered completely successfully
			digitalWrite(redLed, LOW);      //turn red LED off
			digitalWrite(greenLed, HIGH);   //turn green LED on
			Serial.println("Code is correct. Safe unlocked.");
			myBuzzer.playMelody(2, correctTones, noteDurations);
			unlockServo();
		}
	}
}

void lockServo() {
	Serial.println("Locking safe now.");
	delay(100);
	myServo.write(15);
}

void unlockServo() {
	Serial.println("Unlocking safe now.");
	delay(100);
	myServo.write(100);
}



