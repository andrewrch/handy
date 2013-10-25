#include <cstdio>
#include <cstdlib>
#include <string>
#include <exception>

// GL
#include <GL/glew.h>
#include <GL/glfw.h>
// GLM for vectors
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
// Logging
#include <glog/logging.h>

#include "handy_options.h"
#include "hand_mesh.h"
#include "pipeline.h"
#include "shader.h"

namespace handy
{
  class HandyApp
  {
    public:
      HandyApp(HandyOptions options)
      {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        //glFrontFace(GL_CW);
        glCullFace(GL_BACK);
        glEnable(GL_CULL_FACE);
        //glDepthFunc(GL_GEQUAL);
        glEnable(GL_DEPTH_TEST);
        /* NOTE ******************************************************
         * Basically everything here will be replaced with config file
         * or command line arguments
         ************************************************************/
        float aspect = (float) 640 / 480;
        pipeline.setCamera(options.getCameraPos(), 
                           options.getCameraTarget(), 
                           options.getCameraUp());
        pipeline.setPerspectiveProj(45.6f, aspect, 1.0f, 100.0f);   

        shader.loadFromFile(GL_VERTEX_SHADER, options.getVertexShader());
        shader.loadFromFile(GL_FRAGMENT_SHADER, options.getFragmentShader());
        shader.createAndLinkProgram();
        shader.use();

        // WVP
        glm::mat4 wvp = pipeline.getVPTrans();
        GLuint wvpLocation = shader.addUniform("gWVP");
        glUniformMatrix4fv(wvpLocation, 1, false, glm::value_ptr(wvp));
        // WV
        glm::mat4 world = pipeline.getVTrans();
        GLuint worldLocation = shader.addUniform("gWorld");
        glUniformMatrix4fv(worldLocation, 1, false, glm::value_ptr(world));
        // Texture
        GLuint sampler = shader.addUniform("gSampler");
        glUniform1i(sampler, 0);

        const int BONES = 100;
        GLuint bones[BONES];
        for (int i = 0; i < BONES; i++)
        {
          char name[128];
          memset(name, 0, sizeof(name));
          snprintf(name, sizeof(name), "gBones[%d]", i);
          bones[i] = shader.addUniform(name);
        }

        hand_mesh.loadMesh(options.getHandFile());
        std::vector<double> pose(27);
        for (int i = 0; i < 27; i++)
          pose[i] = 0;

        pose[0] = 0.0;
        pose[10] = 0.5;
        pose[17] = 1.0;

        auto transforms = hand_mesh.setPose(pose);

        std::cout << "Number bones " << transforms.size() << std::endl;

        for (int i = 0; i < transforms.size(); i++)
          glUniformMatrix4fv(bones[i], 1, false, glm::value_ptr(transforms[i]));
      }

      bool run()
      {
        if (glfwGetKey(GLFW_KEY_ESC) == GLFW_PRESS)
          return false;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        hand_mesh.render();
        glfwSwapBuffers();

        return true;
      }

    private:
      HandMesh hand_mesh;
      Pipeline pipeline;
      Shader shader;
  };
};

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
  LOG(INFO) << "OpenGL version received: " \
    << major << "." << minor << "." << rev;
  glfwEnable(GLFW_STICKY_KEYS);

  if (glewInit() != GLEW_OK)
    shutdown(1);

  // Build an app, pass program options
  handy::HandyApp *app = new handy::HandyApp(options);

  // Run the main loop until it returns false
  do {} while (app->run());

  delete app;
  shutdown(0);
}
