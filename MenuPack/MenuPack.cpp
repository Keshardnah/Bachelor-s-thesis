/*
  Menu.h - Library for menu.
  Created by Ana Andrašek, April 5, 2020.
*/

#include "MenuPack.h"

MenuPack::MenuPack() {
  buttons = new Buttons;
  button = BT_NONE;
  last_button = BT_NONE;
  device = new lcd;
  buttons->check_the_existence_of_buttons();
}

menu* MenuPack::makeMenu(int max_menu_elements) {
  menu *new_menu = new menu;
  #ifdef SELECTED_ARROW
    new_menu->arrow = SELECTED_ARROW;
  #else
    new_menu->arrow = "> ";
  #endif
  new_menu->parent = new menu_element;
  new_menu->parent = NULL;
  #ifdef RADIO_CHECKBOX_ARROW
    new_menu->radio_checkbox_arrow = RADIO_CHECKBOX_ARROW;
  #else
    new_menu->radio_checkbox_arrow = "x";
  #endif
  new_menu->max_menu_elements = max_menu_elements;
  new_menu->menuElements = new menu_element*[max_menu_elements];
  for(int i=0; i < new_menu->max_menu_elements; i++){
    new_menu->menuElements[i] = NULL;
  }
  return new_menu;
}

void MenuPack::resetMenu(menu *menu) {
  menu->current_element = 0;
  menu->current_row = 0;
}

void MenuPack::deleteMenuElement(menu *menu, int position) {
  if(position < menu->max_menu_elements) {
    int i;
    for(i = position; i < menu->max_menu_elements - 1; i++) {
      if(!menu->menuElements[i]) break;
      menu->menuElements[i] = menu->menuElements[i + 1];
    }
    menu->menuElements[i] = NULL;
  }
}

menu_element* MenuPack::makeMenuElement(String name, MenuElementType type) {
  menu_element *element = new menu_element;
  element->name = name;
  element->type = type;
  return element;
}

void MenuPack::initializeValue(menu_element *menu_element, void *value, void *min, void *max, void *increment) {
  switch (menu_element->type) {
    case ME_INT:
      menu_element->value.int_value.value = *(int*)value;
      menu_element->value.int_value.new_value = *(int*)value;
      menu_element->value.int_value.min = *(int*)min;
      menu_element->value.int_value.max = *(int*)max;
      menu_element->value.int_value.increment = *(int*)increment;
      break;
    case ME_FLOAT:
      menu_element->value.float_value.value = *(float*)value;
      menu_element->value.float_value.new_value = *(float*)value;
      menu_element->value.float_value.min = *(float*)min;
      menu_element->value.float_value.max = *(float*)max;
      menu_element->value.float_value.increment = *(float*)increment;
      break;
  }
}

void MenuPack::setActionFunctionWhenChangeValue(menu_element *menu_element, void (*actionFunctionWhenChangeValue)()) {
  menu_element->actionFunctionWhenChangeValue = actionFunctionWhenChangeValue;
  menu_element->action_function_when_change_value_exist = true;
}

void MenuPack::addElementToMenu(menu_element *menu_element, menu *menu) {
    for (int i = 0; i < menu->max_menu_elements; i++) {
      if (menu->menuElements[i] == NULL) {
        menu->menuElements[i] = menu_element;
        break;
      }
    }
}

void MenuPack::setActionFunctionFirst(menu_element *element, void (*actionFunctionFirst)()) {
	element->actionFunctionFirst = actionFunctionFirst;

}

void MenuPack::setActionFunctionSecond(menu_element *element, void (*actionFunctionSecond)()) {
	element->actionFunctionSecond = actionFunctionSecond;
}

void MenuPack::addParentElement(menu_element *element, menu *menu) {
  menu->parent = element;
}

bool MenuPack::checkButtons(menu *menu) {
  button = buttons->detectButton(); //type the currently active button or NONE if there is none active
  if(button != BT_NONE) menu_event(menu); //if some button is pressed
  else {
    last_button = BT_NONE; //last_button is actually the current button
    return false;
  }
  if(menu->parent != NULL) {
    if(menu->parent->back) return false;
  }
  return true;
}

