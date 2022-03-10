/*
  Buttons.h - Library for detecting which button is pressed.
  Created by Ana Andrašek, April 5, 2020.
*/

#include "Buttons.h"

Buttons::Buttons(){
  set_array_default(analog_btn, MAX_BTN); //sets arrays pins to 100
  set_array_default(digital_btn, MAX_BTN); //sets arrays pins to 100
}

void Buttons::check_the_existence_of_buttons() {
  #ifdef SELECT //check if the button exists at all
    if(A_D_SELECT == 0x00) { //if button is analog
      analog_btn[0] = SELECT; //enter the value of its pin in the array of analog buttons at a specific position for that button
    }
    else { //if button is digital
      digital_btn[0] = SELECT; //enter the value of its pin in the array of digital buttons at a specific position for that button
      pinMode(SELECT, INPUT); //arduino command that must be executed if the button is digital
    }
  #endif
  #ifdef BACK
    if(A_D_BACK == 0x00) {
      analog_btn[1] = BACK;
    }
    else {
      digital_btn[1] = BACK;
      pinMode(BACK, INPUT);
    }
  #endif
  #ifdef UP
    if(A_D_UP == 0x00) {
      analog_btn[2] = UP;
    }
    else {
      digital_btn[2] = UP;
      pinMode(UP, INPUT);
    }
  #endif
  #ifdef DOWN
    if(A_D_DOWN == 0x00) {
      analog_btn[3] = DOWN;
    }
    else {
      digital_btn[3] = DOWN;
      pinMode(DOWN, INPUT);
    }
  #endif
  #ifdef RIGHT
    if(A_D_RIGHT == 0x00) {
      analog_btn[4] = RIGHT;
    }
    else {
      digital_btn[4] = RIGHT;
      pinMode(RIGHT, INPUT);
    }
  #endif
}

enum Button Buttons::detectButton() {
  int value_of_button; //a value indicating whether the button is active
  for(int i=0; i<MAX_BTN; i++) { //loop that goes through the digital buttons
    if(digital_btn[i] != 100) { //if there is such a digital button
      value_of_button = digitalRead(digital_btn[i]); //check if the button is active or not
      if(value_of_button == HIGH) { //if the button is active
        switch (i) { //find the button that is active from the position in the array
          case 0:
            return BT_SELECT; //return active button
            break;
          case 1:
            return BT_BACK;
            break;
          case 2:
            return BT_UP;
            break;
          case 3:
            return BT_DOWN;
            break;
          case 4:
            return BT_RIGHT;
            break;
        }
      }
    }
  }
  bool only_digital = true; //in case we only have digital buttons and none were active, that we can return NONE
  for(int i=0; i<MAX_BTN; i++) { //loop that goes through the analog buttons
    if(analog_btn[i] != 100) { //if there is such analog button
      only_digital = false; //there are also analog buttons
      value_of_button = analogRead(analog_btn[i]); //we check which button is active
      return analog_detect_btn(value_of_button); //we return a button that is active, or NONE if there is none active
    }
  }
  if(only_digital) return BT_NONE; //we will return NONE if none has been returned so far
}

enum Button Buttons::analog_detect_btn(int value_of_button){
  #ifdef MIN_SELECT //we check that there is an interval for that button
    //if the current value is within that interval, we return that button
    if (value_of_button >= MIN_SELECT && value_of_button <= MAX_SELECT)  return BT_SELECT;
  #endif
  #ifdef MIN_BACK
    if (value_of_button >=  MIN_BACK && value_of_button <= MAX_BACK)  return BT_BACK;
  #endif
  #ifdef MIN_UP
    if (value_of_button >=  MIN_UP && value_of_button <= MAX_UP)  return BT_UP;
  #endif
  #ifdef MIN_DOWN
    if (value_of_button >=  MIN_DOWN && value_of_button <= MAX_DOWN)  return BT_DOWN;
  #endif
  #ifdef MIN_RIGHT
    if (value_of_button >=  MIN_RIGHT && value_of_button <= MAX_RIGHT)  return BT_RIGHT;
  #endif
  return BT_NONE;
}

void Buttons::set_array_default (int *array, int size) {
  for (int i=0; i<size; i++) *(array+i) = 100;
}
