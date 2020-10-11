// Menu.h

#ifndef _MENU_h
#define _MENU_h

#include "Arduino.h"

using ActionCallback = void (*)(void* caller_ptr);
struct MenuItem {
	ActionCallback action;
	String title;
	int childsCount;
	MenuItem *childs;
};


class Menu {

private:	
	MenuItem* rootItem;
	int rootItemsCount;

	MenuItem* currentItem;
	MenuItem* currentLevelItems;
	int currentLevelItemsCount;

    
public:
	void* callerCallbackInstance = NULL;
	MenuItem getCurrentItem();
	
	Menu(MenuItem* root);
	void begin();
	
	void reset();
	void moveNext();
	void movePrevious();
	void enter();

	bool hasSubItems();
	bool isRootItem();
};


#endif

