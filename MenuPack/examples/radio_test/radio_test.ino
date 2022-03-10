#include "Arduino.h"
#include <LiquidCrystal.h>
#include <MenuPack.h>

#define pin_RS 8
#define pin_EN 9
#define pin_d4 4
#define pin_d5 5
#define pin_d6 6
#define pin_d7 7

LiquidCrystal *my_lcd = new LiquidCrystal(pin_RS,  pin_EN,  pin_d4,  pin_d5,  pin_d6,  pin_d7);
MenuPack *menu_pack = new MenuPack;
menu *main_menu = new menu;
menu *radio = new menu;
//menu in which the item selected from the radio will be displayed
menu *selected_radio = new menu;

void setup() {
  // put your setup code here, to run once:
  my_lcd->begin(16, 2);
  main_menu = menu_pack->makeMenu(2);

  radio = menu_pack->makeMenu(3);
  menu_element *element = new menu_element;
  element = menu_pack->makeMenuElement("Choose one:", ME_NAME);
  menu_pack->addElementToMenu(element, radio);
  element = menu_pack->makeMenuElement("first", ME_RADIO);
  menu_pack->addElementToMenu(element, radio);
  element = menu_pack->makeMenuElement("second", ME_RADIO);
  menu_pack->addElementToMenu(element, radio);

  element = menu_pack->makeMenuElement("Radio", ME_PARENT);
  menu_pack->setActionFunctionFirst(element, [](){firstPrint(radio);});
  menu_pack->setActionFunctionSecond(element, [](){loopPrint(radio);});
  menu_pack->addElementToMenu(element, main_menu);
  menu_pack->addParentElement(element, radio);

  selected_radio = menu_pack->makeMenu(2);
  element = menu_pack->makeMenuElement("Selected:", ME_NAME);
  menu_pack->addElementToMenu(element, selected_radio);
  element = menu_pack->makeMenuElement("Selected", ME_PARENT);
  menu_pack->setActionFunctionFirst(element, [](){firstPrintSelectedRadio(selected_radio, radio);});
  menu_pack->setActionFunctionSecond(element, [](){loopPrint(selected_radio);});
  menu_pack->addElementToMenu(element, main_menu);
  menu_pack->addParentElement(element, selected_radio);

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

void firstPrintSelectedRadio(menu *selected_radio, menu *radio) {
  menu_pack->resetMenu(selected_radio);
  menu_pack->deleteMenuElement(selected_radio, 1);
  String *elem = menu_pack->getSelectedFromRadio(radio);
  if(elem) {
    menu_element *element = new menu_element;
    element = menu_pack->makeMenuElement(*elem, ME_MENU);
    menu_pack->addElementToMenu(element, selected_radio);
  }
  printEachRow(menu_pack->print(selected_radio, true));
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
