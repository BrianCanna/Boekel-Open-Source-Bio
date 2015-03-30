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

// Below are some functions we created to show you how you could make your own user interfaces

void draw_common()
{

  unsigned char data[256];
  unsigned char i;
  
  char buffer[12];

  // clear the screen
  osb.clearScreen(OpenSourceBio::COLOR_BLACK);
  
  // draw some text. White color, transparent background.
  osb.displayText(10,10,OpenSourceBio::COLOR_WHITE,OpenSourceBio::COLOR_TRANSPARENT,"Boekel Open Source Bio");
  osb.displayText(10,30,OpenSourceBio::COLOR_WHITE,OpenSourceBio::COLOR_TRANSPARENT,"Temperature: ");
  Serial.print("Temperature: ");

  if(osb.getTemperatureValid())
  {
    dtostrf(osb.getTemperature(), 3, 1, buffer); // getTemperature returns an integer, wee need to change it to a char aray to display it.
    osb.displayText(160,30,OpenSourceBio::COLOR_WHITE,OpenSourceBio::COLOR_TRANSPARENT,buffer);
    Serial.println(buffer);
  }
  else
  {
    osb.displayText(160,30,OpenSourceBio::COLOR_WHITE,OpenSourceBio::COLOR_TRANSPARENT,"----");
    Serial.println("(not valid)");
  }

  // draw a sample graph, using 255 data points, in blue, as a bar graph
  for(i = 0; i < 255; i++)
    data[i] = i;
  
  osb.drawGraphBars(1, 100, 318, 110, 255, &data[0],OpenSourceBio::COLOR_BLUE,OpenSourceBio::COLOR_TRANSPARENT);

  // draw another sample graph, this time using 16 data points, in red, and as a step graph
  for(i = 0; i < 16; i++)
    data[i] = 255 - (16 * i);
  
  osb.drawGraphStep(1, 100, 318, 110, 16, &data[0],OpenSourceBio::COLOR_RED,OpenSourceBio::COLOR_TRANSPARENT);
  
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
    osb.displayText(10,50,OpenSourceBio::COLOR_WHITE,OpenSourceBio::COLOR_TRANSPARENT,"pH Readings");
    Serial.println("pH");
    osb.displayText(10,70,OpenSourceBio::COLOR_WHITE,OpenSourceBio::COLOR_TRANSPARENT,"pH");
    osb.displayText(10,90,OpenSourceBio::COLOR_WHITE,OpenSourceBio::COLOR_TRANSPARENT,"Approx mV");
    if(osb.getReadingValid())
    {
        dtostrf(osb.getPH(), 2, 3, buffer);
        osb.displayText(160,70,OpenSourceBio::COLOR_WHITE,OpenSourceBio::COLOR_TRANSPARENT,buffer);
        Serial.print("pH: ");
        Serial.println(buffer);
        dtostrf(osb.getPHmV(), 3, 1, buffer);
        osb.displayText(160,90,OpenSourceBio::COLOR_WHITE,OpenSourceBio::COLOR_TRANSPARENT,buffer);
        Serial.print("Approx mV: ");
        Serial.println(buffer);
    }
    else
    {
        osb.displayText(160,70,OpenSourceBio::COLOR_WHITE,OpenSourceBio::COLOR_TRANSPARENT,"----");
        osb.displayText(160,90,OpenSourceBio::COLOR_WHITE,OpenSourceBio::COLOR_TRANSPARENT,"----");
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
    osb.displayText(10, 50,OpenSourceBio::COLOR_WHITE,OpenSourceBio::COLOR_TRANSPARENT, "Dissolved Oxygen Readings"); 

    Serial.println("Dissolved Oxygen");
    osb.displayText(10, 70,OpenSourceBio::COLOR_WHITE,OpenSourceBio::COLOR_TRANSPARENT, "mg/L");

    osb.displayText(10, 90,OpenSourceBio::COLOR_WHITE,OpenSourceBio::COLOR_TRANSPARENT, "% Saturation");

    if(osb.getReadingValid())
    {
        dtostrf(osb.getDOmgl(), 3, 1, buffer);
        osb.displayText(160, 70,OpenSourceBio::COLOR_WHITE,OpenSourceBio::COLOR_TRANSPARENT, buffer);

        Serial.print("mg/L: ");
        Serial.println(buffer);
        dtostrf(osb.getDOpercentage(), 3, 1, buffer);
        osb.displayText(160, 90,OpenSourceBio::COLOR_WHITE,OpenSourceBio::COLOR_TRANSPARENT, buffer);

        Serial.print("% Saturation: ");
        Serial.println(buffer);
    }
    else
    {
        osb.displayText(160, 70,OpenSourceBio::COLOR_WHITE,OpenSourceBio::COLOR_TRANSPARENT, "----");

        osb.displayText(160, 90,OpenSourceBio::COLOR_WHITE,OpenSourceBio::COLOR_TRANSPARENT, "----");

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
    osb.displayText(10, 50,OpenSourceBio::COLOR_WHITE,OpenSourceBio::COLOR_TRANSPARENT, "Electrolytic Conductivity");

    Serial.println("Electrolytic Conductivity");
    osb.displayText(10, 70,OpenSourceBio::COLOR_WHITE,OpenSourceBio::COLOR_TRANSPARENT, "uS/cm");
    osb.displayText(10, 90,OpenSourceBio::COLOR_WHITE,OpenSourceBio::COLOR_TRANSPARENT, "PSS");


    if(osb.getReadingValid())
    {
        dtostrf(osb.getEC(), 6, 1, buffer);
        osb.displayText(160, 70, OpenSourceBio::COLOR_WHITE,OpenSourceBio::COLOR_TRANSPARENT,buffer);

        Serial.print("uS/cm: ");
        Serial.println(buffer);
        dtostrf(osb.getECpss(), 2, 1, buffer);
        osb.displayText(160, 90,OpenSourceBio::COLOR_WHITE,OpenSourceBio::COLOR_TRANSPARENT, buffer);

        Serial.print("PSS: ");
        Serial.println(buffer);
        dtostrf(osb.getECtds(), 2, 1, buffer);
        Serial.print("TDS: "); 
        Serial.println(buffer);
    }
    else
    {
        osb.displayText(160, 70,OpenSourceBio::COLOR_WHITE,OpenSourceBio::COLOR_TRANSPARENT, "----");
        osb.displayText(160, 90,OpenSourceBio::COLOR_WHITE,OpenSourceBio::COLOR_TRANSPARENT, "----");
        Serial.println("Readings not valid");
    }
    osb.releaseScreen();
} 

