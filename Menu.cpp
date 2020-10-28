#define DEBUG

#include "headers\Menu.h"
#include "headers\SerialDebug.h"

Menu::Menu(MenuItem* root, byte size) {
	this->rootItem = root;
	this->rootItemsCount = size;
	
}

void Menu::begin(){
	this->reset();
}

void Menu::update(){
	if(millis()-lastUpdateTime > 1000){
		lastUpdateTime = millis();
		 DPRINTLN_F("Menu::update(currentItem:%p, count:%d, childs:%d, isRoot:%d, action:%p, call_ptr:%p)", 
		 currentItem, currentLevelItemsCount, currentItem->hasChildren(), 
		 isCurrentItemRoot(), currentItem->action, this->callerCallbackInstance);
	}
}



MenuItem* Menu::getCurrentItem(){
	return currentItem;
}

void Menu::reset() {
	currentLevelItems = rootItem;
	currentLevelItemsCount = rootItemsCount;
	currentItem = rootItem;
}

void Menu::moveNext() {
	MenuItem *upperBoundary = currentLevelItems + currentLevelItemsCount - 1;
	if (currentItem < upperBoundary)
		currentItem++;
}

void Menu::movePrevious() {
	if (currentItem > currentLevelItems)
		currentItem--;
}

void Menu::enter() {
	if (currentItem->childs != NULL)
	{
		currentLevelItems = currentItem->childs;
		currentLevelItemsCount = currentItem->childsCount;
		currentItem = currentItem->childs;
	}else{
		if(currentItem->action != NULL){
			currentItem->action(currentItem->menuAction, callerCallbackInstance);
		}
	}
}


bool Menu::isCurrentItemRoot() {
	return currentItem == rootItem;
}
