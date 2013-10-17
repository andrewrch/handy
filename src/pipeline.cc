#include "pipeline.h"

namespace handy
{
  Pipeline::Pipeline(unsigned int w, unsigned int h) :
      width(w),
      height(h)
  {
    updateTransformations();
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
