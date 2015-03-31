// This is a template file you can use to make your own Arduino programs.
#include <Wire.h>
#include <Boekel_OpenSourceBio.h>

using namespace Boekel;

OpenSourceBio osb; // create osb object.

//The setup function will only run once
void setup()
{

 //Clearing Screen
 osb.clearScreen(OpenSourceBio::COLOR_BLACK);
}

void loop()
{


}
