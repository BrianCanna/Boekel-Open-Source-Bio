#include <Boekel_OpenSourceBio.h>
#include <Wire.h>

const uint8_t Boekel::OpenSourceBio::I2C_ADDRESS = 0x10;
const uint8_t Boekel::OpenSourceBio::I2C_COMMAND_DRAWRECT = 0x1;
const uint8_t Boekel::OpenSourceBio::I2C_COMMAND_DRAWFILLEDRECT = 0x2;
const uint8_t Boekel::OpenSourceBio::I2C_COMMAND_DRAWTEXT = 0x3;
const uint8_t Boekel::OpenSourceBio::I2C_COMMAND_CLEARSCREEN = 0x4;
const uint8_t Boekel::OpenSourceBio::I2C_COMMAND_HOLDSCREEN = 0x5;
const uint8_t Boekel::OpenSourceBio::I2C_COMMAND_RELEASESCREEN = 0x6;
const uint8_t Boekel::OpenSourceBio::I2C_COMMAND_DRAWGRAPH_BAR = 32;
const uint8_t Boekel::OpenSourceBio::I2C_COMMAND_DRAWGRAPH_STEP = 33;
const uint8_t Boekel::OpenSourceBio::I2C_COMMAND_DATABUFFER = 64;
const uint8_t Boekel::OpenSourceBio::I2C_COMMAND_READ = 128;
const uint8_t Boekel::OpenSourceBio::I2C_COMMAND_TIME = 129;
const uint8_t Boekel::OpenSourceBio::I2C_COMMAND_TIMET = 130;

const uint8_t Boekel::OpenSourceBio::I2C_STATUS_IDLE = 0x0;
const uint8_t Boekel::OpenSourceBio::I2C_STATUS_BUSY = 0x1;
const uint8_t Boekel::OpenSourceBio::I2C_STATUS_RESPONSE = 0x2;
const uint8_t Boekel::OpenSourceBio::I2C_STATUS_BADCOMMAND = 0x254;

const uint8_t Boekel::OpenSourceBio::COLOR_BLACK = 0;
const uint8_t Boekel::OpenSourceBio::COLOR_WHITE = 1;
const uint8_t Boekel::OpenSourceBio::COLOR_GREEN = 2;
const uint8_t Boekel::OpenSourceBio::COLOR_RED = 3;
const uint8_t Boekel::OpenSourceBio::COLOR_BLUE = 4;
const uint8_t Boekel::OpenSourceBio::COLOR_YELLOW = 5;
const uint8_t Boekel::OpenSourceBio::COLOR_PURPLE = 6;
const uint8_t Boekel::OpenSourceBio::COLOR_CYAN = 7;
const uint8_t Boekel::OpenSourceBio::COLOR_GREY = 8;
const uint8_t Boekel::OpenSourceBio::COLOR_DARKGREY = 9;
const uint8_t Boekel::OpenSourceBio::COLOR_TRANSPARENT = 0x80;

const uint8_t Boekel::OpenSourceBio::READING_TYPE_PH = 'P';
const uint8_t Boekel::OpenSourceBio::READING_TYPE_EC = 'E';
const uint8_t Boekel::OpenSourceBio::READING_TYPE_DO = 'D';
const uint8_t Boekel::OpenSourceBio::READING_TYPE_TEMPERATURE = 'T';

const uint8_t Boekel::OpenSourceBio::I2C_MAX_DATABUFFER = 28;

Boekel::OpenSourceBio::OpenSourceBio()
{
    reading_data_t osb_reading = {0, 0, 0, 0, 0, 0, 0, 0};
    time_data_t osb_time = {0, 0, 0, 0, 0, 0};
    timet_data_t osb_timet = {0};
}

/**
 * @brief
 * 
 * @author Miguel (3/28/2015)
 * 
 * @param _color 
 */
void Boekel::OpenSourceBio::setforecolor(unsigned char _color)
{
    forecolor = _color;
}

/**
 * @brief
 * 
 * @author Miguel (3/28/2015)
 * 
 * @param _color 
 */
void Boekel::OpenSourceBio::setbackcolor(unsigned char _color)
{
    backcolor = _color;
}

/**
 * @brief Waits until the I2C returns a ready status from the 
 *        board
 * 
 * @author Miguel (3/28/2015)
 */
void Boekel::OpenSourceBio::waitforready()
{
  unsigned char r;

  while(1)
  {
    Wire.requestFrom(I2C_ADDRESS, 1);
    while(Wire.available() == 0);
    r = Wire.read();

    // wait for an IDLE state
    if(r == I2C_STATUS_IDLE)
      break;
  }
}

