#pragma once

#include "mesh.h"
#include <vector>
#include <glm/gtc/matrix_transform.hpp>

namespace handy
{
  namespace Hand {
    enum class Position { X = 0, Y = 1, Z = 2};
    enum class Rotation { A = 3, B = 4, C = 5, D = 6 };
    namespace Thumb {
      enum class Joint1 { X = 7, Z = 8 };
      enum class Joint2 { X = 9 };
      enum class Joint3 { X = 10 };
    }
    namespace Index {
      enum class Joint1 { X = 11, Z = 12 };
      enum class Joint2 { X = 13 };
      enum class Joint3 { X = 14 };
    }
    namespace Middle {
      enum class Joint1 { X = 15, Z = 16 };
      enum class Joint2 { X = 17 };
      enum class Joint3 { X = 18 };
    }
    namespace Ring {
      enum class Joint1 { X = 19, Z = 20 };
      enum class Joint2 { X = 21 };
      enum class Joint3 { X = 22 };
    }
    namespace Little {
      enum class Joint1 { X = 23, Z = 24 };
      enum class Joint2 { X = 25 };
      enum class Joint3 { X = 26 };
    }
  }
  class HandMesh : public Mesh
  {
    public:
      HandMesh();
      ~HandMesh();
      std::vector<glm::mat4> setPose(std::vector<double>& pose);
      bool loadMesh(const std::string& filename);

    private:

      std::map<std::string, glm::mat4> getPoseMatrices(
          const std::vector<double>& p);
      void readNodeHeirarchy(const std::map<std::string,glm::mat4>& matrices,
                             const aiNode* node, 
                             const glm::mat4 parentTransform);

//      enum class PoseIndex 
//      {
//        // Global position
//        GLOB_POS_X,
//        GLOB_POS_Y,
//        GLOB_POS_Z,
//        // Global rotation (Quarterion)
//        GLOB_ROT_A,
//        GLOB_ROT_B,
//        GLOB_ROT_C,
//        GLOB_ROT_D,
//        // Thumb
//        THUMB_1_X,
//        THUMB_1_Z,
//        THUMB_2_X,
//        THUMB_3_X,
//        // Index finger
//        INDEX_1_X,
//        INDEX_1_Z,
//        INDEX_2_X,
//        INDEX_3_X,
//        // Middle finger
//        MIDDLE_1_X,
//        MIDDLE_1_Z,
//        MIDDLE_2_X,
//        MIDDLE_3_X,
//        // Ring
//        RING_1_X,
//        RING_1_Z,
//        RING_2_X,
//        RING_3_X,
//        // Little
//        LITTLE_1_X,
//        LITTLE_1_Z,
//        LITTLE_2_X,
//        LITTLE_3_X,
//      };
  };
};
