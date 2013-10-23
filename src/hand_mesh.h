#pragma once

#include "mesh.h"
#include <vector>
#include <glm/gtc/matrix_transform.hpp>

namespace handy
{
  class HandMesh : public Mesh
  {
    public:
      HandMesh();
      ~HandMesh();
      std::vector<glm::mat4> setPose(std::vector<double>& pose);
      bool loadMesh(const std::string& filename);

    private:

      enum POSE_INDEXES {
        // Global position
        GLOB_POS_X,
        GLOB_POS_Y,
        GLOB_POS_Z,
        // Global rotation (Quarterion)
        GLOB_ROT_A,
        GLOB_ROT_B,
        GLOB_ROT_C,
        GLOB_ROT_D,
        // Thumb
        THUMB_1_X,
        THUMB_1_Z,
        THUMB_2_X,
        THUMB_3_X,
        // Index finger
        INDEX_1_X,
        INDEX_1_Z,
        INDEX_2_X,
        INDEX_3_X,
        // Middle finger
        MIDDLE_1_X,
        MIDDLE_1_Z,
        MIDDLE_2_X,
        MIDDLE_3_X,
        // Ring
        RING_1_X,
        RING_1_Z,
        RING_2_X,
        RING_3_X,
        // Little
        LITTLE_1_X,
        LITTLE_1_Z,
        LITTLE_2_X,
        LITTLE_3_X,
      };
  };
};
