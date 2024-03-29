#include "hand_mesh.h"
#include <map>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include <iostream>

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

  std::map<std::string, glm::mat4> HandMesh::getPoseMatrices(
      const std::vector<double>& pose)
  {
    // Maps a string to a transformation matrix
    std::map<std::string, glm::mat4> matrices;
    // First translate the pose vector in to translations/rotations
    glm::mat4 globalTrans = glm::translate(glm::mat4(1.0), 
        glm::vec3(pose[static_cast<unsigned int>(Hand::Position::X)], 
                  pose[static_cast<unsigned int>(Hand::Position::Y)], 
                  pose[static_cast<unsigned int>(Hand::Position::Z)]));
    glm::mat4 globalRot = glm::toMat4(glm::normalize(glm::quat(
                  pose[static_cast<unsigned int>(Hand::Rotation::W)], 
                  pose[static_cast<unsigned int>(Hand::Rotation::X)],
                  pose[static_cast<unsigned int>(Hand::Rotation::Y)],
                  pose[static_cast<unsigned int>(Hand::Rotation::Z)])));

    //logMatrix(globalTrans, "Global translate");
    //logMatrix(globalRot, "Global rotate");

    matrices["carpals"] = globalTrans * globalRot;

    // Now put all the right numbers in the right place
    // // Beware this isn't very nice code :(
    matrices["thumbjoint1"] = glm::toMat4(glm::normalize(glm::quat(
          glm::vec3(0.0f,
                    pose[static_cast<unsigned int>(Hand::Thumb::Joint1::Y)],
                    pose[static_cast<unsigned int>(Hand::Thumb::Joint1::Z)]))));
    matrices["thumbjoint2"] = glm::toMat4(glm::normalize(glm::quat(
          glm::vec3(0.0f,
                    pose[static_cast<unsigned int>(Hand::Thumb::Joint2::Y)],
                    0.0f))));
    matrices["thumbjoint3"] = glm::toMat4(glm::normalize(glm::quat(
          glm::vec3(0.0f,
                    pose[static_cast<unsigned int>(Hand::Thumb::Joint3::Y)],
                    0.0f))));
    matrices["indexjoint1"] = glm::toMat4(glm::normalize(glm::quat(
          glm::vec3(0.0f,
                    pose[static_cast<unsigned int>(Hand::Index::Joint1::Y)],
                    pose[static_cast<unsigned int>(Hand::Index::Joint1::Z)]))));
    matrices["indexjoint2"] = glm::toMat4(glm::normalize(glm::quat(
          glm::vec3(0.0f,
                    pose[static_cast<unsigned int>(Hand::Index::Joint2::Y)],
                    0.0f))));
    matrices["indexjoint3"] = glm::toMat4(glm::normalize(glm::quat(
          glm::vec3(0.00f,
                    pose[static_cast<unsigned int>(Hand::Index::Joint3::Y)],
                    0.0f))));
    matrices["middlejoint1"] = glm::toMat4(glm::normalize(glm::quat(
          glm::vec3(0.0f,
                    pose[static_cast<unsigned int>(Hand::Middle::Joint1::Y)],
                    pose[static_cast<unsigned int>(Hand::Middle::Joint1::Z)]))));
    matrices["middlejoint2"] = glm::toMat4(glm::normalize(glm::quat(
          glm::vec3(0.0f,
                    pose[static_cast<unsigned int>(Hand::Middle::Joint2::Y)],
                    0.0f))));
    matrices["middlejoint3"] = glm::toMat4(glm::normalize(glm::quat(
          glm::vec3(0.0f,
                    pose[static_cast<unsigned int>(Hand::Middle::Joint3::Y)],
                    0.0f))));
    matrices["ringjoint1"] = glm::toMat4(glm::normalize(glm::quat(
          glm::vec3(0.0f,
                    pose[static_cast<unsigned int>(Hand::Ring::Joint1::Y)],
                    pose[static_cast<unsigned int>(Hand::Ring::Joint1::Z)]))));
    matrices["ringjoint2"] = glm::toMat4(glm::normalize(glm::quat(
          glm::vec3(0.0f,
                    pose[static_cast<unsigned int>(Hand::Ring::Joint2::Y)],
                    0.0f))));
    matrices["ringjoint3"] = glm::toMat4(glm::normalize(glm::quat(
          glm::vec3(0.0f,
                    pose[static_cast<unsigned int>(Hand::Ring::Joint3::Y)],
                    0.0f))));
    matrices["littlejoint1"] = glm::toMat4(glm::normalize(glm::quat(
          glm::vec3(0.0f,
                    pose[static_cast<unsigned int>(Hand::Little::Joint1::Y)],
                    pose[static_cast<unsigned int>(Hand::Little::Joint1::Z)]))));
    matrices["littlejoint2"] = glm::toMat4(glm::normalize(glm::quat(
          glm::vec3(0.0f,
                    pose[static_cast<unsigned int>(Hand::Little::Joint2::Y)],
                    0.0f))));
    matrices["littlejoint3"] = glm::toMat4(glm::normalize(glm::quat(
          glm::vec3(0.0f,
                    pose[static_cast<unsigned int>(Hand::Little::Joint3::Y)],
                    0.0f))));
    return matrices;
  }

  std::vector<glm::mat4> HandMesh::setPose(std::vector<double>& pose)
  {
    // First decode the pose vector into a map
    std::map<std::string, glm::mat4> matrices = getPoseMatrices(pose);

    // Now using those matrices calculate bone matrices given initial skeleton
    glm::mat4 identity(1.0f);
    readNodeHeirarchy(matrices, scene->mRootNode, identity);
    
    // And store them in a vector
    std::vector<glm::mat4> transforms(numBones);
    for (uint i = 0 ; i < numBones ; i++)
      transforms[i] = boneInfo[i].finalTransformation;
    return transforms;
  }

  void HandMesh::readNodeHeirarchy(const std::map<std::string,glm::mat4>& matrices,
                                   const aiNode* node, 
                                   const glm::mat4& parentTransform)
  { 
    std::string nodeName(node->mName.data);
    glm::mat4 nodeTransformation;
    memcpy(&nodeTransformation, &node->mTransformation, 16*sizeof(GLfloat));
    nodeTransformation = glm::transpose(nodeTransformation);

    //logMatrix(nodeTransformation, nodeName);

    if (matrices.find(nodeName) != matrices.end())
      nodeTransformation = nodeTransformation * matrices.find(nodeName)->second;
         
    glm::mat4 globalTransformation = parentTransform * nodeTransformation;
      
    if (boneMapping.find(nodeName) != boneMapping.end()) {
        uint boneIndex = boneMapping[nodeName];
        boneInfo[boneIndex].finalTransformation = globalInverseTransform * 
                                                  globalTransformation * 
                                                  boneInfo[boneIndex].boneOffset;

        //logMatrix(globalTransformation, "Global trans");
        //logMatrix(boneInfo[boneIndex].boneOffset, "Bone offset");
        //logMatrix(nodeTransformation, "Node tranformation");
        //logMatrix(boneInfo[boneIndex].finalTransformation, "Final trans");
    }
    
    // Now process all children
    for (uint i = 0 ; i < node->mNumChildren ; i++) {
        readNodeHeirarchy(matrices, node->mChildren[i], globalTransformation);
    }
  }
}