/**
 * @brief Waits until the I2C returns a response status from the 
 *        board
 * 
 * @author Miguel (3/28/2015)
 * 
 * @param _data 
 * @param _size 
 */
void Boekel::OpenSourceBio::waitforresponse(unsigned char* _data, unsigned char _size)
{
  unsigned char r;

  while(1)
  {
    Wire.requestFrom(I2C_ADDRESS, _size + 1);
    while(Wire.available() == 0);
    r = Wire.read();

    // wait for a RESPONSE status code
    if(r == I2C_STATUS_RESPONSE)
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

/**
 * @brief sets the data buffer, using multiple transfers to 
 *        obtain up to 255 bytes transfered
 * 
 * @author Miguel (3/28/2015)
 * 
 * @param _count 
 * @param _data 
 */
void Boekel::OpenSourceBio::databuffer(unsigned char _count, const unsigned char* _data)
{

  unsigned char i;

  // wait for the device to be ready  
  waitforready();

  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(I2C_COMMAND_DATABUFFER);
  Wire.write(0);
  Wire.endTransmission();
  
  while(_count > 0)
  {
  
    // wait for the device to be ready  
    waitforready();

    Wire.beginTransmission(I2C_ADDRESS);
    Wire.write(I2C_COMMAND_DATABUFFER);
    
    // how many bytes?
    if(_count > I2C_MAX_DATABUFFER)
      Wire.write(I2C_MAX_DATABUFFER);
    else
      Wire.write(_count);
     
    i = 0;
    
    while((_count > 0) && (i < I2C_MAX_DATABUFFER))
    {
      Wire.write(*_data);
      _data++;
      i++;
      _count--;
    }

    Wire.endTransmission();
    
  }
 
}

/**
 * @brief writes an extent (i.e. coordiantes) to the I2C bus
 * 
 * @author Miguel (3/28/2015)
 * 
 * @param _x 
 * @param _y 
 */
void Boekel::OpenSourceBio::sendextent(unsigned int _x, unsigned char _y)
{
  Wire.write((unsigned char)_x & 0xFF);
  Wire.write((unsigned char)(_x >> 8)); 
  Wire.write(_y);
}

/**
 * @brief draws a rectangle on the screen
 * 
 * @author Miguel (3/28/2015)
 * 
 * @param _x 
 * @param _y 
 * @param _width 
 * @param _height 
 * @param _thickness 
 */
void Boekel::OpenSourceBio::drawrect(unsigned int _x, unsigned char _y, unsigned int _width, unsigned char _height, unsigned char _thickness)
{
  waitforready();

  Wire.beginTransmission(I2C_ADDRESS);

  Wire.write(I2C_COMMAND_DRAWRECT);
  sendextent(_x, _y);
  sendextent(_width, _height);
  Wire.write(_thickness);
  Wire.write(forecolor);

  Wire.endTransmission();
}

/**
 * @brief puts a "hold" on the screen, so it doesn't repaint
 *        useful when performing lots of updates
 * 
 * @author Miguel (3/28/2015)
 */
void Boekel::OpenSourceBio::holdscreen()
{
  waitforready();

  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(I2C_COMMAND_HOLDSCREEN);
  Wire.endTransmission();
}

/**
 * @brief release a "hold" on the screen, so it can repaint 
 *        useful when performing lots of updates
 * 
 * @author Miguel (3/28/2015)
 */
void Boekel::OpenSourceBio::releasescreen()
{
  waitforready();

  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(I2C_COMMAND_RELEASESCREEN);
  Wire.endTransmission();
}

/**
 * @brief clears the screen to the provided color
 * 
 * @author Miguel (3/28/2015)
 * 
 * @param _color 
 */
void Boekel::OpenSourceBio::clearscreen(unsigned char _color)
{
  waitforready();

  Wire.beginTransmission(I2C_ADDRESS);

  Wire.write(I2C_COMMAND_CLEARSCREEN);
  Wire.write(_color);
  Wire.endTransmission();
}

/**
 * @brief draws a filled rectangle on the screen
 * 
 * @author Miguel (3/28/2015)
 * 
 * @param _x 
 * @param _y 
 * @param _width 
 * @param _height 
 */
void Boekel::OpenSourceBio::drawfilledrect(unsigned int _x, unsigned char _y, unsigned int _width, unsigned char _height)
{
  waitforready();

  Wire.beginTransmission(I2C_ADDRESS);

  Wire.write(I2C_COMMAND_DRAWFILLEDRECT);
  sendextent(_x, _y);
  sendextent(_width, _height);
  Wire.write(forecolor);

  Wire.endTransmission();
}

/**
 * @brief draws text on the screen
 * 
 * @author Miguel (3/28/2015)
 * 
 * @param _x 
 * @param _y 
 * @param _text 
 */
void Boekel::OpenSourceBio::drawtext(unsigned int _x, unsigned char _y, const char* _text)
{
  // set the text into the databuffer
  databuffer(strlen(_text), (const unsigned char*)_text);
  
  waitforready();

  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(I2C_COMMAND_DRAWTEXT);
  sendextent(_x, _y);
  Wire.write(forecolor);
  Wire.write(backcolor);
  Wire.endTransmission();
}

/**
 * @brief draws a bar graph on the screen
 * 
 * @author Miguel (3/28/2015)
 * 
 * @param _x 
 * @param _y 
 * @param _width 
 * @param _height 
 * @param _pointcount 
 * @param _points 
 */
void Boekel::OpenSourceBio::drawgraph_bars(unsigned int _x, unsigned char _y, unsigned int _width, unsigned char _height, unsigned char _pointcount, unsigned char* _points)
{
  // set the points into the data buffer
  databuffer(_pointcount, _points);
  
  waitforready();

  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(I2C_COMMAND_DRAWGRAPH_BAR);
  sendextent(_x, _y);
  sendextent(_width, _height);
  Wire.write(forecolor);
  Wire.write(backcolor);
  Wire.endTransmission();
}

/**
 * @brief draws a step graph on the screen
 * 
 * @author Miguel (3/28/2015)
 * 
 * @param _x 
 * @param _y 
 * @param _width 
 * @param _height 
 * @param _pointcount 
 * @param _points 
 */
void Boekel::OpenSourceBio::drawgraph_step(unsigned int _x, unsigned char _y, unsigned int _width, unsigned char _height, unsigned char _pointcount, unsigned char* _points)
{
  // set the points into the data buffer
  databuffer(_pointcount, _points);
  
  waitforready();

  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(I2C_COMMAND_DRAWGRAPH_STEP);
  sendextent(_x, _y);
  sendextent(_width, _height);
  Wire.write(forecolor);
  Wire.write(backcolor);
  Wire.endTransmission();
}

/**
 * @brief gets the current readings from the OSB circuit
 * 
 * @author Miguel (3/28/2015)
 */
void Boekel::OpenSourceBio::updatereadings()
{
  waitforready();

  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(I2C_COMMAND_READ);
  Wire.endTransmission();

  waitforresponse((unsigned char*)&osb_reading, sizeof(reading_data_t));
}
/// @todo time_data_t* gettime()

/**
 * @brief gets a time_t compatible value from the OSB circuit
 * 
 * @author Miguel (3/28/2015)
 * 
 * @return unsigned long 
 */
unsigned long Boekel::OpenSourceBio::gettime_t()
{
  waitforready();

  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(I2C_COMMAND_TIMET);
  Wire.endTransmission();

  waitforresponse((unsigned char*)&osb_timet, sizeof(timet_data_t));

  return(osb_timet.time_t);
}

/**
 * @brief returns the type of reading taken 
 * @note A successful call to updatereadings() must occur before
 *       calls to this function
 * 
 * @author Miguel (3/28/2015)
 * 
 * @return char 
 */
char Boekel::OpenSourceBio::get_readingtype()
{
    return(osb_reading.type);
}

/**
 * @brief returns nonzero if the readings received from the OSB 
 *        are valid, zero if not
 * @note a successful call to updatereadings() must occur before
 *       calls to this function
 * 
 * @author Miguel (3/28/2015)
 * 
 * @return char 
 */
char Boekel::OpenSourceBio::get_readingvalid()
{
    return(osb_reading.readingsvalid);
}

/**
 * @brief returns nonzero if the temperature received from the 
 *        OSB are valid, zero if not
 * @note a successful call to updatereadings() must occur before
 *       calls to this function
 * 
 * @author Miguel (3/28/2015)
 * 
 * @return char 
 */
char Boekel::OpenSourceBio::get_temperaturevalid()
{
    return(osb_reading.temperaturevalid);
}

/**
 * @brief returns the temperature from the last reading
 *  
 * @note a successful call to updatereadings() must occur before
 *       calls to this function
 *  
 * @author Miguel (3/28/2015)
 * 
 * @return double 
 */
double Boekel::OpenSourceBio::get_temperature()
{
  // for all electrochemical readings, the temperature reading * 10.0 is in the temperature field
  // just divide by 10.0 to get the temperature again  
  return((double)osb_reading.temperature / 10.0);
}

/**
 * @brief returns the pH from the last reading 
 * if the reading is not of a pH type, returns 0.0
 *  
 * @note a successful call to updatereadings() must occur before
 *       calls to this function
 *  
 * @author Miguel (3/28/2015)
 * 
 * @return double 
 */
double Boekel::OpenSourceBio::get_ph()
{
  if(get_readingtype() != READING_TYPE_PH)
    return(0.0);

  // for pH readings, the pH reading * 1000.0 is in reading 1
  // just divide by 1000.0 to get the pH again  
  return((double)osb_reading.reading1 / 1000.0);
}

/**
 * @brief returns the mV from the pH from the last reading if 
 *        the reading is not of a pH type, returns 0.0
 *  
 * @note a successful call to updatereadings() must occur before
 *       calls to this function
 * 
 * @author Miguel (3/28/2015)
 * 
 * @return double 
 */
double Boekel::OpenSourceBio::get_ph_mV()
{
  if(get_readingtype() != READING_TYPE_PH)
    return(0.0);

  // for pH readings, the mV reading * 10.0 is in reading 2
  // just divide by 10.0 to get the mV again  
  return((double)osb_reading.reading2 / 10.0);
}

/**
 * @brief returns the EC (in uS/cm) from the last reading if the 
 *        reading is not of a EC type, returns 0.0
 *  
 * @note a successful call to updatereadings() must occur before
 *       calls to this function
 * 
 * @author Miguel (3/28/2015)
 * 
 * @return double 
 */
double Boekel::OpenSourceBio::get_ec()
{
  if(get_readingtype() != READING_TYPE_EC)
    return(0.0);

  // for EC readings, the uS/cm reading * 10.0 is in reading 1
  // just divide by 10.0 to get the uS/cm again  
  return((double)osb_reading.reading1 / 10.0);
}

/**
 * @brief returns the EC PSS from the last reading if the 
 *        reading is not of a EC type, returns 0.0
 *  
 * @note a successful call to updatereadings() must occur before
 *       calls to this function
 * 
 * @author Miguel (3/28/2015)
 * 
 * @return double 
 */
double Boekel::OpenSourceBio::get_ec_pss()
{
  if(get_readingtype() != READING_TYPE_EC)
    return(0.0);

  // for EC readings, the PSS reading * 10.0 is in reading 1
  // just divide by 10.0 to get the PSS again  
  return((double)osb_reading.reading2 / 10.0);
}

/**
 * @brief returns the EC TDS from the last reading if the 
 *        reading is not of a EC type, returns 0.0
 *  
 * @note a successful call to osb_updatereadings() must occur 
 *       before calls to this function
 * 
 * @author Miguel (3/28/2015)
 * 
 * @return double 
 */
double Boekel::OpenSourceBio::get_ec_tds()
{
  if(get_readingtype() != READING_TYPE_EC)
    return(0.0);

  // for EC readings, the TDS reading * 10.0 is in reading 1
  // just divide by 10.0 to get the TDS again  
  return((double)osb_reading.reading3 / 10.0);
}

/**
 * @brief returns the DO mg/L from the last reading if the 
 *        reading is not of a DO type, returns 0.0
 *  
 * @note a successful call to updatereadings() must occur before
 *       calls to this function
 * 
 * @author Miguel (3/28/2015)
 * 
 * @return double 
 */
double Boekel::OpenSourceBio::get_do_mgl()
{
  if(get_readingtype() != READING_TYPE_DO)
    return(0.0);

  // for DO readings, the mg/L reading * 100.0 is in reading 1
  // just divide by 100.0 to get the mg/L again  
  return((double)osb_reading.reading1 / 100.0);
}

/**
 * @brief returns the DO percentage from the last reading if the 
 *        reading is not of a DO type, returns 0.0
 *  
 * @note a successful call to osb_updatereadings() must occur 
 *       before calls to this function
 * 
 * @author Miguel (3/28/2015)
 * 
 * @return double 
 */
double Boekel::OpenSourceBio::get_do_percentage()
{
  if(get_readingtype() != READING_TYPE_DO)
    return(0.0);

  // for DO readings, the percentage reading * 10.0 is in reading 1
  // just divide by 10.0 to get the percentage again  
  return((double)osb_reading.reading2 / 10.0);
}
