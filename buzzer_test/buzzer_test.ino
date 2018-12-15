#include "Arduino.h"

#include "pitches.h"
#include "buzzer.h"

const int buzzerPin = A5;
Buzzer myBuzzer;

// notes in the melody:
int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};

const int melodySize = sizeof(melody)/sizeof(int);

// The setup function is called once at startup of the sketch
void setup()
{
	Serial.begin(9600);
	Serial.println("Start buzzer test");


	Serial.print("Number of notes: ");
	Serial.println(melodySize);
	myBuzzer.attach(buzzerPin);

}

// The loop function is called in an endless loop
void loop()
{

	tone(buzzerPin, NOTE_C7, 100);
	//myBuzzer.playMelody(melodySize, melody, noteDurations);
	delay(2000);
}

