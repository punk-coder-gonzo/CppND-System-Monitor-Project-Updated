#include <sstream>
#include <string>
#include <iomanip>

#include "format.h"

<<<<<<< HEAD
// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// Extention: D days, HH:MM:SS for very long uptimes
std::string Format::Time(long int second) {
  int nDays{0}, nHours{0}, nMinutes{0}, nSeconds{0};

  nDays = second / (60 * 60 * 24);
  nHours = (second - nDays * 24 * 60 * 60) / (60 * 60);
  nMinutes = (second - nDays * 24 * 60 * 60 - nHours * 60 * 60) / 60;
  nSeconds = (second - nDays * 24 * 60 * 60 - nHours * 60 * 60 - nMinutes * 60);
  
  std::stringstream ss;

  if (nDays > 0) ss << nDays << " days, ";
  ss << std::setw(2) << std::setfill('0') << nHours   << ":"
     << std::setw(2) << std::setfill('0') << nMinutes << ":" 
     << std::setw(2) << std::setfill('0') << nSeconds;

  std::string result{ss.str()};  
  return result;
}
=======
using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds[[maybe_unused]]) { return string(); }
>>>>>>> origin
