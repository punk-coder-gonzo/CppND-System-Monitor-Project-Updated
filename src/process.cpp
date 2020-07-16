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

// TODO: Return this process's ID
int Process::Pid() { return m_pid; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { return 0; }

// TODO: Return the command that generated this process
string Process::Command() {
  if (m_command == "") m_command = LinuxParser::Command(m_pid);
  return m_command;
}

// a zombie has an empty cmdline
// https://man7.org/linux/man-pages/man5/proc.5.html
// kernel threads as well
// https://lkml.org/lkml/2018/5/16/1084
bool Process::is_nullcmd() { return (LinuxParser::Command(m_pid).empty()); }

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(m_pid); }

// TODO: Return the user (name) that generated this process
string Process::User() { return string(); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return 0; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const { return true; }