// An example that shows how to use the get functions to print
// probe measurements to the serial comm window and to the OSB screen

#include <Wire.h>
#include <Boekel_OpenSourceBio.h>

using namespace Boekel;

OpenSourceBio osb; // create osb object.

//The setup function will only run once
void setup()
{

 //Clearing Screen
 osb.clearScreen(OpenSourceBio::COLOR_BLACK);
 Serial.begin(9600);
}

void loop()
{

    double reading; // this is where we'll put the probe's read value

    char readingBuffer[12]; // To display the "reading" we need to convert it to a string/array of characters,
                            // we'll use "readingBuffer" as the array.
    
    osb.updateReadings();

    osb.holdScreen(); // we'll be upading on-screen text to don't refresh.

    osb.clearScreen(OpenSourceBio::COLOR_BLACK); // repaint the screen to black

    switch(osb.getReadingType()) // find which probe type the OSB is set to
    {
    case OpenSourceBio::READING_TYPE_PH: // the OSB probe type is set to pH
            Serial.print("pH: "); // print reading type to serial
            reading = osb.getPH(); // read sensor
            dtostrf(reading,2,3,readingBuffer); // convert double to string
            osb.displayText(10,70,OpenSourceBio::COLOR_WHITE,OpenSourceBio::COLOR_TRANSPARENT,"pH"); // display the reading type on the OSB screen
        break;
    case OpenSourceBio::READING_TYPE_EC:
            Serial.print("EC: ");
            reading = osb.getEC();
            dtostrf(reading, 6, 1, readingBuffer);
            osb.displayText(10, 70,OpenSourceBio::COLOR_WHITE,OpenSourceBio::COLOR_TRANSPARENT, "uS/cm");
        break;
    case OpenSourceBio::READING_TYPE_DO:
            Serial.print("DO mg/L: ");
            reading = osb.getDOmgl();
            dtostrf(reading, 3, 1, readingBuffer);
            osb.displayText(10, 70,OpenSourceBio::COLOR_WHITE,OpenSourceBio::COLOR_TRANSPARENT, "mg/L");
        break;
    case OpenSourceBio::READING_TYPE_TEMPERATURE:
            Serial.print("Temp: ");
            reading = osb.getTemperature();
            dtostrf(reading, 3, 1, readingBuffer);
            osb.displayText(10,70,OpenSourceBio::COLOR_WHITE,OpenSourceBio::COLOR_TRANSPARENT,"Temperature: ");
        break;
    default:
        Serial.println("Could not obtain a valid reading type. Did you forget to call updatereadings()?");
        break;
    }

 
    osb.displayText(160,70,OpenSourceBio::COLOR_WHITE,OpenSourceBio::COLOR_TRANSPARENT,readingBuffer); // display the reading value on the OSB screen
    Serial.println(readingBuffer); // print sensor value to serial monitor window

    osb.releaseScreen(); // release screen so the new text sent to it is drawn/displayed

}
