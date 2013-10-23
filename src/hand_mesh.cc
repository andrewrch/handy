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

    // Now put all the right numbers in the right place
    matrices["thumbjoint1"] = glm::toMat4(glm::normalize(glm::quat(
            glm::vec3(pose[THUMB_1_X], 0.0f, pose[THUMB_1_Z]))));
    matrices["thumbjoint2"] = glm::toMat4(glm::normalize(glm::quat(
            glm::vec3(pose[THUMB_2_X], 0.0f, 0.0f))));
    matrices["thumbjoint3"] = glm::toMat4(glm::normalize(glm::quat(
            glm::vec3(pose[THUMB_3_X], 0.0f, 0.0f))));

    matrices["indexjoint1"] = glm::toMat4(glm::normalize(glm::quat(
            glm::vec3(pose[INDEX_1_X], 0.0f, pose[INDEX_1_Z]))));
    matrices["indexjoint2"] = glm::toMat4(glm::normalize(glm::quat(
            glm::vec3(pose[INDEX_2_X], 0.0f, 0.0f))));
    matrices["indexjoint3"] = glm::toMat4(glm::normalize(glm::quat(
            glm::vec3(pose[INDEX_3_X], 0.0f, 0.0f))));

    matrices["middlejoint1"] = glm::toMat4(glm::normalize(glm::quat(
            glm::vec3(pose[MIDDLE_1_X], 0.0f, pose[MIDDLE_1_Z]))));
    matrices["middlejoint2"] = glm::toMat4(glm::normalize(glm::quat(
            glm::vec3(pose[MIDDLE_2_X], 0.0f, 0.0f))));
    matrices["middlejoint3"] = glm::toMat4(glm::normalize(glm::quat(
            glm::vec3(pose[MIDDLE_3_X], 0.0f, 0.0f))));

    matrices["ringjoint1"] = glm::toMat4(glm::normalize(glm::quat(
            glm::vec3(pose[RING_1_X], 0.0f, pose[RING_1_Z]))));
    matrices["ringjoint2"] = glm::toMat4(glm::normalize(glm::quat(
            glm::vec3(pose[RING_2_X], 0.0f, 0.0f))));
    matrices["ringjoint3"] = glm::toMat4(glm::normalize(glm::quat(
            glm::vec3(pose[RING_3_X], 0.0f, 0.0f))));
    
    matrices["littlejoint1"] = glm::toMat4(glm::normalize(glm::quat(
            glm::vec3(pose[LITTLE_1_X], 0.0f, pose[LITTLE_1_Z]))));
    matrices["littlejoint2"] = glm::toMat4(glm::normalize(glm::quat(
            glm::vec3(pose[LITTLE_2_X], 0.0f, 0.0f))));
    matrices["littlejoint3"] = glm::toMat4(glm::normalize(glm::quat(
            glm::vec3(pose[LITTLE_3_X], 0.0f, 0.0f))));

    readNodeHierarchy(matrices, 

  }

void HandMesh::ReadNodeHeirarchy(const std::map<std::string,glm::mat4>& matrices,
                                 const aiNode* node, 
                                 const glm::mat4 parentTransform)
{    
  std::string nodeName(node->mName.data);
  glm::mat4 nodeTransformation(1.0f);
  memcpy(&nodeTransformation, &node->mTransformation, 16*sizeof(GLfloat));
  nodeTransformation = glm::transpose(nodeTransformation);

  if (boneMapping.find(nodeName) != boneMapping.end())
    nodeTransformation
       
  glm::mat4 globalTransformation = parentTransform * nodeTransformation;
    
  if (boneMapping.find(nodeName) != boneMapping.end()) {
      uint boneIndex = m_BoneMapping[NodeName];
      boneInfo[boneIndex].finalTransformation = globalInverseTransform * 
                                                globalTransformation * 
                                                m_BoneInfo[BoneIndex].BoneOffset;
  }
  
  // Now process all children
  for (uint i = 0 ; i < node->mNumChildren ; i++) {
      readNodeHeirarchy(matrices, node->mChildren[i], globalTransformation);
  }
}

std::vector<glm::mat4> HandMesh::boneTransform()
{
  glm::mat4 identity(1.0f);

  readNodeHeirarchy(scene->mRootNode, identity);
  std::vector<glm::mat4> transforms(numBones);

  for (uint i = 0 ; i < numBones ; i++)
  {
    transforms[i] = boneInfo[i].finalTransformation;
  }
}

}

