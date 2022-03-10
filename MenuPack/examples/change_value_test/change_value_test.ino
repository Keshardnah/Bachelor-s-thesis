#include "Arduino.h"
#include <LiquidCrystal.h>
#include <MenuPack.h>

#define pin_RS 8
#define pin_EN 9
#define pin_d4 4
#define pin_d5 5
#define pin_d6 6
#define pin_d7 7
#define pin_BL 10

LiquidCrystal *my_lcd = new LiquidCrystal(pin_RS,  pin_EN,  pin_d4,  pin_d5,  pin_d6,  pin_d7);
MenuPack *menu_pack = new MenuPack;
menu *main_menu = new menu;
menu *change_value = new menu;
menu *selected_values = new menu;

void setup() {
  // put your setup code here, to run once:
  my_lcd->begin(16, 2);
  main_menu = menu_pack->makeMenu(2);

  change_value = menu_pack->makeMenu(4);
  menu_element *element = new menu_element;
  element = menu_pack->makeMenuElement("X (int):", ME_MENU);
  menu_pack->addElementToMenu(element, change_value);
  element = menu_pack->makeMenuElement("", ME_INT);
  menu_pack->initializeValue(element, new int(1), new int(0), new int(10), new int(1));
  menu_pack->addElementToMenu(element, change_value);
  element = menu_pack->makeMenuElement("Y (float):", ME_MENU);
  menu_pack->addElementToMenu(element, change_value);
  element = menu_pack->makeMenuElement("", ME_FLOAT);
  menu_pack->initializeValue(element, new float(1.5), new float(0.0), new float(10.0), new float(0.5));
  menu_pack->addElementToMenu(element, change_value);

  element = menu_pack->makeMenuElement("Select values", ME_PARENT);
  menu_pack->setActionFunctionFirst(element, [](){firstPrint(change_value);});
  menu_pack->setActionFunctionSecond(element, [](){loopPrint(change_value);});
  menu_pack->addElementToMenu(element, main_menu);
  menu_pack->addParentElement(element, change_value);

  selected_values = menu_pack->makeMenu(4);
  element = menu_pack->makeMenuElement("Show selected", ME_PARENT);
  menu_pack->setActionFunctionFirst(element, [](){firstPrintSelectedValues(selected_values, change_value);});
  menu_pack->setActionFunctionSecond(element, [](){loopPrint(selected_values);});
  menu_pack->addElementToMenu(element, main_menu);
  menu_pack->addParentElement(element, selected_values);

  firstPrint(main_menu);
}

void loop()
{
  loopPrint(main_menu);
}

void firstPrint(menu *menu) {
  printEachRow(menu_pack->print(menu, true));
}

void loopPrint(menu *menu) {
  if(menu_pack->checkButtons(menu)) { //find the currently active button
    printEachRow(menu_pack->print(menu, false)); //print a buffer
  }
  delay(100); //I put it on because the loop was spinning too fast
              //so when it cleared the screen and re-wrote you could see flickering.
              //With this delay there is no problem when checking buttons.
}

void firstPrintSelectedValues(menu *selected_values, menu *change_value) {
  //resets the menu so that by re-entering in it we would be at the beginning of the menu and not where we stopped
  menu_pack->resetMenu(selected_values);
  //delete all elements
  for(int i = 0; i < 4; i++) {
    menu_pack->deleteMenuElement(selected_values, 0);
  }
  menu_element *element = new menu_element;
  element = menu_pack->makeMenuElement("Current X:", ME_MENU);
  menu_pack->addElementToMenu(element, selected_values);
  String *value = new String(*(int*)(menu_pack->getValue(menu_pack->getMenuElement(change_value, 1))));
  element = menu_pack->makeMenuElement(*value, ME_MENU);
  menu_pack->addElementToMenu(element, selected_values);
  element = menu_pack->makeMenuElement("Current Y:", ME_MENU);
  menu_pack->addElementToMenu(element, selected_values);
  value = new String(*(float*)(menu_pack->getValue(menu_pack->getMenuElement(change_value, 3))));
  element = menu_pack->makeMenuElement(*value, ME_MENU);
  menu_pack->addElementToMenu(element, selected_values);
  printEachRow(menu_pack->print(selected_values, true));
}

void printEachRow(String buffer) {
  my_lcd->clear();
  my_lcd->setCursor(0,0);
  my_lcd->print(buffer);
  if(buffer.length()>16){
    my_lcd->setCursor(0,1);
    my_lcd->print(buffer.substring(16));
  }
}
