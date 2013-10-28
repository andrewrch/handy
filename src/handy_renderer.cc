#include "handy_renderer.h"

namespace handy
{
  HandyRenderer::HandyRenderer(HandyOptions& options)
  {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    //glFrontFace(GL_CW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    //glDepthFunc(GL_GEQUAL);
    glEnable(GL_DEPTH_TEST);

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
    auto transforms = hand_mesh.setPose(options.getPoseVector());

    for (int i = 0; i < transforms.size(); i++)
      glUniformMatrix4fv(bones[i], 1, false, glm::value_ptr(transforms[i]));
  }

  void HandyRenderer::draw(const std::string& filename)
  {
    //if (glfwGetKey(GLFW_KEY_ESC) == GLFW_PRESS)
    //  return false;

    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //hand_mesh.render();
    //glfwSwapBuffers();

    //return true;
  }
};


