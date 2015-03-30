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


 //Clearing Screen
 osb.clearScreen(OpenSourceBio::COLOR_BLACK);
 //draw text on screen x coordinate, y coordinate, "text");
 osb.displayText(10, 10,OpenSourceBio::COLOR_BLUE, OpenSourceBio::COLOR_RED,"Boekel Open Source Bio");
 osb.displayText(10, 30,OpenSourceBio::COLOR_RED, OpenSourceBio::COLOR_BLUE,"Week Day:");
 osb.displayText(10, 50,OpenSourceBio::COLOR_GREEN, OpenSourceBio::COLOR_WHITE,osb.getWeekDay());
 osb.displayText(10, 70,OpenSourceBio::COLOR_CYAN, OpenSourceBio::COLOR_PURPLE,"Screen Test Line 1 ");
 osb.displayText(10, 90,OpenSourceBio::COLOR_YELLOW, OpenSourceBio::COLOR_DARKGREY,"Screen Test Line 2");
 osb.displayText(10, 110,OpenSourceBio::COLOR_WHITE, OpenSourceBio::COLOR_BLACK,"Screen Test Line 3");
 osb.displayText(10, 130,OpenSourceBio::COLOR_RED, OpenSourceBio::COLOR_GREEN,"Screen Test Line 4");
 //pausing for three seconds
 delay(3000);
} 
