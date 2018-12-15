#include "Arduino.h"
#include "Buzzer.h"
#include <ArduinoSTL.h>

#include "morse.h"

using std::vector;
using std::string;

#define NOTE_C7 2093
Buzzer buzzer;
const int morsePin = 2;
const int correctPin = 3;
const int buzzerPin = 8;
const int BUTTON_DELAY = 300;
vector<string> cities(10);

string morseSequence;
string cityName;

char handleHigh() {
	int cnt = 0;
	while (digitalRead(morsePin) == HIGH) {
		digitalWrite(buzzerPin, HIGH);
		cnt++;
		if (cnt > 20) {
			Serial.println("- pressed");
			digitalWrite(buzzerPin, LOW);
			delay(BUTTON_DELAY);
			return '-';
		}
		delay(10);
	}

	Serial.println(". pressed");
	digitalWrite(buzzerPin, LOW);
	delay(BUTTON_DELAY);
	return '.';

}

boolean inCities(const std::string &value,
		const std::vector<std::string> &array) {
	return std::find(array.begin(), array.end(), value) != array.end();
}

void handlePushButton() {
	if (digitalRead(morsePin) == HIGH) {
		morseSequence += handleHigh();
	} else if (morseSequence.length() > 0) {
		int cnt = 0;
		while (digitalRead(morsePin) == LOW) {
			cnt++;
			if (cnt > 100 && morseSequence.length() > 0) {
				cnt = 0;
				std::string character = decodeMorseCharacter(morseSequence);

				if (character.length() > 0) {
					Serial.print("Character found: ");
					Serial.println(character.c_str());
					Serial.print("Total string: ");
					morseSequence.clear();
					// character found
					cityName += character;
					Serial.println(cityName.c_str());
					if (inCities(cityName, cities)) {
						Serial.print("City found: ");
						Serial.println(cityName.c_str());
						cityName.clear();
					}
				}
				return;
			}
			delay(10);
		}
	}

	digitalWrite(buzzerPin, LOW);
}

void handleCorrectionButton() {
	if (digitalRead(correctPin) == HIGH) {
		buzzer.singleTone(NOTE_C7, 100);
		if (morseSequence.length() > 0) {
			morseSequence.pop_back();
			Serial.print("Morse sequence corrected: ");
			Serial.println(morseSequence.c_str());
		} else if (cityName.length() > 0) {
			cityName.pop_back();
			Serial.print("Code name corrected: ");
			Serial.println(cityName.c_str());
		}
		delay(BUTTON_DELAY);
	}
}

void initCities() {

	cities[0] = "COSTA RICA";
	cities[1] = "SOUTH AFRICA";
	cities[2] = "AUSTRALIA";
	cities[3] = "BANGKOK";
	cities[4] = "DUBAI";
	cities[5] = "VANCOUVER";
	cities[6] = "AINSA";
	cities[7] = "SAN FRANCISCO";
}

//The setup function is called once at startup of the sketch
void setup() {
	buzzer.attach(buzzerPin);
	pinMode(morsePin, INPUT);
	pinMode(correctPin, INPUT);
	initCities();
	Serial.begin(9600);
	Serial.println("Initializing");
	buzzer.singleTone(2000, 200);
}

// The loop function is called in an endless loop
void loop() {
	//Add your repeated code here

	handlePushButton();

	handleCorrectionButton();
}