void MenuPack::menu_event(menu *menu) {
  switch (button) { //find the button that is currently active
    case BT_SELECT:
      if(last_button == BT_SELECT) break; //to avoid performing the function repeatedly if the button is held longer
      last_button = BT_SELECT;
      if(chooseAction(menu)) updateBuffer(menu);
      break;
    case BT_BACK:
      if(last_button == BT_BACK) break;
      last_button = BT_BACK;
      if(updateChangeBack(menu)) updateBuffer(menu);
      else {
        if(menu->parent != NULL) menu->parent->back = true;
      }
      break;
    case BT_UP:
      if(last_button == BT_UP) break;
      last_button = BT_UP;
      if(updateMenuUp(menu)) updateBuffer(menu);
      break;
    case BT_DOWN:
      if(last_button == BT_DOWN) break;
      last_button = BT_DOWN;
      if(updateMenuDown(menu)) updateBuffer(menu);
      break;
    case BT_RIGHT:
      if(last_button == BT_RIGHT) break;
      last_button = BT_RIGHT;
      resetMenu(menu);
      updateBuffer(menu);
      break;
    }
}

bool MenuPack::updateChangeBack(menu *menu) {
  if(menu->menuElements[menu->current_element]->change_value) {
    menu->menuElements[menu->current_element]->change_value = false;
    switch (menu->menuElements[menu->current_element]->type) {
      case ME_INT:
        menu->menuElements[menu->current_element]->value.int_value.new_value =
                menu->menuElements[menu->current_element]->value.int_value.value;
        if(menu->menuElements[menu->current_element]->action_function_when_change_value_exist)
          menu->menuElements[menu->current_element]->actionFunctionWhenChangeValue();
        return true;
      case ME_FLOAT:
        menu->menuElements[menu->current_element]->value.float_value.new_value =
              menu->menuElements[menu->current_element]->value.float_value.value;
        if(menu->menuElements[menu->current_element]->action_function_when_change_value_exist)
          menu->menuElements[menu->current_element]->actionFunctionWhenChangeValue();
        return true;
    }
  }
  return false;
}

bool MenuPack::updateMenuDown(menu *menu) {
  if(menu->menuElements[menu->current_element]->change_value) {
    switch (menu->menuElements[menu->current_element]->type) {
      case ME_INT:
        int new_int_value;
        if((new_int_value = menu->menuElements[menu->current_element]->value.int_value.new_value -
            menu->menuElements[menu->current_element]->value.int_value.increment) >=
              menu->menuElements[menu->current_element]->value.int_value.min) {
                menu->menuElements[menu->current_element]->value.int_value.new_value = new_int_value;
                if(menu->menuElements[menu->current_element]->action_function_when_change_value_exist)
                  menu->menuElements[menu->current_element]->actionFunctionWhenChangeValue();
                return true;
        }
        return false;
      case ME_FLOAT:
        float new_float_value;
        if((new_float_value = menu->menuElements[menu->current_element]->value.float_value.new_value -
            menu->menuElements[menu->current_element]->value.float_value.increment) >=
              menu->menuElements[menu->current_element]->value.float_value.min) {
                menu->menuElements[menu->current_element]->value.float_value.new_value = new_float_value;
                if(menu->menuElements[menu->current_element]->action_function_when_change_value_exist)
                  menu->menuElements[menu->current_element]->actionFunctionWhenChangeValue();
                return true;
        }
        return false;
    }
  }
  else {
    menu->current_element++;
    if(menu->current_element < menu->max_menu_elements) {
      if (!(menu->menuElements[menu->current_element] == NULL)) {
        if(device->rows - (menu->current_row + 1)) menu->current_row++;
        return true;
      }
      else {
        menu->current_element--;
      }
    }
    else {
      menu->current_element--;
    }
    return false;
  }
}

