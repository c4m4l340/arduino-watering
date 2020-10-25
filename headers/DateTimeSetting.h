#ifndef __DATE_TIME_SETTING__
#define __DATE_TIME_SETTING__


#define DATETIMESETTING_YEAR 0
#define DATETIMESETTING_MONTH 1
#define DATETIMESETTING_DAY 2
#define DATETIMESETTING_HOUR 3
#define DATETIMESETTING_MINUTE 4

class DateTimeSetting{

    private:
        int year;
        int month;
        int day;
        int hour;
        int minute;
        
        int currentStatus;
        long lastBlinkUpdateTime;

    public:
        void begin();
        void update();
        
        void init();
        void up();
        void down();
        void next();

        void getSettedValue(int* year, int* month, int* day, int* hour, int* minute);
};

#endif