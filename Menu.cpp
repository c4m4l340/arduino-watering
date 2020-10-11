#include "headers\Menu.h"
#include "headers\MenuItemsSetup.h"

Menu::Menu() {
	this->rootItem = itemsRoot;
	this->rootItemsCount = sizeof(itemsRoot) / sizeof(MenuItem);
	this->reset();
}

MenuItem Menu::getCurrentItem(){
	return *currentItem;
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
		currentItem = currentLevelItems;
	}
}

bool Menu::hasSubItems() {
	return currentItem->childs != NULL;
}

bool Menu::isRootItem() {
	return currentItem == rootItem;
}

void Menu::setRoot(MenuItem root){
    this->rootItem = &root;
    this->rootItemsCount = sizeof(root) / sizeof(MenuItem);
}
