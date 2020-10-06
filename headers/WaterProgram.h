#ifndef __WATER_PROGRAM_H__
#define __WATER_PROGRAM_H__

#include "Arduino.h"
#include "LedRgb.h"


#define WATER_PROGRAM_STATUS_STANDBY 0
#define WATER_PROGRAM_SCHEDULED_RUNNING 1
#define WATER_PROGRAM_MANUAL_RUNNING 2



class WaterProgram{  

    private:
        byte previousStatus = WATER_PROGRAM_STATUS_STANDBY;
        byte currentStatus =  WATER_PROGRAM_STATUS_STANDBY;

        LedRgb* Led;
        BlinkGroup blinkGroups[3] ={
            {{0,255,0,75},{0,0,0,2000}},
            {{0,0,255,500},{255,255,0,500}},
            {{0,255,255,500},{255,0,0,500}}
        };

        //void run(); //runs the program. Get values, evaluate and open valves if needed; 
    public:
        WaterProgram(LedRgb* Led);

        void begin();
        void update();
        //void open(); //Open the water
        //void close(); //Closes the water
        //void abort(); //Abort a running execution
};

#endif