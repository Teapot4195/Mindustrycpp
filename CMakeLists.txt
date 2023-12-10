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

###################################################
# Main
###################################################

add_library(Arc OBJECT
        src/arc/files/Fi.cpp
        src/arc/Utilities.h
        src/arc/Utilities.h
        src/arc/util/Exception.h
        src/arc/util/io/Writes.cpp
        src/arc/util/io/Writes.h
        src/arc/util/io/Reads.cpp
        src/arc/util/io/Reads.h
        src/arc/util/Disposable.h
        src/arc/func.h
        src/arc/graphics/Color.cpp
        src/arc/graphics/Color.h
        src/arc/math/Mathf.cpp
        src/arc/math/Mathf.h
        src/arc/math/Rand.cpp
        src/arc/math/Rand.h
        src/arc/math/geom/Vector.h
        src/arc/math/Interp.h
        src/arc/math/geom/Position.h
        src/arc/math/Angles.cpp
        src/arc/math/Angles.h
        src/arc/math/geom/Position.cpp
        src/arc/math/geom/Vec2.cpp
        src/arc/math/geom/Vec2.h
        src/arc/math/geom/Vec3.cpp
        src/arc/math/geom/Vec3.h
        src/arc/math/Mat.cpp
        src/arc/math/Mat.h
        src/arc/math/Affine2.cpp
        src/arc/math/Affine2.h
        src/arc/util/Time.cpp
        src/arc/util/Time.h
        src/arc/struct.h
        src/arc/util/pooling/Pool.h
        src/arc/util/pooling/Pools.h
        src/arc/util/Timer.cpp
        src/arc/util/Timer.h
        src/arc/ApplicationListener.h
        src/arc/Application.h
        src/arc/Application.cpp
        src/arc/graphics/GLTexture.cpp
        src/arc/graphics/GLTexture.h
        src/arc/graphics/Gl.cpp
        src/arc/graphics/Gl.h
        src/arc/graphics/GL20.h
        src/arc/Utils.h
        src/arc/graphics/TextureData.h
        src/arc/graphics/gl/FileTextureData.cpp
        src/arc/graphics/gl/FileTextureData.h
        src/arc/graphics/TextureData.cpp
        src/arc/graphics/PixmapIO.cpp
        src/arc/graphics/PixmapIO.h
        src/arc/util/io/WritesDeflate.cpp
        src/arc/util/io/WritesDeflate.h
        src/arc/util/io/ReadsDeflate.cpp
        src/arc/util/io/ReadsDeflate.h
        src/arc/util/Pack.h
        src/arc/graphics/gl/MipMapGenerator.cpp
        src/arc/graphics/gl/MipMapGenerator.h
        src/arc/Graphics.cpp
        src/arc/Graphics.h
        src/arc/graphics/gl/GLVersion.cpp
        src/arc/graphics/gl/GLVersion.h
        src/arc/util/Log.cpp
        src/arc/util/Log.h
        src/arc/util/ColorCodes.cpp
        src/arc/util/ColorCodes.h
        src/arc/util/OS.cpp
        src/arc/util/OS.h
        src/arc/graphics/Texture.cpp
        src/arc/graphics/Texture.h
        src/arc/graphics/gl/PixmapTextureData.cpp
        src/arc/graphics/gl/PixmapTextureData.h
        src/arc/math/geom/Path.h
        src/arc/math/geom/Bezier.h
        src/arc/math/geom/Rect.cpp
        src/arc/math/geom/Rect.h
        src/arc/math/geom/Shape2D.h
        src/arc/math/geom/Circle.cpp
        src/arc/math/geom/Circle.h
        src/arc/math/geom/Point2.cpp
        src/arc/math/geom/Point2.h
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
