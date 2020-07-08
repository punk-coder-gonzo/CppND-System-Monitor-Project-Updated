#include "gtest/gtest.h"
#include <fstream>
#include <iostream>
#include "format.h"
#include "linux_parser.h"

TEST(TestFormat, ElapsedTime){
    // INPUT: Long int measuring seconds
    // OUTPUT: HH:MM:SS
    // Extention: D days, HH:MM:SS for very long uptimes
    EXPECT_EQ(Format::ElapsedTime(10),"00:00:10");
    EXPECT_EQ(Format::ElapsedTime(45296), "12:34:56");
    EXPECT_EQ(Format::ElapsedTime(1594063617), "18449 days, 19:26:57");
}

TEST(TestLinuxParser, OperatingSystem){
    EXPECT_EQ(LinuxParser::OperatingSystem(), "Ubuntu 18.04.4 LTS");
}

TEST(TestLinuxParser, Kernel){
    EXPECT_EQ(LinuxParser::Kernel(), "5.3.0-62-generic");
}

TEST(TestLinuxParser, Pids){
    std::vector<int> pids = LinuxParser::Pids();
    EXPECT_EQ(pids.size(), 7);
}

TEST(TestLinuxParser, MemoryUtilization){
    EXPECT_EQ(LinuxParser::MemoryUtilization(), 3141624);
}

TEST(TestLinuxParser, UpTime){
    EXPECT_EQ(LinuxParser::UpTime(), 38492);
}