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
      Mesh();
      ~Mesh();
      bool loadMesh(const std::string& filename);
      void render();

    private:

      static const int INVALID_MATERIAL = 0xFFFFFFFF;
      static const int INVALID_OGL_VALUE = 0xFFFFFFFF;
      static const int NUM_BONES_PER_VERTEX = 10;

      struct BoneInfo
      {
        glm::mat4 boneOffset;
        glm::mat4 finalTransformation;        

        BoneInfo() : boneOffset(0), finalTransformation(0)
        { }
      };
    
      struct VertexBoneData
      {        
        uint ids[NUM_BONES_PER_VERTEX];
        float weights[NUM_BONES_PER_VERTEX];

        VertexBoneData()
        {
          reset();
        };
        
        void reset()
        {
          memset(ids, 0, sizeof(ids));
          memset(weights, 0, sizeof(weights));
        }
        
        void addBoneData(uint boneID, float weight);
      };


      bool initFromScene(const std::string& filename);
      void initMesh(unsigned int index, const aiMesh* mesh,
                    std::vector<glm::vec3>& positions,
                    std::vector<glm::vec3>& normals,
                    std::vector<glm::vec2>& texCoords,
                    std::vector<VertexBoneData>& bones,
                    std::vector<unsigned int>& indices);
      void loadBones(uint meshIndex, 
                     const aiMesh* mesh, 
                     std::vector<VertexBoneData>& bones);
      bool initMaterials(const std::string& filename);
      void clear();


      enum VB_TYPES {
        INDEX_BUFFER = 0,
        POS_VB = 1,
        NORMAL_VB = 2,
        TEXCOORD_VB = 3,
        BONE_VB = 4,
        // Cheeky
        NUM_VBs = 5            
      };

      enum VB_LOCATIONS {
        POSITION_LOCATION = 0,
        TEX_COORD_LOCATION = 1, 
        NORMAL_LOCATION = 2,
        BONE_ID_LOCATION = 3,
        BONE_WEIGHT_LOCATION = 4 
      };

      // Pointers for the VAO
      GLuint VAO;
      GLuint buffers[NUM_VBs];

      struct MeshEntry {
          MeshEntry()
          {
              numIndices    = 0;
              baseVertex    = 0;
              baseIndex     = 0;
              materialIndex = INVALID_MATERIAL;
          }
          
          unsigned int numIndices;
          unsigned int baseVertex;
          unsigned int baseIndex;
          unsigned int materialIndex;
      };
      
      std::vector<MeshEntry> entries;
      std::vector<Texture*> textures;
       
      std::map<std::string,uint> boneMapping; // maps a bone name to its index
      uint numBones;
      std::vector<BoneInfo> boneInfo;
      glm::mat4 globalInverseTransform;

      const aiScene* scene;
      Assimp::Importer importer;
  };
};
