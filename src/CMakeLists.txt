cmake_minimum_required(VERSION 2.8)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++0x")

# Search for packages that provide cmake configuration
find_package(OpenGL)
find_package(ImageMagick)
find_package(ImageMagick COMPONENTS Magick++)
find_package(GLEW REQUIRED)

# Search for packages do not provide cmake config but have pkgconfig
find_package(PkgConfig REQUIRED)

find_package(Assimp REQUIRED)
if(Assimp_FOUND)
  message(STATUS "Found Assimp: ${Assimp_LIBRARIES}")
endif(Assimp_FOUND)

# Hide this for now as I don't need glfw3
#pkg_search_module(GLFW REQUIRED glfw3) 
# provide GLFW_INCLUDE_DIRS, and GLFW_LIBRARIES

pkg_search_module(GLFW REQUIRED libglfw)
if(GLFW_FOUND)
message(STATUS "Found GLFW: ${GLFW_LIBRARIES}")
endif(GLFW_FOUND)

# Mannually set up other dependencies which probably have CMake configuraiton 
# but I just don't bother to find out :-p
set(EXTRA_LIBS "-lglog;;-lboost_program_options;-lboost_filesystem;-lboost_system" CACHE STRING "Other dependencies.")


include_directories(${ImageMagick_INCLUDE_DIRS};${ImageMagick_Magick++_INCLUDE_DIRS};)

# Collect all .cc files from source dir and add them to compile dependency
AUX_SOURCE_DIRECTORY(${CMAKE_SOURCE_DIR} ALL_CC_FILES)

add_executable(handy handy.cc ${ALL_CC_FILES})
target_link_libraries(handy 
  ${ImageMagick_LIBRARIES};${GLEW_LIBRARIES};${OPENGL_LIBRARIES};${GLFW_LIBRARIES};${Assimp_LIBRARIES};${EXTRA_LIBS})
