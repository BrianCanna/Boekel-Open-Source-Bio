// This code is used to get the current readings from the transmitter. 
// This also formats the readings and it will work regardless of the mode. 

#include <Wire.h>
#include <Boekel_OpenSourceBio.h>

using namespace Boekel;

OpenSourceBio osb;


void setup()
{
    Serial.begin(9600);
}

void loop()
{
    osb.updateReadings();
    switch(osb.getReadingType())
    {
    case OpenSourceBio::READING_TYPE_PH:
            draw_pH();
        break;
    case OpenSourceBio::READING_TYPE_EC:
            draw_ec();
        break;
    case OpenSourceBio::READING_TYPE_DO:
            draw_do();
        break;
    case OpenSourceBio::READING_TYPE_TEMPERATURE:
            draw_temperature();
        break;
    default:
        Serial.println("Could not obtain a valid reading type. Did you forget to call updatereadings()?");
        break;
    }
}

void draw_common()
{

  unsigned char data[256];
  unsigned char i;
  
  char buffer[12];

  // clear the screen
  osb.clearScreen(OpenSourceBio::COLOR_BLACK);

  osb.setForeColor(OpenSourceBio::COLOR_WHITE);
  osb.setBackColor(OpenSourceBio::COLOR_TRANSPARENT);
  
  // draw some text
  osb.drawText(10, 10, "Boekel Open Source Bio");
  osb.drawText(10, 30, "Temperature: ");
  Serial.print("Temperature: ");

  if(osb.getTemperatureValid())
  {
    dtostrf(osb.getTemperature(), 3, 1, buffer);
    osb.drawText(160, 30, buffer);
    Serial.println(buffer);
  }
  else
  {
    osb.drawText(160, 30, "----");
    Serial.println("(not valid)");
  }

  // draw a sample graph, using 255 data points, in blue, as a bar graph
  for(i = 0; i < 255; i++)
    data[i] = i;
  
  osb.setForeColor(OpenSourceBio::COLOR_BLUE);
  osb.drawGraphBars(1, 100, 318, 110, 255, &data[0]);

  // draw another sample graph, this time using 16 data points, in red, and as a step graph
  for(i = 0; i < 16; i++)
    data[i] = 255 - (16 * i);
  
  osb.setForeColor(OpenSourceBio::COLOR_RED);
  osb.drawGraphStep(1, 100, 318, 110, 16, &data[0]);

  // reset the foreground color
  osb.setForeColor(OpenSourceBio::COLOR_WHITE);
  
}

void draw_temperature()
{ 
 osb.holdScreen();
 draw_common();
 osb.releaseScreen();
} 


void draw_pH()
{
    char buffer[12];
    osb.holdScreen();
    draw_common();
    // draw some text
    osb.drawText(10, 50, "pH Readings");
    Serial.println("pH");
    osb.drawText(10, 70, "pH");
    osb.drawText(10, 90, "Approx mV");
    if(osb.getReadingValid())
    {
        dtostrf(osb.getPH(), 2, 3, buffer);
        osb.drawText(160, 70, buffer);
        Serial.print("pH: ");
        Serial.println(buffer);
        dtostrf(osb.getPHmV(), 3, 1, buffer);
        osb.drawText(160, 90, buffer);
        Serial.print("Approx mV: ");
        Serial.println(buffer);
    }
    else
    {
        osb.drawText(160, 70, "----");
        osb.drawText(160, 90, "----");
        Serial.println("Readings not valid");
    }
    osb.releaseScreen();
} 

void draw_do()
{
    char buffer[12];
    osb.holdScreen();
    draw_common();

    // draw some text
    osb.drawText(10, 50, "Dissolved Oxygen Readings"); 
    Serial.println("Dissolved Oxygen");
    osb.drawText(10, 70, "mg/L");
    osb.drawText(10, 90, "% Saturation");

    if(osb.getReadingValid())
    {
        dtostrf(osb.getDOmgl(), 3, 1, buffer);
        osb.drawText(160, 70, buffer);
        Serial.print("mg/L: ");
        Serial.println(buffer);
        dtostrf(osb.getDOpercentage(), 3, 1, buffer);
        osb.drawText(160, 90, buffer);
        Serial.print("% Saturation: ");
        Serial.println(buffer);
    }
    else
    {
        osb.drawText(160, 70, "----");
        osb.drawText(160, 90, "----");
        Serial.println("Readings not valid");
    }
    osb.releaseScreen();
} 


void draw_ec()
{
    char buffer[12];
    osb.holdScreen();
    draw_common();
    // draw some text
    osb.drawText(10, 50, "Electrolytic Conductivity");
    Serial.println("Electrolytic Conductivity");
    osb.drawText(10, 70, "uS/cm");
    osb.drawText(10, 90, "PSS");

    if(osb.getReadingValid())
    {
        dtostrf(osb.getEC(), 6, 1, buffer);
        osb.drawText(160, 70, buffer);
        Serial.print("uS/cm: ");
        Serial.println(buffer);
        dtostrf(osb.getECpss(), 2, 1, buffer);
        osb.drawText(160, 90, buffer);
        Serial.print("PSS: ");
        Serial.println(buffer);
        dtostrf(osb.getECtds(), 2, 1, buffer);
        Serial.print("TDS: "); 
        Serial.println(buffer);
    }
    else
    {
        osb.drawText(160, 70, "----");
        osb.drawText(160, 90, "----");
        Serial.println("Readings not valid");
    }
    osb.releaseScreen();
} 
