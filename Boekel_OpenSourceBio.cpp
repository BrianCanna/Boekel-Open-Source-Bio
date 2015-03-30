#include <Boekel_OpenSourceBio.h>
#include <Wire.h>

Boekel::OpenSourceBio::OpenSourceBio()
{
    Wire.begin();
    reading_data_t osb_reading = {0, 0, 0, 0, 0, 0, 0, 0};
    time_data_t osb_time = {0, 0, 0, 0, 0, 0};
    timet_data_t osb_timet = {0};
}

/**
 * @brief Sets the text's character color. You may choose any 
 *        color from the library as defined in the header (.h)
 *        file e.g. COLOR_BLACK, COLOR_RED, COLOR_BLUE
 * 
 * @author Miguel (3/28/2015)
 * 
 * @param _color The color you wish to use for the 
 *               foreground/text
 */
void Boekel::OpenSourceBio::setForeColor(unsigned char _color)
{
    forecolor = _color;
}

/**
 * @brief Set's the text's background color. You may choose any 
 *        color from the library as defined in the header (.h)
 *        file e.g. COLOR_BLACK, COLOR_RED, COLOR_BLUE
 * 
 * @author Miguel (3/28/2015)
 * 
 * @param _color 
 */
void Boekel::OpenSourceBio::setBackColor(unsigned char _color)
{
    backcolor = _color;
}

/**
 * @brief Waits until the I2C returns a ready status from the 
 *        board.
 * 
 * @author Miguel (3/28/2015)
 */
void Boekel::OpenSourceBio::waitForReady()
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
void Boekel::OpenSourceBio::waitForResponse(unsigned char* _data, unsigned char _size)
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
 * @brief Sets the data buffer, using multiple transfers to 
 *        obtain up to 255 bytes transfered
 * 
 * @author Miguel (3/28/2015)
 * 
 * @param _count 
 * @param _data 
 */
