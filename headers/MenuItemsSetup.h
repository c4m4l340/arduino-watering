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
	{0, NULL,"Check Addr 1",0, NULL },
	{1, NULL,"Check Addr 2",0, NULL }
};

 MenuItem itemsPrograms[] = {
 	{0, NULL,"Scheduler 1",0, NULL },
 	{1, NULL,"Scheduler 2",0, NULL },
	{2, NULL,"Scheduler 3",0, NULL },
	{3, NULL,"Scheduler 4",0, NULL },
};

MenuItem itemsRoot[] = {
	{0, NULL,"Open", 0, NULL},
    {1, NULL,"Stop", 0, NULL},
	//{ NULL, "Run Mode", 0, NULL},
    {2, NULL,"Date/Time", 0, NULL},
    {3, NULL,"Schedulers", sizeof(itemsPrograms) / sizeof(MenuItem), itemsPrograms},
	{4, NULL,"Check Sensors", sizeof(itemsCheckSensors) / sizeof(MenuItem), itemsCheckSensors}
};


#endif