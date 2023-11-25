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
# Main
###################################################

add_library(Arc OBJECT
        src/arc/util/ArcRuntimeException.h
        src/arc/util/io/Writes.cpp
        src/arc/util/io/Writes.h
        src/arc/util/io/Reads.cpp
        src/arc/util/io/Reads.h
        src/arc/math/Rand.cpp
        src/arc/math/Rand.h
        src/arc/math/geom/Position.h
        src/arc/math/geom/Position.cpp
        src/arc/math/geom/Vec3.cpp
        src/arc/math/geom/Vec3.h
        src/arc/math/Mat.cpp
        src/arc/math/Mat.h
        src/arc/math/Affine2.cpp
        src/arc/math/Affine2.h
)

set_target_properties(Arc PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(Arc SYSTEM PUBLIC ${Boost_INCLUDE_DIR})
target_include_directories(Arc PRIVATE src/arc)

add_executable(Mindustry src/main.cpp)
target_include_directories(Mindustry SYSTEM PUBLIC ${Boost_INCLUDE_DIR})
#target_link_libraries(Mindustry PUBLIC Boost::boost Boost::system Boost::thread)

###################################################
# Doxygen
###################################################

doxygen_add_docs(docs src ALL WARN_IF_UNDOCUMENTED)
