#include <Wire.h>
#include <Boekel_OpenSourceBio.h>

using namespace Boekel;

OpenSourceBio osb;

// The osb object returns the month, day, and year as integers.
// To use them in the displayText function we need to change them to chars.
char mm[3]; // we add an extra index to account for the null character.
char dd[3];
char yy[5];
char hh[3];
char min[3];
char ss[3];

char date[20]; // we will put the string dd/mm/yyyy hh:mm:ss in the date[] array


//The setup function will only run once
void setup()
{

    osb.clearScreen(OpenSourceBio::COLOR_BLACK); // repaint the entire screen with black
    
}

void loop()
{

    osb.holdScreen();
    itoa(osb.getMonth(),mm,10); // put the result of getMonth() into the char array month;
    itoa(osb.getDay(),dd,10);
    itoa(osb.getYear(),yy,10);
    itoa(osb.getHours(),hh,10);
    itoa(osb.getMinutes(),min,10);
    itoa(osb.getSeconds(),ss,10);

    // build the date string "dd/mm/yyyy hh:mm:ss by concatenating/attaching month, day, and year
    strcat(date,mm);
    strcat(date,"/");
    strcat(date,dd);
    strcat(date,"/");
    strcat(date,yy);
    strcat(date," ");
    strcat(date,hh);
    strcat(date,":");
    strcat(date,min);
    strcat(date,":");
    strcat(date,ss);

    // display the date on the screen with a red text color and grey background at location x=100,y=0
    osb.displayText(100,0,OpenSourceBio::COLOR_RED,OpenSourceBio::COLOR_GREY,date);
    osb.releaseScreen();
    

}
