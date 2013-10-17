#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>
#include <vector>
#include <iostream>

namespace handy
{
  class Pipeline
  {
    public:
      Pipeline();

      void setPerspectiveProj(
          float fovy, 
          float aspect, 
          float zNear, 
          float zFar);

      void setCamera(
          const glm::vec3& pos, 
          const glm::vec3& target, 
          const glm::vec3& up);

      inline const glm::mat4& getVTrans() 
      { 
        return VTransformation; 
      }
      inline const glm::mat4& getVPTrans() 
      { 
        return VPTransformation; 
      }

    private:
      struct {
        float fovy;
        float aspect;
        float zNear;
        float zFar;
      } proj;

      struct {
        glm::vec3 pos;
        glm::vec3 target;
        glm::vec3 up;
      } camera;

      glm::mat4 VPTransformation;
      glm::mat4 VTransformation;

      void updateTransformations();
  };
};
