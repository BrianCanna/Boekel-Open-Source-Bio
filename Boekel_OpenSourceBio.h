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

        static const uint8_t COLOR_BLACK;
        static const uint8_t COLOR_WHITE;
        static const uint8_t COLOR_GREEN;
        static const uint8_t COLOR_RED;
        static const uint8_t COLOR_BLUE;
        static const uint8_t COLOR_YELLOW;
        static const uint8_t COLOR_PURPLE;
        static const uint8_t COLOR_CYAN;
        static const uint8_t COLOR_GREY;
        static const uint8_t COLOR_DARKGREY;
        static const uint8_t COLOR_TRANSPARENT;

    protected:

    private:
        
        reading_data_t osb_reading;
        time_data_t osb_time;
        timet_data_t osb_timet;
        

        unsigned long time_t; // raw time

        // 
        unsigned char forecolor;
        unsigned char backcolor;

        // I2C related constants
        static const uint8_t I2C_ADDRESS;
        static const uint8_t I2C_COMMAND_DRAWRECT;
        static const uint8_t I2C_COMMAND_DRAWFILLEDRECT;
        static const uint8_t I2C_COMMAND_DRAWTEXT;
        static const uint8_t I2C_COMMAND_CLEARSCREEN;
        static const uint8_t I2C_COMMAND_HOLDSCREEN;
        static const uint8_t I2C_COMMAND_RELEASESCREEN;
        static const uint8_t I2C_COMMAND_DRAWGRAPH_BAR;
        static const uint8_t I2C_COMMAND_DRAWGRAPH_STEP;
        static const uint8_t I2C_COMMAND_DATABUFFER;
        static const uint8_t I2C_COMMAND_READ;
        static const uint8_t I2C_COMMAND_TIME;
        static const uint8_t I2C_COMMAND_TIMET;

        static const uint8_t I2C_STATUS_IDLE;
        static const uint8_t I2C_STATUS_BUSY;
        static const uint8_t I2C_STATUS_RESPONSE;
        static const uint8_t I2C_STATUS_BADCOMMAND;

        static const uint8_t READING_TYPE_PH;
        static const uint8_t READING_TYPE_EC;
        static const uint8_t READING_TYPE_DO;
        static const uint8_t READING_TYPE_TEMPERATURE;

        static const uint8_t I2C_MAX_DATABUFFER;

    };
}
#endif
