#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include "linux_parser.h"

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

//constructor
Process::Process(int p):processID_(p){};

int Process::Pid() { return processID_; }

float Process::CpuUtilization() { 

    //creating cpu variable
    cpu = LinuxParser::calculateProcessCpu(Pid(), UpTime());
    return LinuxParser::calculateProcessCpu(Pid(), UpTime()); }


string Process::Command() { return LinuxParser::Command(Pid()); }

string Process::Ram() {  return LinuxParser::Ram(Pid()); }

string Process::User() { return LinuxParser::User(Pid()); }

long int Process::UpTime() { return LinuxParser::UpTime(Pid()); }

bool Process::operator<(Process const& a) const { 

  //if a is less than next process  
  return  a.cpu < cpu;
 }