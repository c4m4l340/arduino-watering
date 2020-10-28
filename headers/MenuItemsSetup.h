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

MenuItem itemsCheckSensors[] = {
	{ NULL,"Check Addr 1",0, NULL },
	{ NULL,"Check Addr 2",0, NULL }
};

 MenuItem itemsPrograms[] = {
 	{ NULL,"Program 1",0, NULL },
 	{ NULL,"Program 2",0, NULL },
	{ NULL,"Program 3",0, NULL },
	{ NULL,"Program 4",0, NULL },
};

MenuItem itemsRoot[] = {
	{ NULL,"Open", 0, NULL},
    { NULL,"Stop", 0, NULL},
	//{ NULL, "Run Mode", 0, NULL},
    { NULL,"Date/Time", 0, NULL},
    { NULL, "Programs", sizeof(itemsPrograms) / sizeof(MenuItem), itemsPrograms},
	{ NULL,"Check Sensors", sizeof(itemsCheckSensors) / sizeof(MenuItem), itemsCheckSensors}
};


#endif