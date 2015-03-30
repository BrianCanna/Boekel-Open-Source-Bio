#include <Wire.h>
#include <Boekel_OpenSourceBio.h>

using namespace Boekel;

OpenSourceBio osb;
//The setup function will only run once
void setup()
{
    // clear the screen to black
    osb.clearScreen(OpenSourceBio::COLOR_BLACK);

    // draw a 100wx100h rectangle/square located at top left corner (0,0) with green border of thickness 5 and red fill.
    osb.displayFilledRectangle(0,0,100,100,OpenSourceBio::COLOR_GREEN,OpenSourceBio::COLOR_RED,5); 

    // draw a similar rectangle to the one above but blue border, purple fill, and at a different location with different dimensions
    osb.displayFilledRectangle(75,75,50,100,OpenSourceBio::COLOR_BLUE,OpenSourceBio::COLOR_PURPLE,2);

    // draw a cyan rectangle without a border (transparent border)
    osb.displayFilledRectangle(150,100,100,50,OpenSourceBio::COLOR_TRANSPARENT,OpenSourceBio::COLOR_CYAN,0);
}
//Main program to print text
void loop()
{

}
