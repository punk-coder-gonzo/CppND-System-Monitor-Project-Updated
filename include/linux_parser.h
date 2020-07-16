#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>
#include <experimental/filesystem>

namespace LinuxParser {

namespace fs = std::experimental::filesystem;

// Paths
#ifndef TEST_DATA_DIR
#define TEST_DATA_DIR
#endif
#define ADD_ROOT_DIR(x) TEST_DATA_DIR x

const std::string kProcDirectory{ADD_ROOT_DIR("/proc/")};
const std::string kCmdlineFilename{"/cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"};
const std::string kStatusFilename{"/status"};
const std::string kStatFilename{"/stat"};
const std::string kUptimeFilename{"/uptime"};
const std::string kMeminfoFilename{"/meminfo"};
const std::string kVersionFilename{"/version"};
const std::string kOSPath{ADD_ROOT_DIR("/etc/os-release")};
const std::string kPasswordPath{ADD_ROOT_DIR("/etc/passwd")};

// System
float MemoryUtilization();
long UpTime();
std::vector<int> Pids();
int TotalProcesses();
int RunningProcesses();
std::string OperatingSystem();
std::string Kernel();

// CPU
enum CPUStates {
  kUser_ = 0,
  kNice_,
  kSystem_,
  kIdle_,
  kIOwait_,
  kIRQ_,
  kSoftIRQ_,
  kSteal_,
  kGuest_,
  kGuestNice_
};
std::vector<std::string> CpuUtilization();
long Jiffies();
long ActiveJiffies();
long ActiveJiffies(int pid);
long IdleJiffies();

// Processes
std::string Command(int pid);
std::string Ram(int pid);
std::string Uid(int pid);
std::string User(int pid);
long int UpTime(int pid);

// Helper Functions
bool isProcessDirectory(const fs::directory_entry& d);
}  // namespace LinuxParser

#endif