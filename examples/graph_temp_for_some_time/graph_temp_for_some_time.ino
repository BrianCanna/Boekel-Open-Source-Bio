// This programs shows how to run a sampling session for x minutes. 
// When complete the text banner will change to "Sampling session complete!"

#include <Wire.h>
#include <Boekel_OpenSourceBio.h>

using namespace Boekel;

#define SAMPLING_TIME_MINUTES 3 // the number of minutes for which the session will run
#define MAX_TEMPERATURE 40 // max expected value

OpenSourceBio osb;
//The setup function will only run once
void setup()
{

}
//Main program to print text
void loop()
{
    if(osb.stepGraph(SAMPLING_TIME_MINUTES,OpenSourceBio::READING_TYPE_TEMPERATURE,MAX_TEMPERATURE)) // check if the sampling session is running
    {
        // the sampling session is complete.
        osb.displayText(60, 10,OpenSourceBio::COLOR_BLACK,OpenSourceBio::COLOR_WHITE, "Sampling session complete!");
        while(1);
    }
}
