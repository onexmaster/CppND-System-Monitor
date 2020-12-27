#include "processor.h"
#include "linux_parser.h"
#include <vector>
#include <string>
using std::string;
using std::vector;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
	//use the algorithm given on stackoverflow website
//read the info from the file
  string line, token;
  float val;
  vector<float> values;
  std::ifstream filestream(LinuxParser::kProcDirectory+LinuxParser::kStatFilename);
  if(filestream.is_open())
  {
	  while(getline(filestream,line)){
		  std::istringstream linestream(line);
		  while(linestream>>token)
		  {
			  if(token=="cpu")
			  {
				  while(linestream>>val)
				  values.emplace_back(val);
			  }
		  }
	  }
  }
  //Get the required values
  float user = (values[0]);
  float nice = (values[1]);
  float system = (values[2]);
  float idle = (values[3]);
  float iowait = (values[4]);
  float irq = (values[5]);
  float softirq = (values[6]);
  float steal = (values[7]);

  float PrevIdle = previdle + previowait;
  float Idle = idle + iowait;

  float PrevNonIdle =
      prevuser + prevnice + prevsystem + previrq + prevsoftirq + prevsteal;
  float NonIdle = user + nice + system + irq + softirq + steal;

  float PrevTotal = PrevIdle + PrevNonIdle;
  float Total = Idle + NonIdle;

  float totald = Total - PrevTotal;

  float idled = Idle - PrevIdle;

  float CPU_Percentage = (totald - idled) / totald;

   //assing these values as the old values
  prevuser = (values[0]);
  prevnice = (values[1]);
  prevsystem = (values[2]);
  previdle = (values[3]);
  previowait = (values[4]);
  previrq = (values[5]);
  prevsoftirq = (values[6]);
  prevsteal = (values[7]);

  
  return CPU_Percentage;

   
 }