// Menu.h

#ifndef _MENU_h
#define _MENU_h

#include "Arduino.h"
#include "MenuActionBase.h"

using ActionCallback = void (*)(void* caller_ptr, byte index);
struct MenuItem {
	byte index;
	ActionCallback callback;
	String title;
	byte childsCount;
	MenuItem *childs;
	
	bool hasChildren() {
		return childs!=NULL;
	};
};


class Menu {

private:	
	MenuItem* rootItem;
	byte rootItemsCount;

	MenuItem* currentItem;
	MenuItem* currentLevelItems;
	byte currentLevelItemsCount;

	unsigned long lastUpdateTime=0;
    
public:
	void* callerCallbackInstance = NULL;
	MenuItem* getCurrentItem();
	
	Menu(MenuItem* root, byte size);
	void begin();
	void update();

	void reset();
	void moveNext();
	void movePrevious();
	void enter();

	bool isCurrentItemRoot();
};


#endif

