#include "Arduino.h"
/* www.learningbuz.com */
/*Impport following Libraries*/
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
//I2C pins declaration
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
//LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7);  // Set the LCD I2C address

void setup() {

	lcd.begin(16, 2); //Defining 16 columns and 2 rows of lcd display
	lcd.backlight(); //To Power ON the back light
lcd.backlight();// To Power OFF the back light

}

void loop() {
//Write your code
	lcd.clear();
	lcd.setCursor(0, 0); //Defining positon to write from first row,first column .
	lcd.print("Morse Code"); //You can write 16 Characters per line .
	delay(2000); //Delay used to give a dynamic effect
	lcd.clear();
	lcd.setCursor(0, 0); //Defining positon to write from second row,first column .
	lcd.print("Typ A Town");
	delay(3000);

	lcd.clear();  //Clean the screen
	lcd.setCursor(0, 0);
	lcd.print("Pres The Button");
	delay(2000);
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("Code Typed");
	delay(3000);
}
