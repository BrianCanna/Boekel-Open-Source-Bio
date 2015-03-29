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
        void setforecolor(unsigned char _color);
        void setbackcolor(unsigned char _color);
        void waitforready();
        void waitforresponse(unsigned char* _data, unsigned char _size);
        void databuffer(unsigned char _count, const unsigned char* _data);
        void sendextent(unsigned int _x, unsigned char _y);
        void drawrect(unsigned int _x, unsigned char _y, unsigned int _width, unsigned char _height, unsigned char _thickness);
        void holdscreen();
        void releasescreen();
        void clearscreen(unsigned char _color);
        void drawfilledrect(unsigned int _x, unsigned char _y, unsigned int _width, unsigned char _height);
        void drawtext(unsigned int _x, unsigned char _y, const char* _text);
        void drawgraph_bars(unsigned int _x, unsigned char _y, unsigned int _width, unsigned char _height, unsigned char _pointcount, unsigned char* _points);
        void drawgraph_step(unsigned int _x, unsigned char _y, unsigned int _width, unsigned char _height, unsigned char _pointcount, unsigned char* _points);
        void updatereadings();
        struct time_data_t* gettime();
        unsigned long gettime_t();
        char get_readingtype();
        char get_readingvalid();
        char get_temperaturevalid();
        double get_temperature();
        double get_ph();
        double get_ph_mV();
        double get_ec();
        double get_ec_pss();
        double get_ec_tds();
        double get_do_mgl();
        double get_do_percentage();

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
