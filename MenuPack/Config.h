/*
This is a configuration file that you must first fill out,
and after that in the Arduino IDE you need to write a program that uses these libraries for your lcd
(an example of how to write that program is in the example folder).
It is assumed that lcd attaches to the arduino board.

If you do not have something already defined, such as a specific button, simply delete the defined value.
If you want to define something differently, simply change the value to the desired one,
but look for a form that should be the same as written for the program to work.
*/

#pragma once

//Pins to which buttons are connected
//There should be 2 buttons at a minimum
//If the button is connected to a digital pin, enter the pin number eg 3,
//and if connected to an analog write eg 0

//if a specific menu item needs to be selected to perform an action then the select button must exist,
//otherwise it should not exist
#define SELECT 0
//if the button is analog write 0x00 and if digital 0x01
#define A_D_SELECT 0x00
//if the back button does not exist and you need one,
//you can create a menu item called "back" to perform this function
#define BACK 0
//if the button is analog write 0x00 and if digital 0x01
#define A_D_BACK 0x00
//the up and down buttons must exist if the menu has more than one element
#define UP 0
//if the button is analog write 0x00 and if digital 0x01
#define A_D_UP 0x00
#define DOWN 0
//if the button is analog write 0x00 and if digital 0x01
#define A_D_DOWN 0x00
//button for scrolling text if it is longer than the number of columns specified for the texts
//button for going on the top of menu, if you dont have it you can make menu element on_top
#define RIGHT 0
//if the button is analog write 0x00 and if digital 0x01
#define A_D_RIGHT 0x00

//If multiple buttons are connected to the same pin,
//there are intervals of numbers to determine which button was pressed, "minvalue-maxvalue"
//if each button has a different pin, leave this blank
#define MIN_SELECT 600
#define MAX_SELECT 800
#define MIN_BACK 400
#define MAX_BACK 599
#define MIN_UP 60
#define MAX_UP 199
#define MIN_DOWN 200
#define MAX_DOWN 399
#define MIN_RIGHT 0
#define MAX_RIGHT 59

//an arrow that indicates which element we are currently on
#define SELECTED_ARROW ">"
//arrow indicating which element is marked in checkbox or radio
#define RADIO_CHECKBOX_ARROW "x "

//number of rows and columns on LCD
#define ROWS 2
#define COLUMNS 16
