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
        void setForeColor(unsigned char _color);
        void setBackColor(unsigned char _color);
        void drawRect(unsigned int _x, unsigned char _y, unsigned int _width, unsigned char _height, unsigned char _thickness);
        void holdScreen();
        void releaseScreen();
        void clearScreen(unsigned char _color);
        void drawFilledRect(unsigned int _x, unsigned char _y, unsigned int _width, unsigned char _height);
        void drawText(unsigned int _x, unsigned char _y, const char* _text);
        void drawGraphBars(unsigned int _x, unsigned char _y, unsigned int _width, unsigned char _height, unsigned char _pointcount, unsigned char* _points);
        void drawGraphStep(unsigned int _x, unsigned char _y, unsigned int _width, unsigned char _height, unsigned char _pointcount, unsigned char* _points);
        void updateReadings();
        struct time_data_t* getTime();
        unsigned long getTimeRaw();
        char getReadingType();
        char getReadingValid();
        char getTemperatureValid();
        double getTemperature();
        double getPH();
        double getPHmV();
        double getEC();
        double getECpss();
        double getECtds();
        double getDOmgl();
        double getDOpercentage();

        static const uint8_t COLOR_BLACK = 0;
        static const uint8_t COLOR_WHITE = 1;
        static const uint8_t COLOR_GREEN = 2;
        static const uint8_t COLOR_RED = 3;
        static const uint8_t COLOR_BLUE = 4;
        static const uint8_t COLOR_YELLOW = 5;
        static const uint8_t COLOR_PURPLE = 6;
        static const uint8_t COLOR_CYAN = 7;
        static const uint8_t COLOR_GREY = 8;
        static const uint8_t COLOR_DARKGREY = 9;
        static const uint8_t COLOR_TRANSPARENT = 0x80;

        static const uint8_t READING_TYPE_PH = 'P';
        static const uint8_t READING_TYPE_EC = 'E';
        static const uint8_t READING_TYPE_DO = 'D';
        static const uint8_t READING_TYPE_TEMPERATURE = 'T';

    protected:

    private:
        void waitForReady();
        void waitForResponse(unsigned char* _data, unsigned char _size);
        void dataBuffer(unsigned char _count, const unsigned char* _data);
        void sendExtent(unsigned int _x, unsigned char _y);

        reading_data_t osb_reading;
        time_data_t osb_time;
        timet_data_t osb_timet;
        
        unsigned long time_t; // raw time

        unsigned char forecolor;
        unsigned char backcolor;

        // I2C related constants
        static const uint8_t I2C_ADDRESS = 0x10;
        static const uint8_t I2C_COMMAND_DRAWRECT = 0x1;
        static const uint8_t I2C_COMMAND_DRAWFILLEDRECT = 0x2;
        static const uint8_t I2C_COMMAND_DRAWTEXT = 0x3;
        static const uint8_t I2C_COMMAND_CLEARSCREEN = 0x4;
        static const uint8_t I2C_COMMAND_HOLDSCREEN = 0x5;
        static const uint8_t I2C_COMMAND_RELEASESCREEN = 0x6;
        static const uint8_t I2C_COMMAND_DRAWGRAPH_BAR = 32;
        static const uint8_t I2C_COMMAND_DRAWGRAPH_STEP = 33;
        static const uint8_t I2C_COMMAND_DATABUFFER = 64;
        static const uint8_t I2C_COMMAND_READ = 128;
        static const uint8_t I2C_COMMAND_TIME = 129;
        static const uint8_t I2C_COMMAND_TIMET = 130;

        static const uint8_t I2C_STATUS_IDLE = 0x0;
        static const uint8_t I2C_STATUS_BUSY = 0x1;
        static const uint8_t I2C_STATUS_RESPONSE = 0x2;
        static const uint8_t I2C_STATUS_BADCOMMAND = 0x254;

        static const uint8_t I2C_MAX_DATABUFFER = 28;

    };
}
#endif