bool MenuPack::updateMenuUp(menu *menu) {
  if(menu->menuElements[menu->current_element]->change_value) {
    switch (menu->menuElements[menu->current_element]->type) {
      case ME_INT:
        int new_int_value;
        if((new_int_value = menu->menuElements[menu->current_element]->value.int_value.new_value +
            menu->menuElements[menu->current_element]->value.int_value.increment) <=
              menu->menuElements[menu->current_element]->value.int_value.max) {
                menu->menuElements[menu->current_element]->value.int_value.new_value = new_int_value;
                if(menu->menuElements[menu->current_element]->action_function_when_change_value_exist)
                  menu->menuElements[menu->current_element]->actionFunctionWhenChangeValue();
                return true;
        }
        return false;
      case ME_FLOAT:
        float new_float_value;
        if((new_float_value = menu->menuElements[menu->current_element]->value.float_value.new_value +
          menu->menuElements[menu->current_element]->value.float_value.increment) <=
            menu->menuElements[menu->current_element]->value.float_value.max) {
              menu->menuElements[menu->current_element]->value.float_value.new_value = new_float_value;
              if(menu->menuElements[menu->current_element]->action_function_when_change_value_exist)
                menu->menuElements[menu->current_element]->actionFunctionWhenChangeValue();
              return true;
        }
        return false;
    }
  }
  else {
    menu->current_element--;
    if (menu->current_element < 0) {
      menu->current_element++;
    }
    else {
      menu->current_row--;
      if(menu->current_row < 0) menu->current_row++;
      return true;
    }
    return false;
  }
}

bool MenuPack::chooseAction(menu *menu) {
  switch (menu->menuElements[menu->current_element]->type) {
    case ME_PARENT: //enter the sub menu and performs functions in the sub menu
      menu->menuElements[menu->current_element]->actionFunctionFirst();
      while(true) {
        menu->menuElements[menu->current_element]->actionFunctionSecond();
        if(menu->menuElements[menu->current_element]->back) {
          menu->menuElements[menu->current_element]->back = false;
          break;
        }
      }
      return true;
    case ME_BACK: //if the return to the higher menu is realized using the element
      menu->parent->back = true;
      return false;
    case ME_CHECKBOX: //the element can be selected or unselected
      if(menu->menuElements[menu->current_element]->radio_checkbox_selected) {
        menu->menuElements[menu->current_element]->radio_checkbox_selected = false;
        return true;
      }
      menu->menuElements[menu->current_element]->radio_checkbox_selected = true;
      return true;
    case ME_RADIO: //only one element can be selected
      if(!menu->radio_selected) { //if no element is selected in the radio
        menu->menuElements[menu->current_element]->radio_checkbox_selected = true;
        menu->radio_selected = true;
        return true;
      }
      if(menu->menuElements[menu->current_element]->radio_checkbox_selected) {
        menu->menuElements[menu->current_element]->radio_checkbox_selected = false;
        menu->radio_selected = false;
        return true;
      }
      return false;
    case ME_INT:
      if(!menu->menuElements[menu->current_element]->change_value) {
        menu->menuElements[menu->current_element]->change_value = true;
      }
      else {
        menu->menuElements[menu->current_element]->value.int_value.value =
            menu->menuElements[menu->current_element]->value.int_value.new_value;
      }
      return false;
    case ME_FLOAT:
      if(!menu->menuElements[menu->current_element]->change_value) {
        menu->menuElements[menu->current_element]->change_value = true;
      }
      else {
        menu->menuElements[menu->current_element]->value.float_value.value =
          menu->menuElements[menu->current_element]->value.float_value.new_value;
      }
      return false;
    case ME_TOP:
      resetMenu(menu);
      return true;
    default:
      return false;
  }
}

String* MenuPack::getSelectedFromRadio(menu *radio) {
  if(radio->radio_selected) {
    for(int i=0; i < radio->max_menu_elements; i++) {
      if(radio->menuElements[i]->radio_checkbox_selected) return &(radio->menuElements[i]->name);
    }
  }
  return NULL;
}

