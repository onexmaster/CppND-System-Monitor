#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
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

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, temp, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> temp >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
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
      }
    }
  }
  closedir(directory);
  return pids;
}

// Done: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  long total_mem, mem_free, buffers, cache;
  string line, key, value;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if(filestream.is_open()) {
    while(std::getline(filestream, line)) {
      std::istringstream stream(line);
      while(stream >> key >> value) {
        if (key == "MemTotal:") {
          total_mem = stol(value);
        } else if (key == "MemFree:") {
          mem_free = stol(value);
        } else if (key == "Cached:") {
          cache= stol(value);
        } else if (key == "Buffers:"){
          buffers =stol(value);
        }

      }
    }
  }
  long MemUsed = total_mem - mem_free -( cache + buffers);
  return (float) MemUsed / total_mem;
 }

// Done: Read and return the system uptime
long LinuxParser::UpTime() { 
   long int uptime;
  string line, value;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if(filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream stream(line);
    if(stream >> value) {
      uptime = stol(value);
    }
  }
  return uptime;
  
 }

// Done: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  return UpTime()*sysconf(_SC_CLK_TCK);
 }



// Done: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  //use the cpuUtilization function values 
  long int val=0;
  vector<string>temp=LinuxParser::CpuUtilization();
  val= stol(temp[CPUStates::kUser_]) + stol(temp[CPUStates::kNice_]) + stol(temp[CPUStates::kSystem_]) + stol(temp[CPUStates::kIRQ_]) + stol(temp[CPUStates::kSoftIRQ_]) + stol(temp[CPUStates::kSteal_]);
  return val;
 }

// Done: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  long int val=0;
  vector<string>temp=LinuxParser::CpuUtilization();
  val=(stol(temp[CPUStates::kIdle_]) + stol(temp[CPUStates::kIOwait_]));
  return val;

 }

// Done: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  vector<string> utilization;
  string line, value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream stream(line);
    if (stream >> value) {
      if (value == "cpu") {
        while (stream >> value)
          utilization.push_back(value);
      }
    }
  }
  return utilization;
}

// Done: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  int total_process;
  string line, key, value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if(filestream.is_open()) {
    while(std::getline(filestream, line)) {
      std::istringstream stream(line);
      while(stream >> key >> value) {
        if(key == "processes") {
          total_process = stoi(value);
        }
      }
    }
  }
  return total_process;
 }

// Done: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
    int process_run;
  string line, key, value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if(filestream.is_open()) {
    while(std::getline(filestream, line)) {
      std::istringstream stream(line);
      while(stream >> key >> value) {
        if(key == "procs_running") {
          process_run = stoi(value);
        }
      }
    }
  }
  return process_run;
 }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
  string line, token;
  vector<string> values;
  long total_time{0};
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream stream(line);
    while (stream >> token) {
      values.push_back(token);
    }
  }
   if (values.size() > 21) {
    long utime = stol(values[13]);
    long stime = stol(values[14]);
    long cutime = stol(values[15]);
    long cstime = stol(values[16]);
    total_time = utime + stime + cutime + cstime;
  
  }
  return total_time;
}

// Done: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string line;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream,line);
    return line;
  }
  return string("");
}

// Done: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string line, key, value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream stream(line);
      while(stream >> key >> value) {
        if (key == "VmSize:") {
          //Return in MB
          return std::to_string(stol(value) / 1024);
        }
      }
    }
  }
  return string("0");
}

// Done: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string line, key, value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      std::istringstream stream(line);
      while (stream >> key >> value) {
        if (key == "Uid:") {
          return value;
        }
      }
    }
  }
  return value;
  
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  //get the uid
  string uid=Uid(pid);
  string value1, value2, value3, line;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream stream(line);
      while (stream >> value1 >> value2 >> value3) {
        //match uid with username
        if (value2 == "x" && value3 == uid) {
          return value1;
        }
      }
    }
  }
  return "";

}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  string line, value;
  int i;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream stream(line);
    i=0;
    while (stream >> value){
      if(i==21){
        return (stol(value)/sysconf(_SC_CLK_TCK));
      }
      i++;
      }
    }
  	return 0;
}