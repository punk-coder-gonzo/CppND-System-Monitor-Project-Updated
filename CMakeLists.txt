cmake_minimum_required(VERSION 2.6)


# https://cmake.org/cmake/help/v3.0/policy/CMP0048.html
cmake_policy(SET CMP0048 NEW)

project(monitor)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIRS})

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

add_executable(monitor ${SOURCES})

set_property(TARGET monitor PROPERTY CXX_STANDARD 17)
target_link_libraries(monitor ${CURSES_LIBRARIES})
# TODO: Run -Werror in CI.
target_compile_options(monitor PRIVATE -Wall -Wextra)

add_subdirectory(thirdparty/googletest)

 #Add the testing executable
 add_executable(test EXCLUDE_FROM_ALL
    test/test_monitor.cpp 
    src/format.cpp 
    src/ncurses_display.cpp 
    src/process.cpp 
    src/processor.cpp 
    src/system.cpp
    src/linux_parser.cpp)
target_link_libraries(test 
    gtest_main 
    ${CURSES_LIBRARIES}
)