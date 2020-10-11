#ifndef __MENU_ITEMS_SETUP_H__
#define __MENU_ITEMS_SETUP_H__

#include "Arduino.h"
#include "Menu.h"

// enum MenuItemActionId :byte {
// 	ItemRunMode,
// 	ItemDateTime,
// 	ItemPrograms,
// 	ItemCheckSensor,
// 	ItemProgram1,
// 	ItemProgram2,
// 	//ItemProgram3,
// 	//ItemProgram4,
// 	ItemCheckAddress1,
// 	ItemCheckAddress2,
// 	ItemRun,
// 	ItemStop
// };

// MenuItem itemsCheckSensors[] = {
// 	{ MenuItemActionId::ItemCheckAddress1,"Check Address 1",0, NULL },
// 	{ MenuItemActionId::ItemCheckAddress2,"Check Address 2",0, NULL }
// };


// MenuItem itemsPrograms[] = {
// 	{ MenuItemActionId::ItemProgram1,"Program 1",0, NULL },
// 	{ MenuItemActionId::ItemProgram2,"Program 2",0, NULL },
// 	//{ MenuItemActionId::ItemProgram3,"Program 3",0, NULL },
// 	//{ MenuItemActionId::ItemProgram4,"Program 4",0, NULL },

// };

MenuItem itemsRoot[] = {
	{ NULL, "Run", 0,NULL},
    { NULL, "Stop", 0,NULL},
    { NULL, "Date/Time", 0,NULL},
    { NULL, "Programs", 0,NULL}

	// { MenuItemActionId::ItemRun,"Run",0, NULL },
	// { MenuItemActionId::ItemStop,"Stop",0, NULL },
	// { MenuItemActionId::ItemRunMode,"Run Mode",0, NULL },
	// { MenuItemActionId::ItemDateTime,"Date/Time",0, NULL },
	// { MenuItemActionId::ItemPrograms,"Programs",sizeof(itemsPrograms) / sizeof(MenuItem), itemsPrograms },
	// { MenuItemActionId::ItemCheckSensor,"Check Sensors",sizeof(itemsCheckSensors) / sizeof(MenuItem), itemsCheckSensors }
};


#endif