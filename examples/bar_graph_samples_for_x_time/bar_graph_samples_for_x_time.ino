// This programs shows how to run a sampling session for x minutes for the selected probe in the "Setup" menu with a bar graph
// When complete the text banner will in the screen will change to "Sampling session complete!"

#include <Wire.h>
#include <Boekel_OpenSourceBio.h>

using namespace Boekel;

#define SAMPLING_TIME_MINUTES 3 // the number of minutes for which the sampling session will run
#define MAX_EXPECTED_VALUE 14 // max expected value (integer) of the units you are trying to measure. In this case we chose 14 for PH measurements.

OpenSourceBio osb;
//The setup function will only run once
void setup()
{

}
//Main program, runs in loop.
void loop()
{
    // Note: The function barGraph has a third (optional) paramter, sample size. If not specified the 
    // default value is 64. The max value can be 250 
    // e.g. osb.barGraph(SAMPLING_TIME_MINUTES,MAX_EXPECTED_VALUE,200) will take 200 samples over the given sampling time.
    if(osb.barGraph(SAMPLING_TIME_MINUTES,MAX_EXPECTED_VALUE)) // check if the sampling session is running
    {
        // the sampling session is complete.
        osb.displayText(60, 10,OpenSourceBio::COLOR_BLACK,OpenSourceBio::COLOR_WHITE, "Sampling session complete!");
        while(1);
    }
}
