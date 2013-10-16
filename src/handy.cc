#include <cstdio>
#include <cstdlib>
#include <string>
#include <exception>

// GL
#include <GL/glew.h>
#include <GL/glfw.h>
// Logging
#include <glog/logging.h>

#include "handy_options.h"

namespace handy
{
	class HandyApp
	{
		public:
			HandyApp(HandyOptions options)
			{
			}
			bool run()
			{
			}

		private:

	};
};

void shutdown(int code)
{
	glfwTerminate();
	exit(code);
}

int main(int argc, char* argv[])
{
	// Parse command line arguments
	handy::HandyOptionsParser parser;
	bool carryOn = parser.parse(argc, argv);

	if (!carryOn)
		exit(0);

	handy::HandyOptions options = parser.getHandyOptions();

	// Set up log file
	google::InitGoogleLogging(options.getLogfile().c_str());

	LOG(INFO) << "This is a test innit";

	// Build an app, pass command line arguments to be processed
	handy::HandyApp *app = new handy::HandyApp(options);

  // Initialise GLFW
  if (glfwInit() == GL_FALSE)
    shutdown(1);

  // Get a glfw window.  This is required to do any rendering with OpenGL
  int windowWidth = 640, windowHeight = 480;
  if (!glfwOpenWindow(windowWidth, 
  			windowHeight, 
  			0, 0, 0, 0, 24, 0, 
  			GLFW_WINDOW))
    shutdown(1);
  glfwSetWindowTitle("Handy");

  int major, minor, rev;
  glfwGetGLVersion(&major, &minor, &rev);
  fprintf(stderr, "OpenGL version received: %d.%d.%d\n", major, minor, rev);
  //glfwEnable(GLFW_STICKY_KEYS);

  if (glewInit() != GLEW_OK)
    shutdown(1);

  // Run the main loop until it returns false
  do {} while (app->run());

  delete app;
  shutdown(0);
}