void Boekel::OpenSourceBio::dataBuffer(unsigned char _count, const unsigned char* _data)
{

  unsigned char i;

  // wait for the device to be ready  
  waitForReady();

  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(I2C_COMMAND_DATABUFFER);
  Wire.write(0);
  Wire.endTransmission();
  
  while(_count > 0)
  {
  
    // wait for the device to be ready  
    waitForReady();

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
void Boekel::OpenSourceBio::sendExtent(unsigned int _x, unsigned char _y)
{
  Wire.write((unsigned char)_x & 0xFF);
  Wire.write((unsigned char)(_x >> 8)); 
  Wire.write(_y);
}

/**
 * @brief Draws a rectangle on the screen
 * 
 * @author Miguel (3/28/2015)
 * 
 * @param _x 
 * @param _y 
 * @param _width 
 * @param _height 
 * @param _thickness 
 */
void Boekel::OpenSourceBio::drawRect(unsigned int _x, unsigned char _y, unsigned int _width, unsigned char _height, unsigned char _thickness)
{
  waitForReady();

  Wire.beginTransmission(I2C_ADDRESS);

  Wire.write(I2C_COMMAND_DRAWRECT);
  sendExtent(_x, _y);
  sendExtent(_width, _height);
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
void Boekel::OpenSourceBio::holdScreen()
{
  waitForReady();

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
void Boekel::OpenSourceBio::releaseScreen()
{
  waitForReady();

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
void Boekel::OpenSourceBio::clearScreen(unsigned char _color)
{
  waitForReady();

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
void Boekel::OpenSourceBio::drawFilledRect(unsigned int _x, unsigned char _y, unsigned int _width, unsigned char _height)
{

  waitForReady();

  Wire.beginTransmission(I2C_ADDRESS);

  Wire.write(I2C_COMMAND_DRAWFILLEDRECT);
  sendExtent(_x, _y);
  sendExtent(_width, _height);
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
void Boekel::OpenSourceBio::drawText(unsigned int _x, unsigned char _y, const char* _text)
{
  // set the text into the databuffer
  dataBuffer(strlen(_text), (const unsigned char*)_text);
  
  waitForReady();

  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(I2C_COMMAND_DRAWTEXT);
  sendExtent(_x, _y);
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
void Boekel::OpenSourceBio::drawGraphBars(unsigned int _x, unsigned char _y, unsigned int _width, unsigned char _height, unsigned char _pointcount, unsigned char* _points)
{
  // set the points into the data buffer
  dataBuffer(_pointcount, _points);
  
  waitForReady();

  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(I2C_COMMAND_DRAWGRAPH_BAR);
  sendExtent(_x, _y);
  sendExtent(_width, _height);
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
void Boekel::OpenSourceBio::drawGraphStep(unsigned int _x, unsigned char _y, unsigned int _width, unsigned char _height, unsigned char _pointcount, unsigned char* _points)
{
  // set the points into the data buffer
  dataBuffer(_pointcount, _points);
  
  waitForReady();

  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(I2C_COMMAND_DRAWGRAPH_STEP);
  sendExtent(_x, _y);
  sendExtent(_width, _height);
  Wire.write(forecolor);
  Wire.write(backcolor);
  Wire.endTransmission();
}

/**
 * @brief gets the current readings from the OSB circuit
 * 
 * @author Miguel (3/28/2015)
 */
void Boekel::OpenSourceBio::updateReadings()
{
  waitForReady();

  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(I2C_COMMAND_READ);
  Wire.endTransmission();

  waitForResponse((unsigned char*)&osb_reading, sizeof(reading_data_t));
}

/**
 * @brief gets the current time from the OSB circuit
 * 
 * @author Miguel (3/29/2015)
 * 
 * @return struct time_data_t* 
 */
struct Boekel::time_data_t* Boekel::OpenSourceBio::getTime()
{

  waitForReady();

  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(I2C_COMMAND_TIME);
  Wire.endTransmission();

  waitForResponse((unsigned char*)&osb_time, sizeof(time_data_t));

  return(&(osb_time));

}

/**
 * @brief gets a time_t compatible value from the OSB circuit
 * 
 * @author Miguel (3/28/2015)
 * 
 * @return unsigned long 
 */
unsigned long Boekel::OpenSourceBio::getTimeRaw()
{
  waitForReady();

  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(I2C_COMMAND_TIMET);
  Wire.endTransmission();

  waitForResponse((unsigned char*)&osb_timet, sizeof(timet_data_t));

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
char Boekel::OpenSourceBio::getReadingType()
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
char Boekel::OpenSourceBio::getReadingValid()
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
char Boekel::OpenSourceBio::getTemperatureValid()
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
double Boekel::OpenSourceBio::getTemperature()
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
double Boekel::OpenSourceBio::getPH()
{
  if(getReadingType() != READING_TYPE_PH)
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
double Boekel::OpenSourceBio::getPHmV()
{
  if(getReadingType() != READING_TYPE_PH)
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
double Boekel::OpenSourceBio::getEC()
{
  if(getReadingType() != READING_TYPE_EC)
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
double Boekel::OpenSourceBio::getECpss()
{
  if(getReadingType() != READING_TYPE_EC)
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
double Boekel::OpenSourceBio::getECtds()
{
  if(getReadingType() != READING_TYPE_EC)
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
double Boekel::OpenSourceBio::getDOmgl()
{
  if(getReadingType() != READING_TYPE_DO)
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
double Boekel::OpenSourceBio::getDOpercentage()
{
  if(getReadingType() != READING_TYPE_DO)
    return(0.0);

  // for DO readings, the percentage reading * 10.0 is in reading 1
  // just divide by 10.0 to get the percentage again  
  return((double)osb_reading.reading2 / 10.0);
}

unsigned char Boekel::OpenSourceBio::getHours()
{
     
     return getTime()->hours;
}

unsigned char Boekel::OpenSourceBio::getMinutes()
{
     
    return getTime()->minutes;
}

unsigned char Boekel::OpenSourceBio::getSeconds()
{ 
    return getTime()->seconds;
}

const char* Boekel::OpenSourceBio::getWeekDay()
{

    uint8_t d = getTime()->day;
    uint8_t m = getTime()->month;
    unsigned int y = getTime()->year;

    uint8_t weekDay;

    weekDay = (d+=m<3?y--:y-2,23*m/9+d+4+y/4-y/100+y/400)%7;

    switch(weekDay)
    {
    case 0:
        return "Sun";
        break;
    case 1:
        return "Mon";
        break;
    case 2:
        return "Tues";
        break;
    case 3:
        return "Wed";
        break;
    case 4:
        return "Thurs";
        break;
    case 5:
        return "Fri";
        break;
    case 6:
        return "Sat";
        break;
    default:
        return "---";
        break;
    }
}
unsigned char Boekel::OpenSourceBio::getDay()
{
    return getTime()->day;
}

unsigned char Boekel::OpenSourceBio::getMonth()
{
     // @todo
    return getTime()->month;
}

unsigned int Boekel::OpenSourceBio::getYear()
{
    return getTime()->year;
}

void Boekel::OpenSourceBio::displayText(unsigned int _x, unsigned int _y, unsigned char _forecolor, unsigned char _backcolor, const char* _text)
{
    setForeColor(_forecolor);
    setBackColor(_backcolor);
    drawText(_x,_y,_text);
}
