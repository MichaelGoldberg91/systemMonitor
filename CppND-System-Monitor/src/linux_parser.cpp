#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;


string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}


string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}


vector<int> LinuxParser::Pids() {

  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);

        //std::cout << "pid: " << pid << "\n";
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization() { 

  //create line variable
  std::string line, memTotal, memFree;

  //return value
  float memUsed;

  //creating generic variables
  string variableName, size, sizeType;

  //create ifstream where memory is stored
  std::ifstream ifStream(kProcDirectory+kMeminfoFilename);

  //check to see if file is there
  if(ifStream.is_open())
  {
    //get first line
    getline(ifStream,line);

    //creating stream for the line
    std::stringstream SS(line);

    //pushing stream into generic variables
    SS >> variableName >> size >> sizeType;

    //checking if variableName is "MemTotal:"
    if(variableName == "MemTotal:")
    {
        memTotal = size;
    }
    else
    {
      //std::cout << "Not MemTotal" << "\n";
    }

    //Quick way to empty variables
    variableName = "";
    size = "";
    sizeType = "";
    SS.clear();

    //get next line
    getline(ifStream,line);

    //creating stream for the line
    SS.str(line);

    //pushing stream into generic variables
    SS >> variableName >> size >> sizeType;

    //checking if variableName is "MemFree:"
    if(variableName == "MemFree:")
    {
        memFree = size;
    }
    else
    {
      //std::cout << "Not MemFree" << "\n";
    }

    //Quick way to empty variables
    variableName = "";
    size = "";
    sizeType = "";
    SS.clear(); 
  
    //finding mem used(%)
    memUsed =(((stof(memTotal)) - (stof(memFree))) / (stof(memTotal)));

    //return memUsed as %
    return memUsed;

  }



return 0;
 }

long LinuxParser::UpTime() { 
  
  //variables
  std::string line, upTime, idleProcessTime;

  //create ifstream where upTime information is stored
  std::ifstream ifStream(kProcDirectory+kUptimeFilename);
  
  //check to see if file is there
  if(ifStream.is_open())
  {
    //get first line
    getline(ifStream,line);

    //line to stringstream
    std::stringstream SS(line);

    //store SS variables
    SS >> upTime >>idleProcessTime;
  
  }
  
  //changing string to long and returning upTime value
  return stol(upTime);
  
   }


long LinuxParser::Jiffies() {
  
   //return Jiffies calculation
   return LinuxParser::UpTime() * sysconf(_SC_CLK_TCK); }


long LinuxParser::ActiveJiffies(int pid) { 
  
  //variables
  std::string line;

  //return value
  long activeJiff = 0;

  //create ifstream where upTime information is stored
  std::ifstream ifStream(kProcDirectory+ to_string(pid) + kStatFilename);
  
  //check to see if file is there
  if(ifStream.is_open())
  {

  //get full line
  getline(ifStream, line);

  //move line to SS
  std::stringstream SS(line);

  //create temp string variable
  std::string temp;

  //create vector to push SS into
  std::vector<std::string> tempVect;

  //while SS has value
  while(SS >> temp)
  {
    //push to the back of vector
    tempVect.push_back(temp);
  }
  //add times
  activeJiff = stol(tempVect[13]) + stol(tempVect[14]) + stol(tempVect[15]) + stol(tempVect[16]);

  }
  
  //return active 
  return activeJiff; }


long LinuxParser::ActiveJiffies() {
  
  //variables
  std::string line , type , num1 ,num2 ,num3 ,num4 ,num5 ,num6 ,num7 ,num8, num9, num10;

  //total active jiffies
  long totalJiff;

  //create ifstream where upTime information is stored
  std::ifstream ifStream(kProcDirectory + kStatFilename);
  
  //check to see if file is there
  if(ifStream.is_open())
  {
    //get first line
    getline(ifStream,line);

    //line to stringstream
    std::stringstream SS(line);

    //putting SS into variables
    SS >> type >> num1 >> num2 >> num3 >> num4 >> num5 >> num6 >> num7 >> num8 >> num9 >> num10;

  }
  
  //adding the ten numbers together
  totalJiff = stol(num1) + stol(num2) + stol(num3) + stol(num4) + stol(num5) +
              stol(num6) + stol(num7) + stol(num8) + stol(num9) + stol(num10);

                // std::cout << "total jiff: " << totalJiff << "\n";
 
  //returning sum
  return totalJiff; 
}


long LinuxParser::IdleJiffies() { 
  
//variables
  std::string line , type , num1 ,num2 ,num3 ,num4 ,num5 ,num6 ,num7 ,num8, num9, num10;

  //total idle jiffies
  long idleJiffies;

  //create ifstream where upTime information is stored
  std::ifstream ifStream(kProcDirectory + kStatFilename);
  
  //check to see if file is there
  if(ifStream.is_open())
  {
    //get first line
    getline(ifStream,line);

    //line to stringstream
    std::stringstream SS(line);

    //putting SS into variables
    SS >> type >> num1 >> num2 >> num3 >> num4 >> num5 >> num6 >> num7 >> num8 >> num9 >> num10;

  }
  
  //adding the ten numbers together
  idleJiffies = stol(num4) + stol(num5); 

                // std::cout << "idle jiff sum: " << idleJiffies << "\n";
 
  //returning sum of idles
  return idleJiffies;  }


