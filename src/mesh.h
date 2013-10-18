/*
 *
 * Copyright 2011 Etay Meiri, modified by Andrew Chambers
 *
 *  This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.

 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.

 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <map>
#include <vector>
#include <GL/glew.h>

// GLM for vectors
#include <glm/glm.hpp>

// Assimp for model loading
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>       // Output data structure
#include <assimp/postprocess.h> // Post processing flags

#include "texture.h"

namespace handy
{
  struct Vertex
  {
      Vertex() {}
      Vertex(const glm::vec3& p, const glm::vec2& t, const glm::vec3& n) 
      	: pos(p), tex(t), normal(n) { }

      glm::vec3 pos;
      glm::vec2 tex;
      glm::vec3 normal;
  };

  class Mesh
  {
    public:
      Mesh() 
      { };

      ~Mesh() 
      { 
        clear(); 
      }

      bool loadMesh(const std::string& filename);
      void render();

    private:
      bool initFromScene(const aiScene* scene, const std::string& filename);
      void initMesh(unsigned int index, const aiMesh* aiMesh);
      bool initMaterials(const aiScene* scene, const std::string& filename);
      void clear();

      static const int INVALID_MATERIAL = 0xFFFFFFFF;
      static const int INVALID_OGL_VALUE = 0xFFFFFFFF;

      struct MeshEntry {
          MeshEntry();
          ~MeshEntry();

          void init(const std::vector<Vertex>& vertices,
                    const std::vector<unsigned int>& indices);

          GLuint VB;
          GLuint IB;
          unsigned int numIndices;
          unsigned int materialIndex;
      };

      // Keep this to support multiple hands
      std::vector<MeshEntry> entries;
      std::vector<Texture*> textures;
  };
};
