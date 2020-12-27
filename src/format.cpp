#include <string>

#include "format.h"

using std::string;
using std::to_string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string format(string temp){
	if(temp.size()>=2)
		return temp;
	else 
		{
			string res="0";
			res+=temp;
			return res;
		}

}
string Format::ElapsedTime(long seconds) { 
    int days, hours, minutes, secs;

	//  days
	days = seconds / 86400;
	 seconds %= 86400;
	//  hours
	hours = seconds / 3600;
	seconds %= 3600;
	//  minutes
	minutes = seconds / 60;
	//   seconds
	secs = seconds % 60;

	if (days == 0) {
		return format(to_string(hours)) + ":" + format(to_string(minutes)) + ":" + format(to_string(secs));
	}
	else{
		return format(to_string(days)) + " days " + format(to_string(hours)) + ":" + format(to_string(minutes)) + ":" + format(to_string(secs));
	}

 }