#ifndef __WATER_PROGRAM_H__
#define __WATER_PROGRAM_H__

#include "Arduino.h"
#include "LedRgb.h"
#include "Communications.h"

#define WATER_PROGRAM_STATUS_STANDBY 0
#define WATER_PROGRAM_SCHEDULED_RUNNING 1
#define WATER_PROGRAM_MANUAL_RUNNING 2



class WaterProgram{  

    private:
        byte previousStatus = WATER_PROGRAM_STATUS_STANDBY;
        byte currentStatus =  WATER_PROGRAM_STATUS_STANDBY;

        LedRgb* led;
        BlinkGroup blinkGroups[3] ={
            {{0,255,0,75},{0,0,0,2000}},
            {{0,0,255,500},{255,255,0,500}},
            {{0,255,255,500},{255,0,0,500}}
        };

        Communications* comms;
        static void onCommunicationStarted(byte packet[], int size, void* this_ptr);
        static void onCommunicationTimeout(void* this_ptr);
        static void onCommunicationReceived(byte buffer[], int size, void* this_ptr);
        

        //runs the program. Get values, evaluate and open valves if needed; 
        void runScheduled(); 

        //check if is time to run.
        bool isTimeToRun(int hours, int minutes, int seconds);
    public:
        WaterProgram(LedRgb* led, Communications* comms);

        void begin();
        void update();
        //Open the water
        void open(); 
        //Closes the water
        void close(); 
        //Abort a scheduled execution
        void abort(); 

        // Receive time and check if is time to run
        void check(int hours, int minutes, int seconds);
};

#endif