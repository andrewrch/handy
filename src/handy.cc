#include <cstdio>
#include <cstdlib>
#include <string>
#include <exception>
#include <QApplication>
// GL
#include <GL/glew.h>
#include <GL/glfw.h>
// GLM for vectors
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
// Logging
#include <glog/logging.h>
// Handy source files
#include "handy_viewer.h"
#include "handy_renderer.h"
#include "handy_options.h"
#include "hand_mesh.h"
#include "pipeline.h"
#include "shader.h"

void shutdown(int code)
{
  glfwTerminate();
  exit(code);
}

int main(int argc, char* argv[])
{
  bool helpNeeded = false;
  handy::HandyOptions options;
  try
  {
    helpNeeded = options.parse(argc, argv);
  }
  catch (const handy::HandyOptionsRequiredArgException& e)
  {
    std::cout << "Required argument not supplied (" << e.what() << "see " \
      "./handy --help for usage" << std::endl;
    exit(1);
  }
  catch (const handy::HandyOptionsParsingException& e)
  {
    std::cout << "Error parsing command line options, please try " \
      "./handy --help for usage" << std::endl;
    exit(1);
  }

  if (helpNeeded)
  {
    options.printHelp();
    exit(0);
  }

  // Set up log file
  google::InitGoogleLogging(options.getLogfile().c_str());

  // Check whether we should display the viewer
  if (options.showDisplay())
  {
    QApplication a(argc, argv);
    HandyViewer w;
    if (w.init(options))
    {
      w.show();
      return a.exec();
    }
    else
      shutdown(1);
  }

  // **************************************************************
  // 
  // Need to sort this out so that it renders to a renderbuffer and 
  // blits to a file when we don't want the viewer
  //
  //else
  //{
  //  // Initialise GLFW
  //  if (glfwInit() == GL_FALSE)
  //    shutdown(1);
  //  // Get a glfw window
  //  int windowWidth = 640, windowHeight = 480;
  //  if (!glfwOpenWindow(windowWidth, 
  //        windowHeight, 
  //        0, 0, 0, 0, 24, 0, 
  //        GLFW_WINDOW))
  //  {
  //    shutdown(1);
  //  }
  //  glfwSetWindowTitle("Handy");
  //  int major, minor, rev;
  //  glfwGetGLVersion(&major, &minor, &rev);
  //  LOG(INFO) << "OpenGL version received: " \
  //    << major << "." << minor << "." << rev;
  //  glfwEnable(GLFW_STICKY_KEYS);
  //}
  //if (glewInit() != GLEW_OK)
  //  shutdown(1);

  //// Build an app, pass program options
  //handy::HandyApp *app = new handy::HandyApp(options);

  //// Run the main loop until it returns false
  //do {} while (app->run());

  //delete app;
  shutdown(0);
}
