#include <Wire.h>
#include <Boekel_OpenSourceBio.h>

#define GRAPH_POINTS 64
#define MAX_TEMPERATURE 40.0

unsigned char tempdata[GRAPH_POINTS];
unsigned char datacount = 0;

using namespace Boekel;

OpenSourceBio osb;
//The setup function will only run once
void setup()
{
  unsigned char i;
  
  for(i = 0; i < GRAPH_POINTS; i++)
  {
    tempdata[i] = 0;
  }
  
  // hold all painting
  osb.holdScreen();
  
  // clear the screen
  osb.clearScreen(OpenSourceBio::COLOR_BLACK);

  osb.setForeColor(OpenSourceBio::COLOR_WHITE);
  osb.setBackColor(OpenSourceBio::COLOR_TRANSPARENT);
  osb.drawRect(5, 90, 310, 120, 3);
  
  // draw some text
  osb.drawText(60, 10, "Boekel Open Source Bio");
  osb.drawText(50, 60, "Current temperature:");

  // release the screen
  osb.releaseScreen();
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
        {   
            tempdata[i - 1] = tempdata[i];
        }
    }
    else
    {   
        datacount++;
    }

    // hold all painting
    osb.holdScreen();

    // snap current readings
    osb.updateReadings();

    // save the data if it's valid, otherwise, give it some default values
    if(osb.getTemperatureValid())
    {
        // get the temperature
        temperature = osb.getTemperature();

        // if it exceeded the maximum, make sure we paint it correctly
        if(temperature < MAX_TEMPERATURE)
        {   
            tempdata[datacount - 1] = (osb.getTemperature() * 255.0) / MAX_TEMPERATURE;
        }
        else
        {   
            tempdata[datacount - 1] = 255;
        }

        // save the temperature to a string, so we can display it
        dtostrf(osb.getTemperature(), 2, 1, buffer);
    }
    else
    {
        // no data - just use a default point
        tempdata[datacount - 1] = 128;
        buffer[0] = 0;
    }

    // black on black to clear the previous data from the screen
    osb.setBackColor(OpenSourceBio::COLOR_BLACK);
    osb.setForeColor(OpenSourceBio::COLOR_BLACK);

    // clear the rectangles where the temperature reading and graphs were
    osb.drawFilledRect(230, 60, 80, 16);
    osb.drawFilledRect(10, 95, 300, 110);

    // use red to draw the graph
    osb.setForeColor(OpenSourceBio::COLOR_RED);
    osb.drawGraphStep(10, 95, 300, 110, GRAPH_POINTS, &tempdata[0]);

    // use white to draw the current temperature
    osb.setForeColor(OpenSourceBio::COLOR_WHITE);
    osb.drawText(230, 60, buffer);

    // allow the screen to paint
    osb.releaseScreen();

    delay(500);
}
