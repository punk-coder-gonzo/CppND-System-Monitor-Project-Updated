#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <algorithm>
#include <experimental/filesystem>
#include <iostream>
#include <string>
#include <vector>

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
  string os, kernel, ignore;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> ignore >> kernel;
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

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string line;
  string ignore;
  string value;
  float mem_total;
  float mem_free;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    // TODO: DRY!!
    std::getline(filestream, line);
    std::istringstream mem_total_linestream(line);
    mem_total_linestream >> ignore >> mem_total;

    std::getline(filestream, line);
    std::istringstream mem_freelinestream(line);
    mem_freelinestream >> ignore >> mem_free;
  }

  return (mem_total - mem_free) / mem_total;
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() {
  long uptime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
  }
  return uptime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  long uptime = UpTime();

  return uptime * sysconf(_SC_CLK_TCK);
}

vector<string> LinuxParser::ReadPidStatFile(int pid) {
  fs::path proc_directory{kProcDirectory};
  fs::path PID{to_string(pid)};
  fs::path stat_filename{kStatFilename};
  string token;
  vector<string> tokens;

  std::ifstream stream(proc_directory / PID / stat_filename);

  if (stream.is_open()) {
    while (getline(stream, token, ' ')) {
      tokens.emplace_back(token);
    }
  }
  return tokens;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
  auto tokens = ReadPidStatFile(pid);

  return stof(tokens[PidStat::utime]) + stof(tokens[PidStat::stime]) +
         stof(tokens[PidStat::cutime]) + stof(tokens[PidStat::cstime]);
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  auto stat = CpuUtilization();
  long active_jiffies{0};
  for (auto item : stat) {
    active_jiffies += stof(item);
  }
  return active_jiffies;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  auto stat = CpuUtilization();
  return stof(stat[kIdle_]) + stof(stat[kIOwait_]);
}

// DONE: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  vector<string> result;
  std::string value, ignore;
  std::string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> ignore;
    for (int i = 0; i < 10; i++) {
      linestream >> value;
      result.emplace_back(value);
    }
  }
  return result;
}

// https://en.cppreference.com/w/cpp/string/byte/isdigit
unsigned int count_digits(const std::string& s) {
  return std::count_if(s.begin(), s.end(),
                       [](unsigned char c) { return std::isdigit(c); });
}

// DONE:
bool LinuxParser::isProcessDirectory(const fs::directory_entry& d) {
  std::string dirname;

  if (d.path().has_filename()) {
    dirname = d.path().filename();
  } else {
    dirname = d.path().relative_path().filename();
  }

  bool isAllDigit = (count_digits(dirname) == dirname.length());

  return fs::is_directory(d.status()) && isAllDigit;
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  int value{0};

  auto iter = fs::directory_iterator(kProcDirectory);
  value = std::count_if(fs::begin(iter), fs::end(iter), isProcessDirectory);

  return value;
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string key{}, line;
  int value{0};

  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while ("procs_running" != key) {
      getline(stream, line);
      std::istringstream linestream(line);
      linestream >> key >> value;
    }
  }
  return value;
}

// DONE: Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string cmdline;

  fs::path proc_directory{kProcDirectory};
  fs::path cmdline_filename{kCmdlineFilename};
  fs::path PID{to_string(pid)};
  {
    std::ifstream stream(proc_directory / PID / cmdline_filename);
    if (stream.is_open()) {
      std::getline(stream, cmdline, '\0');
      std::stringstream ss(cmdline);
      ss >> cmdline;
    }
    stream.close();
  }

  fs::path status_filename{kStatusFilename};
  std::string ignore;
  if (cmdline.empty()) {
    std::ifstream stream(proc_directory / PID / status_filename);
    if (stream.is_open()) {
      std::getline(stream, cmdline, '\0');
      std::stringstream ss(cmdline);
      ss >> ignore >> cmdline;
    }
  }

  return cmdline;
}

// DONE: Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string key;
  float value;
  string line;

  fs::path proc_directory{kProcDirectory};
  fs::path status_filename{kStatusFilename};
  fs::path PID{to_string(pid)};
  std::ifstream stream(proc_directory / PID / status_filename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::stringstream ss(line);
      while (ss >> key >> value) {
        if (key == "VmSize:") {
          return to_string(value / 1024);
        }
      }
    }
  }

  return to_string(value);
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string key;
  string value;
  string line;

  fs::path proc_directory{kProcDirectory};
  fs::path status_filename{kStatusFilename};
  fs::path PID{to_string(pid)};
  std::ifstream stream(proc_directory / PID / status_filename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::stringstream ss(line);
      while (ss >> key >> value) {
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
  string this_uid = LinuxParser::Uid(pid);
  string line;
  // https://man7.org/linux/man-pages/man5/passwd.5.html
  string name, password, uid;

  std::ifstream stream(kPasswordPath);

  while (getline(stream, line)) {
    std::stringstream ss(line);
    std::vector<string> tokens;
    std::string s;
    while (getline(ss, s, ':')) {
      tokens.emplace_back(s);
      if (s.compare(uid)) {
        name = tokens[0];
        return name;
      }
    }
  }

  return string();
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  auto tokens = ReadPidStatFile(pid);
  long elapsed_time = UpTime() * sysconf(_SC_CLK_TCK) -
                      stol(tokens[PidStat::starttime]);  // in jiffies
  return elapsed_time / sysconf(_SC_CLK_TCK);
}

int LinuxParser::CpuUtilization(int pid) {
  auto tokens = ReadPidStatFile(pid);
  long total_runtime = ActiveJiffies(pid);  // in jiffies
  long elapsed_time = UpTime() * sysconf(_SC_CLK_TCK) -
                      stol(tokens[PidStat::starttime]);  // in jiffies
  return 100 * total_runtime / elapsed_time;
}