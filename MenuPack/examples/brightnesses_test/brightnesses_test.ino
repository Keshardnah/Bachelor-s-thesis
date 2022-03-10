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
menu *change_brightness = new menu;
menu *selected_brightness = new menu;

void setup() {
  // put your setup code here, to run once:
  pinMode(pin_BL, OUTPUT);
  analogWrite(pin_BL, 128);
  my_lcd->begin(16, 2);
  main_menu = menu_pack->makeMenu(2);

  change_brightness = menu_pack->makeMenu(2);
  menu_element *element = new menu_element;
  element = menu_pack->makeMenuElement("Set brightness:", ME_NAME);
  menu_pack->addElementToMenu(element, change_brightness);
  element = menu_pack->makeMenuElement("", ME_INT);
  menu_pack->initializeValue(element, new int(128), new int(0), new int(255), new int(32));
  menu_pack->setActionFunctionWhenChangeValue(element, [](){changeBrightness();});
  menu_pack->addElementToMenu(element, change_brightness);

  element = menu_pack->makeMenuElement("Brightnesses", ME_PARENT);
  menu_pack->setActionFunctionFirst(element, [](){firstPrint(change_brightness);});
  menu_pack->setActionFunctionSecond(element, [](){loopPrint(change_brightness);});
  menu_pack->addElementToMenu(element, main_menu);
  menu_pack->addParentElement(element, change_brightness);

  selected_brightness = menu_pack->makeMenu(2);
  element = menu_pack->makeMenuElement("Selected bright:", ME_NAME);
  menu_pack->addElementToMenu(element, selected_brightness);
  element = menu_pack->makeMenuElement("Show brightness", ME_PARENT);
  menu_pack->setActionFunctionFirst(element, [](){firstPrintSelectedBrightness(selected_brightness, change_brightness);});
  menu_pack->setActionFunctionSecond(element, [](){loopPrint(selected_brightness);});
  menu_pack->addElementToMenu(element, main_menu);
  menu_pack->addParentElement(element, selected_brightness);

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

void changeBrightness() {
  analogWrite(pin_BL, *(int*)(menu_pack->getNewValue(menu_pack->getMenuElement(change_brightness, 1))));
}

void firstPrintSelectedBrightness(menu *selected_brightness, menu *change_brightness) {
  //resets the menu so that by re-entering in it we would be at the beginning of the menu and not where we stopped
  menu_pack->resetMenu(selected_brightness);
  //delete all elements
  menu_pack->deleteMenuElement(selected_brightness, 1);
  menu_element *element = new menu_element;
  String *value = new String(*(int*)(menu_pack->getValue(menu_pack->getMenuElement(change_brightness, 1))));
  element = menu_pack->makeMenuElement(*value, ME_MENU);
  menu_pack->addElementToMenu(element, selected_brightness);
  printEachRow(menu_pack->print(selected_brightness, true));
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
