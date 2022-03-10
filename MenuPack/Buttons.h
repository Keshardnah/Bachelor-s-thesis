/*
  Buttons.h - Library for detecting which button is pressed.
  Created by Ana Andrašek, April 5, 2020.
*/

#pragma once

#include "Arduino.h"
#include "Config.h"

//the maximum number of buttons
#define MAX_BTN 5

enum Button {
  BT_SELECT,
  BT_BACK,
  BT_UP,
  BT_DOWN,
  BT_RIGHT,
  BT_NONE
};

class Buttons
{
private:
    //the values of the pins to which the buttons are connected are written in the arrays,
    //and using the position we know which pin corresponds to which button
    //positions corresponding to specific buttons:
    // 0-select, 1-back, 2-up, 3-down, 4-scroll

    //array for analog buttons
    int analog_btn[MAX_BTN];
    //array for digital buttons
    int digital_btn[MAX_BTN];

    //function sets arrays pins to 100 (this pin does not exist on arduino boards)
    //by this we know which buttons exist and which buttons do not
    void set_array_default (int *array, int size);
public:
  //the constructor in which the assumed values are set
  Buttons();

  //returns which button is pressed
  enum Button detectButton();

  //If buttons are analog, due to the interval value accounts which button is pressed
  enum Button analog_detect_btn(int value_of_button);

  //checks which buttons are analog and which buttons are digital
  //and therefore fill the arrays: analog_btn and digital_btn
  void check_the_existence_of_buttons();
};
