#include "gtest/gtest.h"
#include <fstream>
#include <iostream>
#include "format.h"

TEST(MonitorTest, TestTimeFormat){
    // INPUT: Long int measuring seconds
    // OUTPUT: HH:MM:SS
    EXPECT_EQ(Format::Time(10),"00:00:10");
    EXPECT_EQ(Format::Time(1594063617), "19:26:57");
}