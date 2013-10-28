#pragma once

#include <cstdio>
#include <cstdlib>
#include <string>
// GL
#include <GL/glew.h>
#include <GL/glfw.h>
// GLM for vectors
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
// Logging
#include <glog/logging.h>
// Handy source files
#include "handy_options.h"
#include "hand_mesh.h"
#include "pipeline.h"
#include "shader.h"

namespace handy
{
  class HandyRenderer
  {
    public:
      // Constructs the renderer
      HandyRenderer(HandyOptions& options);
      // Draws the hand to a file
      void draw(const std::string& file);
    private:
      HandMesh hand_mesh;
      Pipeline pipeline;
      Shader shader;
  };
};


