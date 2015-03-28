#include "Wire.h"
#include "string.h"

#define OSB_I2C_ADDRESS 0x10

#define OSB_I2C_COMMAND_DRAWRECT 0x1
#define OSB_I2C_COMMAND_DRAWFILLEDRECT 0x2
#define OSB_I2C_COMMAND_DRAWTEXT 0x3
#define OSB_I2C_COMMAND_CLEARSCREEN 0x4
#define OSB_I2C_COMMAND_HOLDSCREEN 0x5
#define OSB_I2C_COMMAND_RELEASESCREEN 0x6
#define OSB_I2C_COMMAND_DRAWGRAPH_BAR 32
#define OSB_I2C_COMMAND_DRAWGRAPH_STEP 33
#define OSB_I2C_COMMAND_DATABUFFER 64
#define OSB_I2C_COMMAND_READ 128
#define OSB_I2C_COMMAND_TIME 129
#define OSB_I2C_COMMAND_TIMET 130

#define OSB_I2C_STATUS_IDLE 0x0
#define OSB_I2C_STATUS_BUSY 0x1
#define OSB_I2C_STATUS_RESPONSE 0x2
#define OSB_I2C_STATUS_BADCOMMAND 0x254

#define OSB_COLOR_BLACK 0
#define OSB_COLOR_WHITE 1
#define OSB_COLOR_GREEN 2
#define OSB_COLOR_RED 3
#define OSB_COLOR_BLUE 4
#define OSB_COLOR_YELLOW 5
#define OSB_COLOR_PURPLE 6
#define OSB_COLOR_CYAN 7
#define OSB_COLOR_GREY 8
#define OSB_COLOR_DARKGREY 9
#define OSB_COLOR_TRANSPARENT 0x80

#define OSB_READING_TYPE_PH 'P'
#define OSB_READING_TYPE_EC 'E'
#define OSB_READING_TYPE_DO 'D'
#define OSB_READING_TYPE_TEMPERATURE 'T'

#define OSB_I2C_MAX_DATABUFFER 28

// for receiving time
struct __attribute__((packed)) osb_time_data_t 
{
  unsigned char hours;
  unsigned char minutes;
  unsigned char seconds;
  unsigned char day;
  unsigned char month;
  unsigned int year;
};


// for receiving time_t
struct __attribute__((packed)) osb_timet_data_t 
{
  unsigned long time_t;
};

// for receiving readings
struct __attribute__((packed)) osb_reading_data_t 
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


// instances of the structs for reading data from the OSB
osb_reading_data_t osb_reading = {0, 0, 0, 0, 0, 0, 0, 0};
osb_time_data_t osb_time = {0, 0, 0, 0, 0, 0};
osb_timet_data_t osb_timet = {0};

// our foreground and background color
unsigned char osb_forecolor = OSB_COLOR_WHITE;
unsigned char osb_backcolor = OSB_COLOR_TRANSPARENT;


void osb_setforecolor(unsigned char _color)
{
  osb_forecolor = _color;
}


void osb_setbackcolor(unsigned char _color)
{
  osb_backcolor = _color;
}

// Waits until the I2C returns a ready status from the board
void osb_waitforready()
{
  unsigned char r;

  while(1)
  {
    Wire.requestFrom(OSB_I2C_ADDRESS, 1);
    while(Wire.available() == 0);
    r = Wire.read();

    // wait for an IDLE state
    if(r == OSB_I2C_STATUS_IDLE)
      break;
  }
}


// Waits until the I2C returns a response status from the board
void osb_waitforresponse(unsigned char* _data, unsigned char _size)
{
  unsigned char r;

  while(1)
  {
    Wire.requestFrom(OSB_I2C_ADDRESS, _size + 1);
    while(Wire.available() == 0);
    r = Wire.read();

    // wait for a RESPONSE status code
    if(r == OSB_I2C_STATUS_RESPONSE)
    {
      unsigned char* ptr = _data;
      unsigned char count = _size;

      while(count > 0)
      {
        while(Wire.available() == 0);
        r = Wire.read();
        *ptr = r;
        ptr++;
        count--;
      }

      break;
    }
  }
}


