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
        glClearColor(0.0f, 0.1f, 0.0f, 0.0f);
        //glFrontFace(GL_CW);
        glCullFace(GL_BACK);
        glEnable(GL_CULL_FACE);
        //glDepthFunc(GL_GEQUAL);
        glEnable(GL_DEPTH_TEST);
        /* NOTE ******************************************************
         * Basically everything here will be replaced with config file
         * or command line arguments
         ************************************************************/
        glm::vec3 pos(0.0f, 0.0f, 10.0f);
        glm::vec3 target(0.0f, 0.0f, 0.0f);
        glm::vec3 up(0.0, 1.0f, 0.0f);
        float aspect = (float) 640 / 480;
        pipeline.setCamera(pos, target, up);
        pipeline.setPerspectiveProj(45.6f, aspect, 1.0f, 100.0f);   

        shader.loadFromFile(GL_VERTEX_SHADER, "src/shaders/bones.glslv");
        shader.loadFromFile(GL_FRAGMENT_SHADER, "src/shaders/bones.glslf");
        shader.createAndLinkProgram();
        shader.use();

        // WVP
        glm::mat4 wvp = pipeline.getVPTrans();
        GLuint wvpLocation = shader.addUniform("gWVP");
        glUniformMatrix4fv(wvpLocation, 1, false, glm::value_ptr(wvp));

        for (int i = 0; i < 4; i++)
          LOG(INFO) << wvp[i][0] << " "  << wvp[i][1] << " " << wvp[i][2] << " " << wvp[i][3];
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

        hand_mesh.loadMesh("hand_model/hand.dae");
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
  // Parse command line arguments
  handy::HandyOptionsParser parser;
  bool carryOn = parser.parse(argc, argv);

  // If help requested
  if (!carryOn)
    exit(0);

  handy::HandyOptions options;
  try
  {
    options = parser.getHandyOptions();
  }
  catch (const handy::HandyOptionsParsingException& e)
  {
    exit(1);
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
