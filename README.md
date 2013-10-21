handy
=====

*WARNING* This is still in a very early state so doesn't have any real 
functionality.

Renders a hand mesh.  The ultimate aim of the software is to be able to produce 
synthetic data using custom made shaders, allowing easy production of different 
data given the application (E.g. depth data, joint classification, textured 
etc).

The hand mesh is borrowed from [libhand](http://www.libhand.org), and much of 
the code is from the excellent tutorials at 
[OGLdev](http://ogldev.atspace.co.uk/).

Dependencies
------------

I've used a few C++11 features so a recent compiler (GCC >= 4.7) will be needed.

Other library dependencies are:

* GLEW 
* GLFW
* GLM
* Assimp
* Glog
* boost::program\_options
* boost::filesystem
* ImageMagick

Building
--------

My first foray into CMake.  Follow these instructions to (Hopefully) build.

    mkdir build
    cd build
    cmake ../src
    make


