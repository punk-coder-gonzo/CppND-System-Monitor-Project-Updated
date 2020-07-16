#include "gtest/gtest.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <experimental/filesystem>
#include "format.h"
#include "linux_parser.h"
#include "system.h"

namespace fs = std::experimental::filesystem;

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
    EXPECT_FLOAT_EQ(LinuxParser::MemoryUtilization(), 0.779595);
}

TEST(TestLinuxParser, UpTime){
    EXPECT_EQ(LinuxParser::UpTime(), 38492);
}

TEST(TestLinuxParser, TotalProcesses){
    EXPECT_EQ(LinuxParser::TotalProcesses(), 7);
}

TEST(TestSystem, Processes){
    System sys;
    EXPECT_EQ(sys.Processes().size(), 7);
    EXPECT_EQ(sys.Processes().at(0).Pid(), 1046);
    EXPECT_EQ(sys.Processes().at(0).Command(), "VBoxClient");
    EXPECT_EQ(sys.Processes().at(2).Command(), "/usr/sbin/kerneloops");
}

TEST(TestLinuxParser, isProcessDirectory){
    
    fs::path tmpdir{"tmp"};
    fs::remove_all(tmpdir);

    fs::create_directories(tmpdir / "1235");
    
    auto iter = fs::directory_iterator(tmpdir);
    
    EXPECT_TRUE(LinuxParser::isProcessDirectory(*iter)) << "\tTest directory: " << *iter;

   //fs::remove_all(tmpdir);

}

TEST(TestLinuxParser, isNotProcessDirectory){
    fs::path tmpdir{"tmp"};
    
    fs::remove_all(tmpdir);

    fs::create_directories(tmpdir / "A1235");
    fs::create_directories(tmpdir / "1235B");
    fs::create_directories(tmpdir / "12C35");
    
    for (auto& iter : fs::directory_iterator(tmpdir)){
        EXPECT_FALSE(LinuxParser::isProcessDirectory(iter)) << "\tTest directory: " << iter;
    }
    
    //fs::remove_all(tmpdir);
}

TEST(TestLinuxParser, ProcessDirectoryEdgeCase){
    
    fs::path tmpdir{"tmp"};
    
    fs::remove_all(tmpdir);

    // tailing "/"
    fs::create_directories(tmpdir / "666/");
    
    for (auto& iter : fs::directory_iterator(tmpdir)){
        EXPECT_TRUE(LinuxParser::isProcessDirectory(iter)) << "\tTest directory: " << iter;
    }
    
    //fs::remove_all(tmpdir);
}