#include <Wire.h>
#include <Boekel_OpenSourceBio.h>

using namespace Boekel;

OpenSourceBio osb;
//The setup function will only run once
void setup()
{
  Serial.begin(38400);
  osb.clearscreen(OpenSourceBio::COLOR_BLACK);
}
//Main program to print text
void loop()
{
    time_data_t* ot;
    unsigned long tt;

    Serial.print("Paused...");
    while(Serial.available() == 0);
    Serial.read();
    delay(100);

    Serial.println("Requesting time_t from OSB"); 
    tt = osb.gettime_t();
    Serial.print("time_t=");
    Serial.println(tt);

    Serial.println("Requesting decoded time from OSB"); 
    ot = osb.gettime();

    Serial.print("Current time: ");
    Serial.print(ot->hours);
    Serial.print(":");
    Serial.print(ot->minutes);
    Serial.print(":");
    Serial.println(ot->seconds);

    Serial.print("Current date: ");
    Serial.print(ot->month);
    Serial.print("/");
    Serial.print(ot->day);
    Serial.print("/");
    Serial.println(ot->year);

    Serial.println("Requesting a reading");

    osb.updatereadings();

    Serial.print("Got a reading type of ");
    Serial.print(osb.get_readingtype());
    Serial.println();

    switch(osb.get_readingtype())
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

    }

    Serial.println("done");
}


void draw_common()
{

  unsigned char data[256];
  unsigned char i;
  
  char buffer[12];

  // clear the screen
  osb.clearscreen(OpenSourceBio::COLOR_BLACK);

  osb.setforecolor(OpenSourceBio::COLOR_WHITE);
  osb.setbackcolor(OpenSourceBio::COLOR_TRANSPARENT);
  
  // draw some text
  osb.drawtext(10, 10, "Boekel Open Source Bio");
  osb.drawtext(10, 30, "Temperature: ");
  Serial.print("Temperature: ");

  if(osb.get_temperaturevalid())
  {
    dtostrf(osb.get_temperature(), 3, 1, buffer);
    osb.drawtext(160, 30, buffer);
    Serial.println(buffer);
  }
  else
  {
    osb.drawtext(160, 30, "----");
    Serial.println("(not valid)");
  }

  // draw a sample graph, using 255 data points, in blue, as a bar graph
  for(i = 0; i < 255; i++)
    data[i] = i;
  
  osb.setforecolor(OpenSourceBio::COLOR_BLUE);
  osb.drawgraph_bars(1, 100, 318, 110, 255, &data[0]);

  // draw another sample graph, this time using 16 data points, in red, and as a step graph
  for(i = 0; i < 16; i++)
    data[i] = 255 - (16 * i);
  
  osb.setforecolor(OpenSourceBio::COLOR_RED);
  osb.drawgraph_step(1, 100, 318, 110, 16, &data[0]);

  // reset the foreground color
  osb.setforecolor(OpenSourceBio::COLOR_WHITE);
  
}

void draw_temperature()
{ 
 osb.holdscreen();
 draw_common();
 osb.releasescreen();
} 


void draw_pH()
{
    char buffer[12];
    osb.holdscreen();
    draw_common();
    // draw some text
    osb.drawtext(10, 50, "pH Readings");
    Serial.println("pH");
    osb.drawtext(10, 70, "pH");
    osb.drawtext(10, 90, "Approx mV");
    if(osb.get_readingvalid())
    {
        dtostrf(osb.get_ph(), 2, 3, buffer);
        osb.drawtext(160, 70, buffer);
        Serial.print("pH: ");
        Serial.println(buffer);
        dtostrf(osb.get_ph_mV(), 3, 1, buffer);
        osb.drawtext(160, 90, buffer);
        Serial.print("Approx mV: ");
        Serial.println(buffer);
    }
    else
    {
        osb.drawtext(160, 70, "----");
        osb.drawtext(160, 90, "----");
        Serial.println("Readings not valid");
    }
    osb.releasescreen();
} 

void draw_do()
{
    char buffer[12];
    osb.holdscreen();
    draw_common();

    // draw some text
    osb.drawtext(10, 50, "Dissolved Oxygen Readings"); 
    Serial.println("Dissolved Oxygen");
    osb.drawtext(10, 70, "mg/L");
    osb.drawtext(10, 90, "% Saturation");

    if(osb.get_readingvalid())
    {
        dtostrf(osb.get_do_mgl(), 3, 1, buffer);
        osb.drawtext(160, 70, buffer);
        Serial.print("mg/L: ");
        Serial.println(buffer);
        dtostrf(osb.get_do_percentage(), 3, 1, buffer);
        osb.drawtext(160, 90, buffer);
        Serial.print("% Saturation: ");
        Serial.println(buffer);
    }
    else
    {
        osb.drawtext(160, 70, "----");
        osb.drawtext(160, 90, "----");
        Serial.println("Readings not valid");
    }
    osb.releasescreen();
} 


void draw_ec()
{
    char buffer[12];
    osb.holdscreen();
    draw_common();
    // draw some text
    osb.drawtext(10, 50, "Electrolytic Conductivity");
    Serial.println("Electrolytic Conductivity");
    osb.drawtext(10, 70, "uS/cm");
    osb.drawtext(10, 90, "PSS");

    if(osb.get_readingvalid())
    {
        dtostrf(osb.get_ec(), 6, 1, buffer);
        osb.drawtext(160, 70, buffer);
        Serial.print("uS/cm: ");
        Serial.println(buffer);
        dtostrf(osb.get_ec_pss(), 2, 1, buffer);
        osb.drawtext(160, 90, buffer);
        Serial.print("PSS: ");
        Serial.println(buffer);
        dtostrf(osb.get_ec_tds(), 2, 1, buffer);
        Serial.print("TDS: "); 
        Serial.println(buffer);
    }
    else
    {
        osb.drawtext(160, 70, "----");
        osb.drawtext(160, 90, "----");
        Serial.println("Readings not valid");
    }
    osb.releasescreen();
}
