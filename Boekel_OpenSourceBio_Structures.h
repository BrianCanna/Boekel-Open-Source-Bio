#ifndef _BOEKEL_OPENSOURCEBIO_STRUCTURES_H_
#define _BOEKEL_OPENSOURCEBIO_STRUCTURES_H_

namespace Boekel
{
    struct __attribute__((packed)) time_data_t 
    {
      unsigned char hours;
      unsigned char minutes;
      unsigned char seconds;
      unsigned char day;
      unsigned char month;
      unsigned int year;
    };


    // for receiving time_t
    struct __attribute__((packed)) timet_data_t 
    {
      unsigned long time_t;
    };

    // for receiving readings
    struct __attribute__((packed)) reading_data_t 
    {
      char type;
      char temperaturevalid;
      long temperature;
      char readingsvalid;
      long reading1;
      long reading2;
      long reading3;
      long reading4;
    };
}

#endif
