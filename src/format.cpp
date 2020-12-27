#include <string>

#include "format.h"

using std::string;
using std::to_string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
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
		return to_string(hours) + ":" + to_string(minutes) + ":" + to_string(secs);
	}
	else{
		return to_string(days) + " days " + to_string(hours) + ":" + to_string(minutes) + ":" + to_string(secs);
	}

 }