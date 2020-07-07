#include "gtest/gtest.h"
#include <fstream>
#include <iostream>
#include "format.h"

TEST(MonitorTest, TestTimeFormat){
    // INPUT: Long int measuring seconds
    // OUTPUT: HH:MM:SS
    // Extention: D days, HH:MM:SS for very long uptimes
    EXPECT_EQ(Format::Time(10),"00:00:10");
    EXPECT_EQ(Format::Time(45296), "12:34:56");
    EXPECT_EQ(Format::Time(1594063617), "18449 days, 19:26:57");
}