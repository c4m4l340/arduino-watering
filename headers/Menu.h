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

	bool hasChildren() {
		return childs!=NULL;
	};
};


class Menu {

private:	
	MenuItem* rootItem;
	int rootItemsCount;

	MenuItem* currentItem;
	MenuItem* currentLevelItems;
	int currentLevelItemsCount;

	unsigned long lastUpdateTime=0;
    
public:
	void* callerCallbackInstance = NULL;
	MenuItem getCurrentItem();
	
	Menu(MenuItem* root, int size);
	void begin();
	void update();

	void reset();
	void moveNext();
	void movePrevious();
	void enter();

	bool isCurrentItemRoot();
};


#endif

