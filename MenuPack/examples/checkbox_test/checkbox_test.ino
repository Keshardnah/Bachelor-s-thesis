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
menu *checkbox = new menu;
//menu in which the items selected from the checkbox will be displayed
menu *selected_checkbox = new menu;

void setup() {
  // put your setup code here, to run once:
  my_lcd->begin(16, 2);
  main_menu = menu_pack->makeMenu(2);

  checkbox = menu_pack->makeMenu(3);
  menu_element *element = new menu_element;
  element = menu_pack->makeMenuElement("Choose more:", ME_NAME);
  menu_pack->addElementToMenu(element, checkbox);
  element = menu_pack->makeMenuElement("first", ME_CHECKBOX);
  menu_pack->addElementToMenu(element, checkbox);
  element = menu_pack->makeMenuElement("second", ME_CHECKBOX);
  menu_pack->addElementToMenu(element, checkbox);

  element = menu_pack->makeMenuElement("Checkbox", ME_PARENT);
  menu_pack->setActionFunctionFirst(element, [](){firstPrint(checkbox);});
  menu_pack->setActionFunctionSecond(element, [](){loopPrint(checkbox);});
  menu_pack->addElementToMenu(element, main_menu);
  menu_pack->addParentElement(element, checkbox);

  selected_checkbox = menu_pack->makeMenu(3);
  element = menu_pack->makeMenuElement("Selected:", ME_NAME);
  menu_pack->addElementToMenu(element, selected_checkbox);
  element = menu_pack->makeMenuElement("Selected", ME_PARENT);
  menu_pack->setActionFunctionFirst(element, [](){firstPrintSelectedCheckbox(selected_checkbox, checkbox);});
  menu_pack->setActionFunctionSecond(element, [](){loopPrint(selected_checkbox);});
  menu_pack->addElementToMenu(element, main_menu);
  menu_pack->addParentElement(element, selected_checkbox);

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

void firstPrintSelectedCheckbox(menu *selected_checkbox, menu *checkbox) {
  //resets the menu so that by re-entering in it we would be at the beginning of the menu and not where we stopped
  menu_pack->resetMenu(selected_checkbox);
  //delete all elements except the title
  menu_pack->deleteMenuElement(selected_checkbox, 2);
  menu_pack->deleteMenuElement(selected_checkbox, 1);
  //fetch new elements that are selected in the checkbox
  String **elem = menu_pack->getSelectedFromCheckbox(checkbox);
  while(*elem) {
    menu_element *element = new menu_element;
    element = menu_pack->makeMenuElement(**elem, ME_MENU);
    //adding new elements to the menu
    menu_pack->addElementToMenu(element, selected_checkbox);
    elem++;
  }
  printEachRow(menu_pack->print(selected_checkbox, true));
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
