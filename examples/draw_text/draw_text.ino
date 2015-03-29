#include <Wire.h>
#include <Boekel_OpenSourceBio.h>

using namespace Boekel;

OpenSourceBio osb;
//The setup function will only run once
void setup()
{

}
//Main program to print text
void loop()
{
//Setting Colors
 osb.setforecolor(OpenSourceBio::COLOR_RED);
 osb.setbackcolor(OpenSourceBio::COLOR_TRANSPARENT);


 //Clearing Screen
 osb.clearscreen(OpenSourceBio::COLOR_BLACK);
 //draw text on screen x coordinate, y coordinate, "text");
 osb.drawtext(10, 10, "Boekel Open Source Bio");
 osb.drawtext(10, 30, "Screen Test ");
 osb.drawtext(10, 50, "Screen Test ");
 osb.drawtext(10, 70, "Screen Test ");
 osb.drawtext(10, 90, "Screen Test ");
 osb.drawtext(10, 110, "Screen Test ");
 osb.drawtext(10, 130, "Screen Test ");
 //pausing for three seconds
 delay(3000);
} 