// sets the data buffer, using multiple transfers to obtain up to 255 bytes transfered
void osb_databuffer(unsigned char _count, const unsigned char* _data)
{

  unsigned char i;

  // wait for the device to be ready  
  osb_waitforready();

  Wire.beginTransmission(OSB_I2C_ADDRESS);
  Wire.write(OSB_I2C_COMMAND_DATABUFFER);
  Wire.write(0);
  Wire.endTransmission();
  
  while(_count > 0)
  {
  
    // wait for the device to be ready  
    osb_waitforready();

    Wire.beginTransmission(OSB_I2C_ADDRESS);
    Wire.write(OSB_I2C_COMMAND_DATABUFFER);
    
    // how many bytes?
    if(_count > OSB_I2C_MAX_DATABUFFER)
      Wire.write(OSB_I2C_MAX_DATABUFFER);
    else
      Wire.write(_count);
     
    i = 0;
    
    while((_count > 0) && (i < OSB_I2C_MAX_DATABUFFER))
    {
      Wire.write(*_data);
      _data++;
      i++;
      _count--;
    }

    Wire.endTransmission();
    
  }
  
}


// writes an extent (i.e. coordiantes) to the I2C bus
void osb_sendextent(unsigned int _x, unsigned char _y)
{
  Wire.write((unsigned char)_x & 0xFF);
  Wire.write((unsigned char)(_x >> 8)); 
  Wire.write(_y);
}


// draws a rectangle on the screen
void osb_drawrect(unsigned int _x, unsigned char _y, unsigned int _width, unsigned char _height, unsigned char _thickness)
{

  osb_waitforready();

  Wire.beginTransmission(OSB_I2C_ADDRESS);

  Wire.write(OSB_I2C_COMMAND_DRAWRECT);
  osb_sendextent(_x, _y);
  osb_sendextent(_width, _height);
  Wire.write(_thickness);
  Wire.write(osb_forecolor);

  Wire.endTransmission();

}


// puts a "hold" on the screen, so it doesn't repaint
// useful when performing lots of updates
void osb_holdscreen()
{

  osb_waitforready();

  Wire.beginTransmission(OSB_I2C_ADDRESS);
  Wire.write(OSB_I2C_COMMAND_HOLDSCREEN);
  Wire.endTransmission();

}


// release a "hold" on the screen, so it can repaint
// useful when performing lots of updates
void osb_releasescreen()
{

  osb_waitforready();

  Wire.beginTransmission(OSB_I2C_ADDRESS);
  Wire.write(OSB_I2C_COMMAND_RELEASESCREEN);
  Wire.endTransmission();

}


// clears the screen to the provided color
void osb_clearscreen(unsigned char _color)
{

  osb_waitforready();

  Wire.beginTransmission(OSB_I2C_ADDRESS);

  Wire.write(OSB_I2C_COMMAND_CLEARSCREEN);
  Wire.write(_color);
  Wire.endTransmission();

}


// draws a filled rectangle on the screen
void osb_drawfilledrect(unsigned int _x, unsigned char _y, unsigned int _width, unsigned char _height)
{

  osb_waitforready();

  Wire.beginTransmission(OSB_I2C_ADDRESS);

  Wire.write(OSB_I2C_COMMAND_DRAWFILLEDRECT);
  osb_sendextent(_x, _y);
  osb_sendextent(_width, _height);
  Wire.write(osb_forecolor);

  Wire.endTransmission();

}


// draws text on the screen
void osb_drawtext(unsigned int _x, unsigned char _y, const char* _text)
{

  // set the text into the databuffer
  osb_databuffer(strlen(_text), (const unsigned char*)_text);
  
  osb_waitforready();

  Wire.beginTransmission(OSB_I2C_ADDRESS);
  Wire.write(OSB_I2C_COMMAND_DRAWTEXT);
  osb_sendextent(_x, _y);
  Wire.write(osb_forecolor);
  Wire.write(osb_backcolor);
  Wire.endTransmission();

}



// draws a bar graph on the screen
void osb_drawgraph_bars(unsigned int _x, unsigned char _y, unsigned int _width, unsigned char _height, unsigned char _pointcount, unsigned char* _points)
{

  // set the points into the data buffer
  osb_databuffer(_pointcount, _points);
  
  osb_waitforready();

  Wire.beginTransmission(OSB_I2C_ADDRESS);
  Wire.write(OSB_I2C_COMMAND_DRAWGRAPH_BAR);
  osb_sendextent(_x, _y);
  osb_sendextent(_width, _height);
  Wire.write(osb_forecolor);
  Wire.write(osb_backcolor);
  Wire.endTransmission();

}


