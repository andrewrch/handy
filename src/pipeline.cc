#include "pipeline.h"

namespace handy
{
  Pipeline::Pipeline()
  {
    proj.fovy = 45.0f;
    proj.aspect = 4.0 / 3.0; 
    proj.zNear = 0.0f;
    proj.zFar = 1000.0f;

    // Some reasonably sensible default camera options
    camera.pos = glm::vec3(100.0f, 0.0f, 0.0f);
    camera.up = glm::vec3(0.0f, 1.0f, 0.0f);
    camera.target = glm::vec3(0.0f, 0.0f, 0.0f);
//    updateTransformations();
  }

  void Pipeline::setPerspectiveProj
    (float fovy, float aspect, float zNear, float zFar)
  {
    proj.fovy = fovy;
    proj.aspect = aspect;
    proj.zNear = zNear;
    proj.zFar = zFar;
    updateTransformations();
  }

  void Pipeline::setCamera
    (const glm::vec3& pos, const glm::vec3& target, const glm::vec3& up)
  {
    camera.pos = pos;
    camera.target = target;
    camera.up = up;
    updateTransformations();
  }

  void Pipeline::updateTransformations()
  {
    // View
    VTransformation = glm::lookAt(camera.pos, camera.target, camera.up);
    // View projection
    glm::mat4 projTrans = glm::perspective(proj.fovy, 
                                           proj.aspect, 
                                           proj.zNear, 
                                           proj.zFar);

    VPTransformation = projTrans * VTransformation;
  }
};
