#include "Arduino.h"
//The setup function is called once at startup of the sketch
//void setup()
//{
// Add your initialization code here
//}

// The loop function is called in an endless loop
//void loop()
//{
//Add your repeated code here
//}

#include <Servo.h>

#define NOTE_A6 1760
#define NOTE_B5 988
#define NOTE_C7 2093

//define pins
const int firstButton = 8;
const int secondButton = 9;
const int thirdButton = 10;
const int redLed = 11;
const int greenLed = 12;
const int buzzer = 13;

// Define the secret code to be used, can be any array length as long as the numbers range between 1-3
const int code[] = {1,1,1}; //this is the code that we set for our lock
const int CODE_SIZE = sizeof(code)/sizeof(int);

// Tone heights for correct and incorrect codes
const int correctTones[] = {988, 1760};
const int incorrectTones[] = {208, 196};

// variables
Servo myServo;

// Empty array to store the values entered by the user
int userEntered[CODE_SIZE];

//counter to store number of times buttons have been pressed
int buttonCheck;

void setup() {
  myServo.attach(7);

  // set up the serial monitor
  Serial.begin(9600);
  Serial.println("Please enter a code to unlock the safe.");
  Serial.println();

  // Setup input pins
  pinMode(firstButton, INPUT);
  pinMode(secondButton, INPUT);
  pinMode(thirdButton, INPUT);

  // Set up output pins
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(buzzer, OUTPUT);

  // Initialise with red led on to denote "locked" status
  digitalWrite(redLed, HIGH);

  // initilise servo in locked position
  lockServo();
}

void loop() {

  // check the buttons
  if (digitalRead(firstButton) == HIGH){
    tone(buzzer, NOTE_C7, 100);
    checkEntered(1);

    // wait some time, otherwise a button push will be detected as multiple pushes
    delay(250);

  }
  else if (digitalRead(secondButton) == HIGH){
    tone(buzzer, NOTE_B5, 100);
    checkEntered(2);
    delay(250); //wait

  }
  else if (digitalRead(thirdButton) == HIGH){ //if greenButton is pressed
    tone(buzzer, NOTE_A6, 100);
    checkEntered(3);
    delay(250);
  }
}


//this function resets the board through software.
void(* resetFunc) (void) = 0;        // declare reset fuction at address 0

// Receives input from the user, checks if an index is 0 i.e. empty
// if empty, sets that index to the user input, stores the index and increments check
void checkEntered(int button) {
  int index = -1;                   //-1 if array is full
  for (int i = 0; i< CODE_SIZE; i++) {
    //Serial.print("size of userEntered is "); Serial.println(sizeof(userEntered));
    if (userEntered[i] == 0) {      //if index is empty
      userEntered[i] = button;      //store the pressed button in that index
      index = i;                    //store the index
      buttonCheck++;                //increment pressed button count
      Serial.print(i); Serial.print(": "); Serial.println(userEntered[i]);
      break;                        //exit the loop
    }
  }

  //if CODE_SIZE buttons have been pushed..
  if (buttonCheck == CODE_SIZE) {
    // Proceed to checking the entered code with the actual code
    checkCode();
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
  myServo.write(160);
}

boolean isCodeCorrect() {
  if (sizeof(userEntered)/sizeof(int) != CODE_SIZE) {
    return false;
  }
  for (int i = 0; i < CODE_SIZE; i++) {
    if (userEntered[i] != code[i]) {
      return false;
    }
  }
  return true;
}

void playSounds(const int beeps[]) {
   //loop to emit two short beeps denoting  code is correct
   int numberOfBeeps = sizeof(beeps) / sizeof(int);
   for (int i = 0; i < numberOfBeeps; i++) {
     tone(buzzer, beeps[i], 500);
     //delay(100);
   }
   noTone(buzzer);
}

// Compares entered code to actual code
void checkCode() {

  if (isCodeCorrect()) {
    digitalWrite(redLed, LOW);      //turn red LED off
    digitalWrite(greenLed, HIGH);   //turn green LED on
    Serial.println("Code is correct. Safe unlocked.");
    Serial.println();

     //loop to emit two short beeps denoting  code is correct
    playSounds(correctTones);

    //display green LED for 1.5 s
    delay(1500);
    // turn the green led off
    digitalWrite(greenLed, LOW);
    delay(500);
    unlockServo();

    // rest board will lock the servo again
//    delay(15000);
//    resetFunc();
  } else {
    Serial.println("Entered code is wrong. Please try again.");
    playSounds(incorrectTones);

    delay(300);
    resetFunc();
  }
}