// draws a step graph on the screen
void osb_drawgraph_step(unsigned int _x, unsigned char _y, unsigned int _width, unsigned char _height, unsigned char _pointcount, unsigned char* _points)
{

  // set the points into the data buffer
  osb_databuffer(_pointcount, _points);
  
  osb_waitforready();

  Wire.beginTransmission(OSB_I2C_ADDRESS);
  Wire.write(OSB_I2C_COMMAND_DRAWGRAPH_STEP);
  osb_sendextent(_x, _y);
  osb_sendextent(_width, _height);
  Wire.write(osb_forecolor);
  Wire.write(osb_backcolor);
  Wire.endTransmission();

}


// gets the current readings from the OSB circuit
void osb_updatereadings()
{

  osb_waitforready();

  Wire.beginTransmission(OSB_I2C_ADDRESS);
  Wire.write(OSB_I2C_COMMAND_READ);
  Wire.endTransmission();

  osb_waitforresponse((unsigned char*)&osb_reading, sizeof(osb_reading_data_t));

}


// gets the current time from the OSB circuit
struct osb_time_data_t* osb_gettime()
{

  osb_waitforready();

  Wire.beginTransmission(OSB_I2C_ADDRESS);
  Wire.write(OSB_I2C_COMMAND_TIME);
  Wire.endTransmission();

  osb_waitforresponse((unsigned char*)&osb_time, sizeof(osb_time_data_t));

  return(&(osb_time));

}


// gets a time_t compatible value from the OSB circuit
unsigned long osb_gettime_t()
{

  osb_waitforready();

  Wire.beginTransmission(OSB_I2C_ADDRESS);
  Wire.write(OSB_I2C_COMMAND_TIMET);
  Wire.endTransmission();

  osb_waitforresponse((unsigned char*)&osb_timet, sizeof(osb_timet_data_t));

  return(osb_timet.time_t);

}


// returns the type of reading taken
// NOTE: a successful call to osb_updatereadings() must occur before calls to this function
char osb_get_readingtype()
{
  return(osb_reading.type);
}


// returns nonzero if the readings received from the OSB are valid, zero if not
// NOTE: a successful call to osb_updatereadings() must occur before calls to this function
char osb_get_readingvalid()
{
  return(osb_reading.readingsvalid);
}


// returns nonzero if the temperature received from the OSB are valid, zero if not
// NOTE: a successful call to osb_updatereadings() must occur before calls to this function
char osb_get_temperaturevalid()
{
  return(osb_reading.temperaturevalid);
}


// returns the temperature from the last reading
// NOTE: a successful call to osb_updatereadings() must occur before calls to this function
double osb_get_temperature()
{
  // for all electrochemical readings, the temperature reading * 10.0 is in the temperature field
  // just divide by 10.0 to get the temperature again  
  return((double)osb_reading.temperature / 10.0);
}


// returns the pH from the last reading
// if the reading is not of a pH type, returns 0.0
// NOTE: a successful call to osb_updatereadings() must occur before calls to this function
double osb_get_ph()
{
  if(osb_get_readingtype() != OSB_READING_TYPE_PH)
    return(0.0);

  // for pH readings, the pH reading * 1000.0 is in reading 1
  // just divide by 1000.0 to get the pH again  
  return((double)osb_reading.reading1 / 1000.0);
}


// returns the mV from the pH from the last reading
// if the reading is not of a pH type, returns 0.0
// NOTE: a successful call to osb_updatereadings() must occur before calls to this function
double osb_get_ph_mV()
{
  if(osb_get_readingtype() != OSB_READING_TYPE_PH)
    return(0.0);

  // for pH readings, the mV reading * 10.0 is in reading 2
  // just divide by 10.0 to get the mV again  
  return((double)osb_reading.reading2 / 10.0);
}


// returns the EC (in uS/cm) from the last reading
// if the reading is not of a EC type, returns 0.0
// NOTE: a successful call to osb_updatereadings() must occur before calls to this function
double osb_get_ec()
{
  if(osb_get_readingtype() != OSB_READING_TYPE_EC)
    return(0.0);

  // for EC readings, the uS/cm reading * 10.0 is in reading 1
  // just divide by 10.0 to get the uS/cm again  
  return((double)osb_reading.reading1 / 10.0);
}


// returns the EC PSS from the last reading
// if the reading is not of a EC type, returns 0.0
// NOTE: a successful call to osb_updatereadings() must occur before calls to this function
double osb_get_ec_pss()
{
  if(osb_get_readingtype() != OSB_READING_TYPE_EC)
    return(0.0);

  // for EC readings, the PSS reading * 10.0 is in reading 1
  // just divide by 10.0 to get the PSS again  
  return((double)osb_reading.reading2 / 10.0);
}


