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
  osb.holdscreen();
  
  // clear the screen
  osb.clearscreen(OpenSourceBio::COLOR_BLACK);

  osb.setforecolor(OpenSourceBio::COLOR_WHITE);
  osb.setbackcolor(OpenSourceBio::COLOR_TRANSPARENT);
  osb.drawrect(5, 90, 310, 120, 3);
  
  // draw some text
  osb.drawtext(60, 10, "Boekel Open Source Bio");
  osb.drawtext(50, 60, "Current temperature:");

  // release the screen
  osb.releasescreen();
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
    osb.holdscreen();

    // snap current readings
    osb.updatereadings();

    // save the data if it's valid, otherwise, give it some default values
    if(osb.get_temperaturevalid())
    {
        // get the temperature
        temperature = osb.get_temperature();

        // if it exceeded the maximum, make sure we paint it correctly
        if(temperature < MAX_TEMPERATURE)
        {   
            tempdata[datacount - 1] = (osb.get_temperature() * 255.0) / MAX_TEMPERATURE;
        }
        else
        {   
            tempdata[datacount - 1] = 255;
        }

        // save the temperature to a string, so we can display it
        dtostrf(osb.get_temperature(), 2, 1, buffer);
    }
    else
    {
        // no data - just use a default point
        tempdata[datacount - 1] = 128;
        buffer[0] = 0;
    }

    // black on black to clear the previous data from the screen
    osb.setbackcolor(OpenSourceBio::COLOR_BLACK);
    osb.setforecolor(OpenSourceBio::COLOR_BLACK);

    // clear the rectangles where the temperature reading and graphs were
    osb.drawfilledrect(230, 60, 80, 16);
    osb.drawfilledrect(10, 95, 300, 110);

    // use red to draw the graph
    osb.setforecolor(OpenSourceBio::COLOR_RED);
    osb.drawgraph_step(10, 95, 300, 110, GRAPH_POINTS, &tempdata[0]);

    // use white to draw the current temperature
    osb.setforecolor(OpenSourceBio::COLOR_WHITE);
    osb.drawtext(230, 60, buffer);

    // allow the screen to paint
    osb.releasescreen();

    delay(500);
}
