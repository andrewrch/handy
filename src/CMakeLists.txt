cmake_minimum_required(VERSION 2.8.8)
PROJECT(handy)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11")
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Search for packages that provide cmake configuration
find_package(OpenGL REQUIRED)
find_package(ImageMagick REQUIRED)
find_package(ImageMagick COMPONENTS Magick++ REQUIRED)
find_package(GLEW REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(PkgConfig REQUIRED)
#find_package(Assimp REQUIRED)
pkg_search_module(GLFW REQUIRED libglfw)
pkg_search_module(Assimp REQUIRED assimp)

# Search for packages do not provide cmake config but have pkgconfig

QT5_WRAP_UI(UIS_HDRS
    ui/handy_viewer.ui
)

set(EXTRA_LIBS "-lglog;-lboost_program_options;-lboost_filesystem;-lboost_system" CACHE STRING "Other dependencies.")

include_directories(
${ImageMagick_INCLUDE_DIRS}
${ImageMagick_Magick++_INCLUDE_DIRS}
${Assimp_INCLUDE_DIRS}
)

# Collect all .cc files from source dir and add them to compile dependency
AUX_SOURCE_DIRECTORY(${CMAKE_SOURCE_DIR} ALL_CC_FILES)

add_executable(handy handy.cc ${ALL_CC_FILES} ${UIS_HDRS})
target_link_libraries(handy 
${ImageMagick_LIBRARIES}
${GLEW_LIBRARIES}
${OPENGL_LIBRARIES}
${GLFW_LIBRARIES}
${Assimp_LIBRARIES}
${EXTRA_LIBS})

qt5_use_modules(handy Widgets)
