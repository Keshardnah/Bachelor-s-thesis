

#pragma once

#include "MenuElement.h"

struct menu {
  String arrow;
  int current_element = 0;
  int current_row = 0; //current row on LCD
  int max_menu_elements;
  menu_element **menuElements;
  menu_element *parent; //if the menu is a sub menu then there is a parent in the higher menu so it can go back to it
  String radio_checkbox_arrow; //if menu is radio or checkbox
  bool radio_selected = false; //whether one value in the radio is selected (if the menu is radio)
};
