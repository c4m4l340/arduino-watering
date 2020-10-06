#define DEBUG

#ifndef _SERIAL_DEBUG_H
#define _SERIAL_DEBUG_H


    #ifdef DEBUG    
        #define DINITSERIAL(...) Serial.begin(__VA_ARGS__);  while (!Serial)        
        #define DPRINT(...)  Serial.print(__VA_ARGS__)     //DPRINT is a macro, debug print
        #define DPRINTLN(...) Serial.println(__VA_ARGS__)   //DPRINTLN is a macro, debug print with new line
        #define DPRINTLN_F(...) char __FILE__msg__LINE__[100]; snprintf(__FILE__msg__LINE__, 100, __VA_ARGS__); Serial.println(__FILE__msg__LINE__)
    #else
        #define DPRINT(...)     //now defines a blank line
        #define DPRINTLN(...)   //now defines a blank line
        #define DINITSERIAL(...) //now defines a blank line
        #define DPRINTLN_F(...) //now defines a blank line
    #endif
#endif