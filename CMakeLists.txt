cmake_minimum_required(VERSION 3.25)
project(Mindustry)

set(CMAKE_CXX_STANDARD 23)

include_directories(src)

###################################################
# Packages required
###################################################

find_package(Doxygen
        REQUIRED dot)

set(Boost_USE_MULTITHREADED ON)
find_package(Boost 1.83.0
        REQUIRED
        COMPONENTS)

add_executable(Mindustry src/main.cpp)
target_include_directories(Mindustry SYSTEM PUBLIC ${Boost_INCLUDE_DIR})
#target_link_libraries(Mindustry PUBLIC Boost::boost Boost::system Boost::thread)