String** MenuPack::getSelectedFromCheckbox(menu *checkbox) {
  String **selected_checkbox = new String*[checkbox->max_menu_elements];
  for(int i=0; i < checkbox->max_menu_elements; i++) {
    selected_checkbox[i] = NULL;
  }
  int position = 0;
  for(int i=0; i < checkbox->max_menu_elements; i++) {
    if(checkbox->menuElements[i]->radio_checkbox_selected) {
      selected_checkbox[position] = &(checkbox->menuElements[i]->name);
      position++;
    }
  }
  return selected_checkbox;
}

void* MenuPack::getValue(menu_element *menu_element) {
  switch (menu_element->type) {
    case ME_INT:
      return &(menu_element->value.int_value.value);
    case ME_FLOAT:
      return &(menu_element->value.float_value.value);
  }
}

void* MenuPack::getNewValue(menu_element *menu_element) {
  switch (menu_element->type) {
    case ME_INT:
      return &(menu_element->value.int_value.new_value);
    case ME_FLOAT:
      return &(menu_element->value.float_value.new_value);
  }
}

menu_element* MenuPack::getMenuElement(menu *menu, int position) {
  return menu->menuElements[position];
}

String MenuPack::print(menu *menu, boolean first_display) {
  if(first_display) updateBuffer(menu);
  return buffer;
}