// returns the EC TDS from the last reading
// if the reading is not of a EC type, returns 0.0
// NOTE: a successful call to osb_updatereadings() must occur before calls to this function
double osb_get_ec_tds()
{
  if(osb_get_readingtype() != OSB_READING_TYPE_EC)
    return(0.0);

  // for EC readings, the TDS reading * 10.0 is in reading 1
  // just divide by 10.0 to get the TDS again  
  return((double)osb_reading.reading3 / 10.0);
}


// returns the DO mg/L from the last reading
// if the reading is not of a DO type, returns 0.0
// NOTE: a successful call to osb_updatereadings() must occur before calls to this function
double osb_get_do_mgl()
{
  if(osb_get_readingtype() != OSB_READING_TYPE_DO)
    return(0.0);

  // for DO readings, the mg/L reading * 100.0 is in reading 1
  // just divide by 100.0 to get the mg/L again  
  return((double)osb_reading.reading1 / 100.0);
}


// returns the DO percentage from the last reading
// if the reading is not of a DO type, returns 0.0
// NOTE: a successful call to osb_updatereadings() must occur before calls to this function
double osb_get_do_percentage()
{
  if(osb_get_readingtype() != OSB_READING_TYPE_DO)
    return(0.0);

  // for DO readings, the percentage reading * 10.0 is in reading 1
  // just divide by 10.0 to get the percentage again  
  return((double)osb_reading.reading2 / 10.0);
}



#define GRAPH_POINTS 64
#define MAX_TEMPERATURE 40.0

unsigned char tempdata[GRAPH_POINTS];
unsigned char datacount = 0;

void setup()
{
  
  unsigned char i;
  
  Wire.begin();
  
  for(i = 0; i < GRAPH_POINTS; i++)
    tempdata[i] = 0;
  
  // hold all painting
  osb_holdscreen();
  
  // clear the screen
  osb_clearscreen(OSB_COLOR_BLACK);

  osb_setforecolor(OSB_COLOR_WHITE);
  osb_setbackcolor(OSB_COLOR_TRANSPARENT);
  osb_drawrect(5, 90, 310, 120, 3);
  
  // draw some text
  osb_drawtext(60, 10, "Boekel Open Source Bio");
  osb_drawtext(50, 60, "Current temperature:");

  // release the screen
  osb_releasescreen();
  
}


void loop()
{
  unsigned char i;
  char buffer[12];
  double temperature;
  
  // shift the data down a bit?
  if(datacount >= GRAPH_POINTS)
  {
    for(i = 1; i < GRAPH_POINTS; i++)
      tempdata[i - 1] = tempdata[i];
  }
  else
    datacount++;
  
  // hold all painting
  osb_holdscreen();
  
  // snap current readings
  osb_updatereadings();
  
  // save the data if it's valid, otherwise, give it some default values
  if(osb_get_temperaturevalid())
  {
    // get the temperature
    temperature = osb_get_temperature();
    
    // if it exceeded the maximum, make sure we paint it correctly
    if(temperature < MAX_TEMPERATURE)
      tempdata[datacount - 1] = (osb_get_temperature() * 255.0) / MAX_TEMPERATURE;
    else
      tempdata[datacount - 1] = 255;
      
    // save the temperature to a string, so we can display it
    dtostrf(osb_get_temperature(), 2, 1, buffer);
  }
  else
  {
    // no data - just use a default point
    tempdata[datacount - 1] = 128;
    buffer[0] = 0;
  }
  
  // black on black to clear the previous data from the screen
  osb_setbackcolor(OSB_COLOR_BLACK);
  osb_setforecolor(OSB_COLOR_BLACK);

  // clear the rectangles where the temperature reading and graphs were
  osb_drawfilledrect(230, 60, 80, 16);
  osb_drawfilledrect(10, 95, 300, 110);

  // use red to draw the graph
  osb_setforecolor(OSB_COLOR_RED);
  osb_drawgraph_step(10, 95, 300, 110, GRAPH_POINTS, &tempdata[0]);

  // use white to draw the current temperature
  osb_setforecolor(OSB_COLOR_WHITE);
  osb_drawtext(230, 60, buffer);

  // allow the screen to paint
  osb_releasescreen();
  
  delay(500);
  
}

