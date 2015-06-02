/** 
 * Hardware/Circuit connections: If your voltage-to-current 
 * converter requires a V+ and V- for the voltage terminals use 
 * pins "11" and "G" from the Open Source Bio respectively. 
 *  
 * If you wish to also power the converter from the OSB's 
 * Arduino board, for Vcc and GND of the converter use the OSB's
 * "V" and "G" pins respectively. For current limits of the 
 * Arduino board reference
 * http://playground.arduino.cc/Main/ArduinoPinCurrentLimitations 
 *  
 * @details This example shows how to produce a PWM pulse on the
 *          Open Source Bio Arduino port pin 11 whose DC voltage
 *          reading will be between 0V and 5V. This can
 *          be used to interface the Open Source Bio with
 *          voltage-to-current converters, devices whose input
 *          is a voltage and whose output is a respective
 *          current.
 *  
 *          For this example we will use the temperature probe
 *          and assume a minium and maximum temperature
 *          of 0 and 100 celsius respectively.
 *  
 *          NOTE/IMPORTANT: The PWM pulse is an 8-bit PWM pulse.
 *          This means that there can only be 256
 *          different voltage values for the sensor range
 *          selected by the "SENSOR_MAX" and "SENSOR_MIN"
 *          defines. So, while you can use the each of the
 *          sensor's (DO, EC, Temp., pH) full range it is
 *          recommended that use an expected range.
 *  
 *          @example If for a particular experiment you expect a
 *                   temperature between 20 and 50 degrees
 *                   celsius use these values for MIN and MAX
 *                   respectively rather than 0 and 100 celsius (the
 *                   entire sensor range). Using expected values
 *                   can give you a better voltage value
 *                   resolution.
 *  
 *  
 *                  
 */


#include <Wire.h>
#include <Boekel_OpenSourceBio.h>

using namespace Boekel;

OpenSourceBio osb; // create osb object.


/* 
* Sensor Ranges. These are the ranges of values that you cna use for each probe: 
*  
* pH: 0.001 to 14.000
* DO: 0.01 to 35.99 mg/L
* EC: 0.07 us/cm to 500,000 us/cm
* Temperature: 0 to 100 C; 
*  
*/

// Set temperature sensor's expected reading range.

// 100 celsius
#define SENSOR_MAX 100 
// 0 celsius
#define SENSOR_MIN 0 

// produce the PWM on the OSB's Arduino port pin 11
#define PWM_PIN 11


//The setup function will only run once
void setup()
{

 Serial.begin(9600); // Let's use the serial for debugging purposes.

 //Clearing Screen
 osb.clearScreen(OpenSourceBio::COLOR_BLACK);
}

void loop()
{

    double reading; // this is where we'll put the probe's read value

    osb.updateReadings();

    reading = osb.getTemperature(); // get the new temperature reading

    if( (reading>SENSOR_MAX) || (reading<SENSOR_MIN)){
        Serial.print("Sensor reading out of expted range: ");
        Serial.println(reading);
    }else{
        int pwmValue = osb.getPWM(reading,SENSOR_MIN,SENSOR_MAX);
        // Serial.println(pwmValue); // optional, can be used for debugging purposes. It will output the calculated PWM value.

        analogWrite(PWM_PIN,pwmValue); // The analogWrite function produces a PWM with the duty ratio defined by a PWM value between 0 and 255.
                               // A value of 255 will produce a 100% duty+ cycle, a value of 0 will produce a 0% duty+ cycle.
                               // When measured with a multimeter a 100% duty+ cycle PWM will read 5V, a 75% duty+ cycle PWM will read 3.75V
    }


}
