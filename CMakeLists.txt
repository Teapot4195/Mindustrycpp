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

file(DOWNLOAD "https://raw.githubusercontent.com/vector-of-bool/cmrc/master/CMakeRC.cmake"
        "${CMAKE_BINARY_DIR}/CMakeRC.cmake")
include("${CMAKE_BINARY_DIR}/CMakeRC.cmake")

###################################################
# Pack Resources for jar
###################################################

#cmrc_add_resource_library(
#        app1-resources
#
#        ALIAS app1::rc
#        NAMESPACE app1
#
#        resources/protocols.txt
#        resources/hosts.txt
#        resources/terminal.jpeg
#)
#
#add_executable(application1 application1.cpp)
#target_link_libraries(application1 app1::rc)

###################################################
# Submodule libraries
###################################################

add_library(lopepng OBJECT
        libs/lodepng/lodepng.cpp
        libs/lodepng/lodepng.h
)

add_subdirectory(libs/infoware)

set(REPROC++ ON)
add_subdirectory(libs/reproc)

add_subdirectory(tests)

###################################################
# Main
###################################################

add_library(Arc OBJECT
)

set_target_properties(Arc PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(Arc SYSTEM PUBLIC ${Boost_INCLUDE_DIR} ${infoware_public_headers})
target_include_directories(Arc PRIVATE src/arc libs/lodepng)

target_link_libraries(Arc PUBLIC lodepng infoware reproc++)

add_executable(Mindustry src/main.cpp)
target_include_directories(Mindustry SYSTEM PUBLIC ${Boost_INCLUDE_DIR})
#target_link_libraries(Mindustry PRIVATE Arc)
#target_link_libraries(Mindustry PUBLIC Boost::boost Boost::system Boost::thread)

###################################################
# Doxygen
###################################################

doxygen_add_docs(docs src ALL WARN_IF_UNDOCUMENTED)
