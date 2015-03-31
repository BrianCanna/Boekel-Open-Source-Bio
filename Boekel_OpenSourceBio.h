/// @todo Add software license
#ifndef _BOEKEL_OPENSOURCEBIO_H_
#define _BOEKEL_OPENSOURCEBIO_H_

#include <Arduino.h>
#include <string.h>
#include <Boekel_OpenSourceBio_Structures.h>

namespace Boekel{
    class OpenSourceBio{

    public:

        OpenSourceBio();

        void holdScreen(),
            releaseScreen(),
            clearScreen(unsigned char _color),
            displayFilledRectangle(unsigned int _x, unsigned char _y, unsigned int _width, unsigned char _height, unsigned char _forecolor, unsigned char _backcolor, unsigned char _thickness),
            drawGraphBars(unsigned int _x, unsigned char _y, unsigned int _width, unsigned char _height, unsigned char _pointcount, unsigned char* _points, unsigned char _forecolor,unsigned char _backcolor),
            drawGraphStep(unsigned int _x, unsigned char _y, unsigned int _width, unsigned char _height, unsigned char _pointcount, unsigned char* _points, unsigned char _forecolor,unsigned char _backcolor),
            updateReadings(),
            displayText(unsigned int _x, unsigned int _y, unsigned char _forecolor, unsigned char _backcolor, const char* _text);

        bool stepGraph(unsigned int sampleTimeMinutes, const uint8_t type, int maxValueExpected, unsigned int sampleSize = DEFAULT_GRAPH_POINTS),
             barGraph(unsigned int sampleTimeMinutes, const uint8_t type, int maxValueExpected, unsigned int sampleSize = DEFAULT_GRAPH_POINTS);
            
        struct time_data_t* getTime();

        unsigned long getTimeRaw();

        unsigned char getHours(),
            getMinutes(), 
            getSeconds(), 
            getDay(), 
            getMonth();

        const char* getWeekDay();

        unsigned int getYear();

        char getReadingType(),
            getReadingValid(),
            getTemperatureValid();

        double getTemperature(),
            getPH(),
            getPHmV(),
            getEC(),
            getECpss(),
            getECtds(),
            getDOmgl(),
            getDOpercentage();

        static const uint8_t COLOR_BLACK = 0,
                            COLOR_WHITE = 1,
                            COLOR_GREEN = 2,
                            COLOR_RED = 3,
                            COLOR_BLUE = 4,
                            COLOR_YELLOW = 5,
                            COLOR_PURPLE = 6,
                            COLOR_CYAN = 7,
                            COLOR_GREY = 8,
                            COLOR_DARKGREY = 9,
                            COLOR_TRANSPARENT = 0x80,
                            READING_TYPE_PH = 'P',
                            READING_TYPE_EC = 'E',
                            READING_TYPE_DO = 'D',
                            READING_TYPE_TEMPERATURE = 'T';

        static const uint8_t MAX_GRAPH_POINTS = 250,
                             DEFAULT_GRAPH_POINTS = 64;

    protected:

    private:
        void setForeColor(unsigned char _color),
            setBackColor(unsigned char _color),
            waitForReady(),
            waitForResponse(unsigned char* _data, unsigned char _size),
            dataBuffer(unsigned char _count, const unsigned char* _data),
            sendExtent(unsigned int _x, unsigned char _y),
            drawText(unsigned int _x, unsigned char _y, const char* _text),
            drawRect(unsigned int _x, unsigned char _y, unsigned int _width, unsigned char _height, unsigned char _forecolor ,unsigned char _thickness),
            drawFilledRectangle(unsigned int _x, unsigned char _y, unsigned int _width, unsigned char _height, unsigned char _backcolor );

        reading_data_t osb_reading;
        time_data_t osb_time;
        timet_data_t osb_timet;
        
        unsigned long time_t; // raw time

        unsigned char forecolor, backcolor;

        // I2C related constants
        static const uint8_t I2C_ADDRESS = 0x10,
                            I2C_COMMAND_DRAWRECT = 0x1,
                            I2C_COMMAND_DRAWFILLEDRECT = 0x2,
                            I2C_COMMAND_DRAWTEXT = 0x3,
                            I2C_COMMAND_CLEARSCREEN = 0x4,
                            I2C_COMMAND_HOLDSCREEN = 0x5,
                            I2C_COMMAND_RELEASESCREEN = 0x6,
                            I2C_COMMAND_DRAWGRAPH_BAR = 32,
                            I2C_COMMAND_DRAWGRAPH_STEP = 33,
                            I2C_COMMAND_DATABUFFER = 64,
                            I2C_COMMAND_READ = 128,
                            I2C_COMMAND_TIME = 129,
                            I2C_COMMAND_TIMET = 130,
                            I2C_STATUS_IDLE = 0x0,
                            I2C_STATUS_BUSY = 0x1,
                            I2C_STATUS_RESPONSE = 0x2,
                            I2C_STATUS_BADCOMMAND = 0x254,
                            I2C_MAX_DATABUFFER = 28;

    };
}
#endif