vector<string> LinuxParser::CpuUtilization() { return {}; }//Not needed, i did work in processor


int LinuxParser::TotalProcesses() {
  
  //create line variable
  std::string line, processessTotal;

  //create ifstream where memory is stored
  std::ifstream ifStream(kProcDirectory+kStatFilename);

  //check to see if file is there
  if(ifStream.is_open())
  {
    //get first line
   while(std::getline(ifStream,line))
    {
        //creating stream for the line
        std::stringstream SS(line);

        //temp variables for if
        std::string temp, temp1;

        //create stream with line
        SS >> temp >> temp1;

        //if first part of the string is processess
        if(temp == "processes")
        {
          //save second part 
          processessTotal = temp1;
        }
    }
  }

   //change string to int and return
   return std::stoi(processessTotal); }


int LinuxParser::RunningProcesses() { 

  //create line variable
  std::string line, runningProcesses;

  //create ifstream where memory is stored
  std::ifstream ifStream(kProcDirectory+kStatFilename);

  //check to see if file is there
  if(ifStream.is_open())
  {
    //get first line
   while(std::getline(ifStream,line))
    {
        //creating stream for the line
        std::stringstream SS(line);

        //temp variables for if
        std::string temp, temp1;

        //create stream with line
        SS >> temp >> temp1;

        //if first part of the string is procs_running
        if(temp == "procs_running")
        {
          //save second part 
          runningProcesses = temp1;
        }
    }
  }
   //change string to int and return
   return std::stoi(runningProcesses);
 }

string LinuxParser::Command(int pid) { 

  //string variable
  std::string line;
  
  //create ifstream where memory is stored
  std::ifstream ifStream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);

  //grab line from file
  getline(ifStream, line);
  
  //return line
  return line;
 }

string LinuxParser::Ram(int pid) {
   
  //create ifstream where memory is stored
  std::ifstream ifStream(kProcDirectory + std::to_string(pid) + kStatusFilename);

  //generic variables
  std::string line, key, value, returnRam, returnRamTemp;

  //using your search for ram
  if (ifStream.is_open()) {
    while (std::getline(ifStream, line)) {
     // std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {

        //if key  == certain keyname
        if (key == "VmSize:") {

            //return value in mb
            returnRamTemp = value;

            //converting string to int and dividing by 1000 to convert kb to mb
            int temp = stoi(value) / 1000;

            //return float back to string for return
            returnRam = to_string(temp);  
        }
      }
    }
  }  
   //return ram
   return returnRam; }


string LinuxParser::Uid(int pid) {
  
    //create ifstream where memory is stored
  std::ifstream ifStream(kProcDirectory + std::to_string(pid) + kStatusFilename);

  //generic variables
  std::string line, key, value, uid;

  //using your search for uid
  if (ifStream.is_open()) {
    while (std::getline(ifStream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {

        //if key  == keyname
        if (key == "Uid:") {

            //return value
            uid = value;    
        }
      }
    }
  }  
   //return
   return uid; }


string LinuxParser::User(int pid) {
  
  //create ifstream where user is stored
  std::ifstream ifStream(kPasswordPath);

  //generic variables
  std::string line, key, value, user, temp;

  //if file open
  if (ifStream.is_open()) {

    //while there is another line grab it
    while (std::getline(ifStream, line)) 
    {
      //replace
      std::replace(line.begin(), line.end(), ':', ' ');

      //put line into string stream
      std::stringstream SS(line);

      //while there are values in line
      while (SS >> value >> temp >> key )
      {
        //grab pid and get uid and comapre with key, if matches, return 
        if (Uid(pid) == key)
        {
            return value;
        }
      }
    }
  }
  
   return ""; }

long LinuxParser::UpTime(int pid) { 

  //return value
  long returnValue;

  //generic variables
  std::string line, temp;

  //create stream
  std::ifstream ifStream (kProcDirectory + std::to_string(pid) + kStatFilename);

  //if stream is open
  if(ifStream.is_open())
  {
    //looping through file line by line
    while(getline(ifStream, line))
    {
      //create stream with line
      std::stringstream SS(line);

      //get 22nd value
      for(int x = 0; x < 22;x ++)
      {
        //store variable in temp (string)
        SS >> temp;
      }
    }
  }
 //change string to long and divides it by sysconf(_SC_CLK_TCK)
  returnValue = UpTime() - (stol(temp) / sysconf(_SC_CLK_TCK));

  //return
  return returnValue; }

//personal function to calculate CPU for process
float LinuxParser::calculateProcessCpu(int pid, long t)
{
  //return value
  float cpu;

  //find total jiffie time for specific pid
  float pidJiffieTime = LinuxParser::ActiveJiffies(pid);

  //call function to find uptime
  float upTime = LinuxParser::UpTime();

  //find seconds active
  float secAct = upTime - (t / sysconf(_SC_CLK_TCK));

  //cpu calculations
  cpu = (pidJiffieTime / sysconf(_SC_CLK_TCK)) / secAct;

  //return
  return cpu;
}