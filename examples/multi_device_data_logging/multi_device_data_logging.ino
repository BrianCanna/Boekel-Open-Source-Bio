/**
 * @date May 30, 2015 
 *  
 * @brief Example that sends temperature and dissolved oxygen 
 *        readings through pins 10 and 11 of the OSB/Arduino.
 *        Note1: Make sure that the probe type is set to
 *        dissolved oxygen.
 *  
 * @details This examples shows how to use pins 10 and 11 for 
 *          those users who need to log data using different
 *          devices e.g. CompactRIO, or any other device that
 *          has serial communication capabilities. Please note
 *          that although the software is the same, the
 *          interface/circuit will vary depending on the type of
 *          interface your device has e.g. RS232, TTL, USB.
 *          Visit the Open Source Bio product page for more
 *          details.
*/

#include <SoftwareSerial.h> // the SoftwareSerial library is needed 
                            // when you wish to create a serial interface (TX and RX pins)
                            // on two digital pins of the Arduino/OSB i.e. pins 10 and 11
#include <Wire.h>
#include <Boekel_OpenSourceBio.h>

// we will use pin 10 for receiving data
#define PIN_RX 10
// we will use pin 11 for sending data
#define PIN_TX 11

SoftwareSerial mySerial(PIN_RX,PIN_TX); // create the serial object. Note: if you want to log data to a PC delete this line (you can log data to a PC using the regular Serial object which sends data through the USB type B connector)

using namespace Boekel;

OpenSourceBio osb; // create osb object.

//The setup function will only run once
void setup()
{

 // Note: if you want to log data in a PC, use "Serial" instad of my "mySerial". This is because with a PC you can log data using the OSB's USB type B connector/interface which is the Arduino's default serial port hardware.

 mySerial.begin(9600);// Set the baud rate to 9600 (Sets the speed (baud rate) for the serial communication. Supported baud rates are 300, 600, 1200, 2400, 4800, 9600, 14400, 19200, 28800, 31250, 38400, 57600, and 115200.)
                      // check that this baud rate value (9600 in this case) matches your device's e.g. CompactRIO
                      // The serial settings of the device you are attaching to the OSB need to be
                      // 8 data bits, no parity, one stop bit

 //Clearing Screen
 osb.clearScreen(OpenSourceBio::COLOR_BLACK);
}

void loop()
{

    double reading; // this is where we'll put the probe's read value

    char readingBufferDO[12]; // To display the "reading" we need to convert it to a string/array of characters,
                            // we'll use "readingBuffer" as the array.
    char readingBufferTemp[12]; // To display the "reading" we need to convert it to a string/array of characters,
                            // we'll use "readingBuffer" as the array.

    osb.updateReadings();

    // The data from the OSB via serial will have the following Comma Separated Value (CSV) format:
    // "S:xxxx,yyyy;\r\n" 
    // where "S:" will signal the start of a new reading, 
    // "xxxx" will be the dissolved oxygen reading, "yyyy" will be the temperature reading,
    // and ";\r\n" will signal the end of a reading.
    // 
    // This is data format that you can parse with software written in your device (e.g. CompactRIO).
    // 
    // You can of course change this format if you like.


    mySerial.print("S:"); // Send new reading signal

    // Dissolved oxygen
    reading = osb.getDOmgl(); // get a new DO mg/L reading
    dtostrf(reading, 3, 1, readingBufferDO); // format the reading
    mySerial.print(readingBufferDO); // send dissolved oxygen reading

    // send a comma to separate the DO and Temp. readings.
    mySerial.print(",");

    // Temperature
    reading = osb.getTemperature(); // get the new temperature reading
    dtostrf(reading, 3, 1, readingBufferTemp); // format the reading
    mySerial.print(readingBufferTemp); // send the temperature reading


    mySerial.print(";\r\n"); // send characters that signal the end of a reading.

    // Note: if you would like to get readings from other probes such as EC see the "display_probe_measurement" example. 
    // In the case of EC you use these two lines for instance:
    // reading = osb.getEC();
    // dtostrf(reading, 6, 1, readingBufferEC);

}