void MenuPack::updateBuffer(menu *menu) {
  int current_row = menu->current_row;
  int current_element = menu->current_element;
  int columns = device->columns;
  int rows = device->rows;
  int up_rows = current_row;
  int down_rows = rows - 1 - up_rows;
  buffer = "";
  int name_length;
  int distance;
  int arrow_length = menu->arrow.length();
  int radio_checkbox_arrow_length = menu->radio_checkbox_arrow.length();
  if(up_rows) { //if above are rows to fill
    for(int row=0; row!=current_row; row++){
        distance = current_row - row;
        setPrefix(menu->menuElements[current_element-distance], arrow_length, radio_checkbox_arrow_length,
                    menu->radio_checkbox_arrow, &buffer);
        if(menu->menuElements[current_element-distance]->type == ME_INT ||
              menu->menuElements[current_element-distance]->type == ME_FLOAT) {
          name_length = buffer.length();
          if(menu->menuElements[current_element-distance]->name != "") {
            buffer.concat(menu->menuElements[current_element-distance]->name);
          }
          if(menu->menuElements[current_element-distance]->type == ME_FLOAT) {
            buffer.concat(menu->menuElements[current_element-distance]->value.float_value.value);
          }
          else {
            buffer.concat(menu->menuElements[current_element-distance]->value.int_value.value);
          }
          name_length = buffer.length() - name_length;
        }
        else {
          buffer.concat(menu->menuElements[current_element-distance]->name);
          name_length = menu->menuElements[current_element-distance]->name.length();
          if(menu->menuElements[current_element-distance]->type == ME_PARENT) {
            buffer.concat(">");
            name_length = name_length + 1;
          }
        }
        setSpacesOnEnd(menu->menuElements[current_element-distance]->type, columns, name_length, arrow_length,
                            radio_checkbox_arrow_length, &buffer);
    }
  }
  if(menu->menuElements[current_element]->type != ME_NAME) {
    buffer.concat(menu->arrow);
    if(menu->menuElements[current_element]->type == ME_RADIO || menu->menuElements[current_element]->type == ME_CHECKBOX) {
      if(menu->menuElements[current_element]->radio_checkbox_selected) {
        buffer.concat(menu->radio_checkbox_arrow);
      }
      else {
        for(int i=radio_checkbox_arrow_length; i>0; i--){
          buffer.concat(" ");
        }
      }
    }
  }
  if(menu->menuElements[current_element]->type == ME_INT || menu->menuElements[current_element]->type == ME_FLOAT) {
    name_length = buffer.length();
    if(menu->menuElements[current_element]->name != "") {
      buffer.concat(menu->menuElements[current_element]->name);
    }
    if(menu->menuElements[current_element]->change_value) {
      if(menu->menuElements[current_element]->type == ME_FLOAT) {
        buffer.concat(menu->menuElements[current_element]->value.float_value.new_value);
      }
      else {
        buffer.concat(menu->menuElements[current_element]->value.int_value.new_value);
      }
    }
    else {
      if(menu->menuElements[current_element]->type == ME_FLOAT) {
        buffer.concat(menu->menuElements[current_element]->value.float_value.value);
      }
      else {
        buffer.concat(menu->menuElements[current_element]->value.int_value.value);
      }
    }
    name_length = buffer.length() - name_length;
  }
  else {
    buffer.concat(menu->menuElements[current_element]->name);
    name_length = menu->menuElements[current_element]->name.length();
    if(menu->menuElements[current_element]->type == ME_PARENT) {
      buffer.concat(">");
      name_length = name_length + 1;
    }
  }
  setSpacesOnEnd(menu->menuElements[current_element]->type, columns, name_length, arrow_length,
                      radio_checkbox_arrow_length, &buffer);
  distance = 1; //current_element to print
  while(down_rows) { //if down are rows to fill
    if(current_element+distance == menu->max_menu_elements) break;
    if(menu->menuElements[current_element+distance] == NULL) break;
    setPrefix(menu->menuElements[current_element+distance], arrow_length, radio_checkbox_arrow_length,
                menu->radio_checkbox_arrow, &buffer);
    if(menu->menuElements[current_element+distance]->type == ME_INT ||
          menu->menuElements[current_element+distance]->type == ME_FLOAT) {
      name_length = buffer.length();
      if(menu->menuElements[current_element+distance]->name != "") {
        buffer.concat(menu->menuElements[current_element+distance]->name);
      }
      if(menu->menuElements[current_element+distance]->type == ME_FLOAT) {
        buffer.concat(menu->menuElements[current_element+distance]->value.float_value.value);
      }
      else {
        buffer.concat(menu->menuElements[current_element+distance]->value.int_value.value);
      }
      name_length = buffer.length() - name_length;
    }
    else {
      buffer.concat(menu->menuElements[current_element+distance]->name);
      name_length = menu->menuElements[current_element+distance]->name.length();
      if(menu->menuElements[current_element+distance]->type == ME_PARENT) {
        buffer.concat(">");
        name_length = name_length + 1;
      }
    }
    setSpacesOnEnd(menu->menuElements[current_element+distance]->type, columns, name_length, arrow_length,
                        radio_checkbox_arrow_length, &buffer);
    down_rows--;
    distance++;
  }
  }

  void MenuPack::setPrefix(menu_element *menu_element, int arrow_length, int radio_checkbox_arrow_length,
                            String radio_checkbox_arrow, String *buffer) {
    if(menu_element->type != ME_NAME) {
      for(int i=arrow_length; i>0; i--){
        buffer->concat(" ");
      }
      if(menu_element->type == ME_RADIO || menu_element->type == ME_CHECKBOX) {
        if(menu_element->radio_checkbox_selected) {
          buffer->concat(radio_checkbox_arrow);
        }
        else {
          for(int i=radio_checkbox_arrow_length; i>0; i--){
            buffer->concat(" ");
          }
        }
      }
    }
  }

  void MenuPack::setSpacesOnEnd(MenuElementType type, int columns, int name_length, int arrow_length, int radio_checkbox_arrow_length,
                                  String *buffer) {
    if(type == ME_RADIO || type == ME_CHECKBOX) {
      for(int i=columns - name_length - arrow_length - radio_checkbox_arrow_length; i>0; i--){
        buffer->concat(" ");
      }
    }
    else {
      if(type != ME_NAME) {
        for(int i=columns - name_length - arrow_length; i>0; i--){
          buffer->concat(" ");
        }
      }
      else {
        for(int i=columns - name_length; i>0; i--){
          buffer->concat(" ");
        }
      }
    }
  }
