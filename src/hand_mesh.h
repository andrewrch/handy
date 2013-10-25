#pragma once

#include "mesh.h"
#include <vector>
#include <glm/gtc/matrix_transform.hpp>

namespace handy
{
  namespace Hand {
    enum class Position { X = 0, Y = 1, Z = 2};
    enum class Rotation { W = 3, X = 4, Y = 5, Z = 6 };
    namespace Thumb {
      enum class Joint1 { Y = 7, Z = 8 };
      enum class Joint2 { Y = 9 };
      enum class Joint3 { Y = 10 };
    }
    namespace Index {
      enum class Joint1 { Y = 11, Z = 12 };
      enum class Joint2 { Y = 13 };
      enum class Joint3 { Y = 14 };
    }
    namespace Middle {
      enum class Joint1 { Y = 15, Z = 16 };
      enum class Joint2 { Y = 17 };
      enum class Joint3 { Y = 18 };
    }
    namespace Ring {
      enum class Joint1 { Y = 19, Z = 20 };
      enum class Joint2 { Y = 21 };
      enum class Joint3 { Y = 22 };
    }
    namespace Little {
      enum class Joint1 { Y = 23, Z = 24 };
      enum class Joint2 { Y = 25 };
      enum class Joint3 { Y = 26 };
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
                             const glm::mat4& parentTransform);

  };
};
