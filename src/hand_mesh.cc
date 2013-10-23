#include "hand_mesh.h"
#include <map>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>

namespace handy
{
  HandMesh::HandMesh()
  {
  }

  HandMesh::~HandMesh()
  {
  }

  bool HandMesh::loadMesh(const std::string& filename)
  {
    bool ret = Mesh::loadMesh(filename);
    
    // Here do some checking to make sure all bones
    // are there and correct

    return ret;
  }

  std::vector<glm::mat4> HandMesh::setPose(std::vector<double>& pose)
  {
    // Maps a string to a transformation matrix
    std::map<std::string, glm::mat4> matrices;
    // First translate the pose vector in to translations/rotations

    glm::mat4 globalTrans = glm::translate(glm::mat4(1.0), 
        glm::vec3(pose[GLOB_POS_X], pose[GLOB_POS_Y], pose[GLOB_POS_Z]));
    glm::quat quaternionRotation(pose[GLOB_ROT_A], pose[GLOB_ROT_B], 
                                 pose[GLOB_ROT_C], pose[GLOB_ROT_D]);
    // Dont forget to normalise...
    quaternionRotation = glm::normalize(quaternionRotation);
    glm::mat4 globalRot = glm::toMat4(quaternionRotation);
    matrices["root"] = globalTrans * globalRot;

    
        




  }
}

