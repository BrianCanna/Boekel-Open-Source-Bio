// The device should say “Hello World” on the user programmable screen. 
// This test indicates that both the Arduino and the Boekel board are working correctly. 

#include <Wire.h>
#include <Boekel_OpenSourceBio.h>

using namespace Boekel;

OpenSourceBio osb;

// The program setup loop runs once and we start the I2C library
void setup()
{
 
}
// The program main loop runs continuously and it delays the program for 3000
// milliseconds then gives the print command
void loop()
{
 delay(3000);
 //Setting Colors
 osb.setforecolor(OpenSourceBio::COLOR_RED);
 osb.setbackcolor(OpenSourceBio::COLOR_TRANSPARENT);


 //Clearing Screen
 osb.clearscreen(OpenSourceBio::COLOR_BLACK);
 // Print command osb_drawtext(x coordinate, y coordinate, “Text”);
 osb.drawtext(100, 100, "HELLO WORLD");
} 
