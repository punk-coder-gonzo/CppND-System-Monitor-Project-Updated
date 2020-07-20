#include "processor.h"

#include "linux_parser.h"
// TODO: Return the aggregate CPU utilization
using namespace LinuxParser;
float Processor::Utilization() const {
  // https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux

  static std::array<float, 10> prev{0};
  std::vector<std::string> cpustat = CpuUtilization();

  float PrevIdle = prev[kIdle_] + prev[kIOwait_];

  float Idle = std::stof(cpustat[kIdle_]) + std::stof(cpustat[kIOwait_]);

  float PrevNonIdle = prev[kUser_] + prev[kNice_] + prev[kSystem_] +
                      prev[kIRQ_] + prev[kSoftIRQ_] + prev[kSteal_];

  float NonIdle = std::stof(cpustat[kUser_]) + std::stof(cpustat[kNice_]) +
                  std::stof(cpustat[kSystem_]) + std::stof(cpustat[kIRQ_]) +
                  std::stof(cpustat[kSoftIRQ_]) + std::stof(cpustat[kSteal_]);

  float PrevTotal = PrevIdle + PrevNonIdle;
  float Total = Idle + NonIdle;

  float totald = Total - PrevTotal;
  float idled = Idle - PrevIdle;

  for (std::size_t i = 0; i < cpustat.size(); i++) {
    prev[i] = std::stof(cpustat[i]);
  }
  if (totald < std::numeric_limits<float>::min())
    totald = std::numeric_limits<float>::min();

  return (totald - idled) / totald;
}