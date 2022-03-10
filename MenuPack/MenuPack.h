/*
  Menu.h - Library for menu.
  Created by Ana Andrašek, April 5, 2020.
*/

#pragma once

#include <Buttons.h>
#include <Menu.h>
#include <lcd.h>

class MenuPack
{
private:
  //currently active button
  enum Button button;
  //button that was previously active
  enum Button last_button;

  //is called when some button is pressed and performs a specific action for each button
  void menu_event(menu *menu);
  //uses the library Buttons.h
  Buttons *buttons;

  //buffer in which is written what we want to print on the LCD
  String buffer;
  lcd *device;
  //update the buffer so that it can be printed on the lcd
  void updateBuffer(menu *menu);
  //is called inside the updateBuffer function to set a good number of spaces at the end of the row
  void setSpacesOnEnd(MenuElementType type, int columns, int name_length, int arrow_length, int radio_checkbox_arrow_length,
                                  String *buffer);
  //is called inside the updateBuffer function to set prefix
  void setPrefix(menu_element *menu_element, int arrow_length, int radio_checkbox_arrow_length,
                            String radio_checkbox_arrow, String *buffer);
  //update the menu when we press the down button
  bool updateMenuDown(menu *menu);
  //update the menu when we press the up button
  bool updateMenuUp(menu *menu);
  //selects the action to be performed if we press the select button
  bool chooseAction(menu *menu);
  bool updateChangeBack(menu *menu);

public:
  //the constructor in which the assumed values are set
  MenuPack();
  menu *makeMenu(int max_menu_elements);
  //resets the menu so that by re-entering in it (if it is a sub menu) we would be at the beginning of the menu and not where we stopped
  void resetMenu(menu *menu);
  void deleteMenuElement(menu *menu, int position);
  menu_element *makeMenuElement(String name, MenuElementType type);
  void initializeValue(menu_element *menu_element, void *value, void *min, void *max, void *increment);
  void addElementToMenu(menu_element *menu_element, menu *menu);
  void setActionFunctionFirst(menu_element *element, void (*actionFunctionFirst)());
  void setActionFunctionSecond(menu_element *element, void (*actionFunctionSecond)());
  void setActionFunctionWhenChangeValue(menu_element *menu_element, void (*actionFunctionWhenChangeValue)());
  //adding parent element to sub menu
  void addParentElement(menu_element *element, menu *menu);
  //called in the arduino program to check if some button is pressed
  bool checkButtons(menu *menu);
  //returns the buffer
  String print(menu *menu, boolean first_display);
  //returns the name of the selected element from the radio
  String *getSelectedFromRadio(menu *radio);
  //returns the names of the elements selected from the checkbox
  String **getSelectedFromCheckbox(menu *checkbox);
  void *getValue(menu_element *menu_element);
  void *getNewValue(menu_element *menu_element);
  menu_element *getMenuElement(menu *menu, int position);
};
