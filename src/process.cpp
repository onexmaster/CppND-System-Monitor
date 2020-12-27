#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <unistd.h>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// Done: Return this process's ID
int Process::Pid() { 
    return pid_;
 }

// Done: Return this process's CPU utilization
float Process::CpuUtilization() { 
    //get systens uptime
   long uptime=LinuxParser::UpTime();
   //get the time for this process
   long total_time=LinuxParser::ActiveJiffies(pid_);
   //get the processor's freq
   long hertz=sysconf(_SC_CLK_TCK);
    //get the start_time of this system
    //no other way to ;(, need to parse the file again
    string line, token;
  vector<string> values;
  std::ifstream filestream( LinuxParser::kProcDirectory + std::to_string(pid_) + LinuxParser::kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream stream(line);
    while (stream >> token) {
      values.push_back(token);
    }
  }
  long start_time=stol(values[21]);
  float seconds = float(uptime) - (float(start_time) / float(hertz));
  float cpu_usage = (float(total_time) / float(hertz)) / float(seconds);

  return cpu_usage;

 }

// Done: Return the command that generated this process
string Process::Command() { 
    return LinuxParser::Command(Pid());
 }

// Done: Return this process's memory utilization
string Process::Ram() { 
    return LinuxParser::Ram(Pid());
 }

// Done: Return the user (name) that generated this process
string Process::User() { 
    return LinuxParser::User(Pid());
 }

// Done: Return the age of this process (in seconds)
long int Process::UpTime() { 
    //get the start_time of the pid 
    long int start_time=LinuxParser::UpTime(Pid());
    long int uptime= LinuxParser::UpTime();
    long int hertz=sysconf(_SC_CLK_TCK);
    long seconds = uptime-(start_time / hertz);

  return seconds;
}

// Done: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 
    //sort according to ram
    long ram1=stol(LinuxParser::Ram(pid_));
    long ram2=stol(LinuxParser::Ram(a.pid_));
    if(ram1>ram2)
        return true;
    else
    {
        return false;
    }
    
 }