#include "Arduino.h"
#include <LiquidCrystal.h>


LiquidCrystal lcd(12, 8, 7, 4, 3, 2);
long setTime = 0;
int start = 0;
int i = 0;
int pressed = 0;
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
int pogingen = 5;


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

int checkWires() {
	//Serial.print("input : ");
	int val1 = readVal(socket1);
	printVal(readVal(socket1));
	int val2 = readVal(socket2);
	printVal(readVal(socket2));
	int val3 = readVal(socket3);
	printVal(readVal(socket3));
	int val4 = readVal(socket4);
	printVal(readVal(socket4));
	int val5 = readVal(socket5);
	printVal(readVal(socket5));
	Serial.println("");
	if (val1 < 60 && val2 > 60 && val2 < 100 && val3 > 100 && val3 < 140
			&& val4 > 140 && val4 < 180 && val5 > 180 && val5 < 220) {
		return 1;
	} else {
		return 0;
	}
}

void explode() {
	lcd.setCursor(0, 0);
	lcd.print("GEEXPLODEERD!     ");
	lcd.setCursor(0, 1);
	lcd.print("               ");
	onklaar = 1;
}

void dismantle() {
	lcd.setCursor(0, 0);
	lcd.print("ONTMANTELD!     ");
	lcd.setCursor(0, 1);
	lcd.print("                    ");
	onklaar = 1;
}

void setup() {
	pinMode(wire1, OUTPUT);
	pinMode(wire2, OUTPUT);
	pinMode(wire3, OUTPUT);
	pinMode(wire4, OUTPUT);
	pinMode(wire5, OUTPUT);
	pinMode(socket1, INPUT_PULLUP);
	pinMode(socket2, INPUT_PULLUP);
	pinMode(socket3, INPUT_PULLUP);
	pinMode(socket4, INPUT_PULLUP);
	pinMode(socket5, INPUT_PULLUP);
	Serial.begin(9600);
	lcd.begin(16, 2);
	lcd.setCursor(0, 0);
	lcd.print("0:00:00:00");
	analogWrite(wire1, 40);
	analogWrite(wire2, 80);
	analogWrite(wire3, 120);
	analogWrite(wire4, 160);
	analogWrite(wire5, 200);
}

void loop() {
	int switchstate_setplus_time = digitalRead(13);
	if (start == 1 && onklaar == 0) {
		int ontmanteld = checkWires();
		long milliseconds = setTime - millis();
		int hours = milliseconds / 3600000;
		int minutes = (milliseconds % 3600000) / 60000;
		int seconds = (milliseconds % 60000) / 1000;
		int tenmils = (milliseconds % 1000) / 10;
		int procent = milliseconds * 100 / setTime + 1;
		if (milliseconds > 0) {
			if (onklaar == 0) {
				lcd.setCursor(0, 0);
				lcd.print(formatOutput(hours, minutes, seconds, tenmils));
				lcd.setCursor(0, 1);
				lcd.print("POGINGEN : ");
				lcd.print(pogingen);
				lcd.print("                 ");
				if (ontmanteld == 1 && switchstate_setplus_time == HIGH) {
					dismantle();
				}
				if (switchstate_setplus_time == HIGH
						&& milliseconds < (setTime - 10000) && ontmanteld == 0) {
					pogingen -= 1;
					Serial.println("!poging!");
					delay(1000);
				}
				Serial.println(pogingen);
				if (pogingen < 1) {
					setTime = 0;
				}
			}
		} else {
			if (onklaar == 0) {
				explode();
			}
		}
	}

	if (start == 0 && onklaar == 0) {
		if (switchstate_setplus_time == HIGH) {
			pressed = 1;
		}
		if (switchstate_setplus_time == LOW && pressed == 1) {
			lcd.setCursor(2, 0);
			setTime += 600000;
			if (setTime > 10800000) {
				setTime = 0;
			}
			int secondhours = setTime / 3600000;
			int secondminutes = (setTime % 3600000) / 60000;
			lcd.print(secondminutes);
			lcd.setCursor(0, 0);
			lcd.print(secondhours);
			lcd.setCursor(0, 1);
			lcd.print("HOLD TO CONFIRM");
			pressed = 0;
			delay(200);
		}
		while (switchstate_setplus_time == HIGH) {
			switchstate_setplus_time = digitalRead(13);
			if (i > 99) {
				setTime += millis();
				start = 1;
				break;
			}
			i += 1;
			if (switchstate_setplus_time == LOW) {
				i = 0;
			}
			delay(10);
		}
	}
}


