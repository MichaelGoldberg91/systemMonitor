#include "processor.h"
#include "linux_parser.h"
#include <string>


float Processor::Utilization() { 
    
    //creating variables (string for text file) from proc/stat
    std::string cpu, user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;

    //variable to read in data from text file
    std::string line;

    //creating a stream to text file
    std::ifstream f(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);

    //if file is open
    if(f.is_open())
    {
        //read in first line "for cpu"
        std::getline(f, line);

        //place line into stringstream
        std::stringstream SS(line);

        //store each data into correct variable;
        SS >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;

    }

        //Calculations using stackoverflow

        //finding idle time
        float idleTime = stof(idle) + stof(iowait);

        //finding nonidle time
        float nonIdleTime = stof(user) + stof(nice) + stof(system) + stof(irq) + stof (softirq) + stof(steal);

        //total time
        float totalTime = idleTime + nonIdleTime;

        //% used
        float percentUsage = (totalTime - idleTime) / (totalTime);

        //return
        return percentUsage;
 }