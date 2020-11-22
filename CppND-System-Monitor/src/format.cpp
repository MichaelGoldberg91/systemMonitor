#include <string>
#include <iostream>

#include "format.h"

using std::string;

string Format::ElapsedTime(long seconds) { 

    //string variables
    std::string hoursString = "";
    std::string minutesString = "";
    std::string secondsString = "";
    std::string returnString = "00:00:00";
    
    //time variables 
    long hours = (seconds / 3600);
    long minutes = ((seconds / 60) % 60);
    long secondsOnly  = (seconds % 60);

    //converting long to string
    hoursString = std::to_string(hours);
    minutesString = std::to_string(minutes);
    secondsString = std::to_string(secondsOnly);

    //if statements to check if below ten to add 0 in front
    if (hours < 10)
    {
        hoursString = "0" + hoursString;
    }
    
    if(minutes < 10)
    {
        minutesString = "0" + minutesString;
    }

    if (secondsOnly < 10)
    {
       secondsString = "0" + secondsString;
    }
    
    //packaging string
    returnString = hoursString + ":" + minutesString + ":" + secondsString;

    //return
    return returnString;
    
     }