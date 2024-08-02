#pragma once
#include "defns.h"
#include "types.h"
#include <string>


int param(Menu p);
int clampValToMenu(Menu p, int val);
void setParam(Menu p, int val);
void changeParam(Menu p, int delta);
void customDisplay(Menu p);
Menu currMenu();
int numMenus();
void nextMenu();
void prevMenu();
const MenuItem& menuItem(Menu m);
const MenuItem& currMenuItem();
std::string currSubMenuName();
