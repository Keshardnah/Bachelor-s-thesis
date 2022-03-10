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
menu *sub_menu = new menu;

void setup() {
  // put your setup code here, to run once:
  my_lcd->begin(16, 2);
  main_menu = menu_pack->makeMenu(6);
  menu_element *element = new menu_element;
  element = menu_pack->makeMenuElement("Main menu", ME_NAME);
  menu_pack->addElementToMenu(element, main_menu);
  element = menu_pack->makeMenuElement("Configuration", ME_MENU);
  menu_pack->addElementToMenu(element, main_menu);

  sub_menu = menu_pack->makeMenu(4);
  element = menu_pack->makeMenuElement("Low", ME_MENU);
  menu_pack->addElementToMenu(element, sub_menu);
  element = menu_pack->makeMenuElement("Medium", ME_MENU);
  menu_pack->addElementToMenu(element, sub_menu);
  element = menu_pack->makeMenuElement("High", ME_MENU);
  menu_pack->addElementToMenu(element, sub_menu);
  element = menu_pack->makeMenuElement("Back", ME_BACK);
  menu_pack->addElementToMenu(element, sub_menu);

  element = menu_pack->makeMenuElement("LCD Contrast", ME_PARENT);
  menu_pack->setActionFunctionFirst(element, [](){firstPrint(sub_menu);});
  menu_pack->setActionFunctionSecond(element, [](){loopPrint(sub_menu);});
  menu_pack->addElementToMenu(element, main_menu);
  menu_pack->addParentElement(element, sub_menu);

  element = menu_pack->makeMenuElement("Brightnesses", ME_MENU);
  menu_pack->addElementToMenu(element, main_menu);
  element = menu_pack->makeMenuElement("Dark mode", ME_MENU);
  menu_pack->addElementToMenu(element, main_menu);
  element = menu_pack->makeMenuElement("On top", ME_TOP);
  menu_pack->addElementToMenu(element, main_menu);

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

void printEachRow(String buffer) {
  my_lcd->clear();
  my_lcd->setCursor(0,0);
  my_lcd->print(buffer);
  if(buffer.length()>16){
    my_lcd->setCursor(0,1);
    my_lcd->print(buffer.substring(16));
  }
}
