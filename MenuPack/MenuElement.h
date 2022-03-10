

#pragma once

#include "Arduino.h"
#include "Buttons.h"
#include "Menu.h"

enum MenuElementType {
  ME_NAME, //Menu name plus description
  ME_MENU, //a basic member of the menu with selected arrow
  ME_PARENT, //clicking on it enters the submenu
  ME_BACK, //by clicking on it we return to the previous menu
  ME_RADIO, //radio element (only one value can be selected in the radio)
  ME_CHECKBOX, //checkbox element (more values can be selected in checkbox)
  ME_INT,
  ME_FLOAT,
  ME_TOP
};

union change_value {
  struct change_int_value {
    int value;
    int new_value;
    int min;
    int max;
    int increment;
  } int_value;
  struct change_float_value {
    float value;
    float new_value;
    float min;
    float max;
    float increment;
  } float_value;
};

struct menu_element {
  String name;
  MenuElementType type;
  void (*actionFunctionFirst)(); //a function that is performed after selecting element
  void (*actionFunctionSecond)(); //a function that is performed in a loop after selecting an element
  bool back = false; //true if you returns from sub to the higher menu
  bool radio_checkbox_selected = false; //if the item is inside the radio or checkbox indicates whether it is selected
  change_value value;
  bool change_value = false;
  void (*actionFunctionWhenChangeValue)();
  bool action_function_when_change_value_exist = false;
};
