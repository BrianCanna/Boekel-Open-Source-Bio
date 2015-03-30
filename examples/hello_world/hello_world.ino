// The device should say “Hello World” on the user programmable screen. 
// This test indicates that both the Arduino and the Boekel board are working correctly. 

#include <Wire.h>
#include <Boekel_OpenSourceBio.h>

using namespace Boekel;

OpenSourceBio osb;

// The program setup loop runs once and we start the I2C library
void setup()
{
  //Clearing Screen
 osb.clearScreen(OpenSourceBio::COLOR_BLACK);

 // Print command osb_drawtext(x coordinate, y coordinate, “Text”);
 osb.displayText(100, 100, OpenSourceBio::COLOR_RED, OpenSourceBio::COLOR_TRANSPARENT, "HELLO WORLD");
}

void loop()
{

} 
