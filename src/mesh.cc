/*

	Copyright 2011 Etay Meiri

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <assert.h>
#include "boost/filesystem.hpp"
#include "mesh.h"

// Logging
#include <glog/logging.h>

namespace handy
{
  void Mesh::VertexBoneData::addBoneData(uint boneID, float weight)
  {
    uint idsSize = sizeof(ids)/sizeof(ids[0]);
    for (uint i = 0 ; i < idsSize ; i++) 
    {
      if (weights[i] == 0.0) 
      {
        ids[i]     = boneID;
        weights[i] = weight;
        return;
      }        
    }
    
    // should never get here - more bones than we have space for
    assert(0);
  }

  Mesh::Mesh() 
  { 
    VAO = 0;
    // Zero buffers
    memset(buffers, 0, sizeof(buffers));
    numBones = 0;
    scene = nullptr;
  }

  Mesh::~Mesh() 
  { 
    clear(); 
  }

  void Mesh::clear()
  {
    for (unsigned int i = 0 ; i < textures.size() ; i++) 
    {
      if(textures[i])
      {
        delete textures[i];
        textures[i] = nullptr;
      }
    }

    if (buffers[0] != 0) 
      glDeleteBuffers(NUM_VBs, buffers);
       
    if (VAO != 0) 
    {
      glDeleteVertexArrays(1, &VAO);
      VAO = 0;
    }
  }

  bool Mesh::loadMesh(const std::string& filename)
  {
    // Release the previously loaded mesh (if it exists)
    clear();
    
    // Create the VAO
    glGenVertexArrays(1, &VAO);   
    glBindVertexArray(VAO);
    
    // Create the buffers for the vertices attributes
    glGenBuffers(NUM_VBs, buffers);
    bool ret = false;

    scene = importer.ReadFile(filename,
      aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);
    
    if (scene) 
    {
      // Copy matrix to GLM style matrix
      memcpy(&globalInverseTransform, &scene->mRootNode->mTransformation, 
        16*sizeof(GLfloat));
      globalInverseTransform = glm::transpose(globalInverseTransform);
      globalInverseTransform = glm::inverse(globalInverseTransform);

      //glm::mat4 m = globalInverseTransform;
      //for (int i = 0; i < 4; i++)
      //  std::cout << m[i][0] << " " << m[i][1] << " " << m[i][2] << " " << m[i][3] << std::endl; 
      ret = initFromScene(filename);
    }
    else 
    {
      LOG(ERROR) << "Error parsing " << filename.c_str() << \
        " more details: " << importer.GetErrorString();
    }

    // Unbind the VAO to make sure it isn't modified
    glBindVertexArray(0);    
    return ret;
  }
  
  bool Mesh::initFromScene(const std::string& filename)
  {  
    entries.resize(scene->mNumMeshes);
    textures.resize(scene->mNumMaterials);

    LOG(INFO) << "Number of meshes: " << scene->mNumMeshes;
    LOG(INFO) << "Number of materials: " << scene->mNumMaterials;

    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoords;
    std::vector<VertexBoneData> bones;
    std::vector<uint> indices;
       
    uint numVertices = 0;
    uint numIndices = 0;
    
    // Count the number of vertices and indices
    for (uint i = 0 ; i < entries.size() ; i++) 
    {
      entries[i].materialIndex = scene->mMeshes[i]->mMaterialIndex;        
      entries[i].numIndices    = scene->mMeshes[i]->mNumFaces * 3;
      entries[i].baseVertex    = numVertices;
      entries[i].baseIndex     = numIndices;
      
      numVertices += scene->mMeshes[i]->mNumVertices;
      numIndices  += entries[i].numIndices;
    }
    
    // Reserve space in the vectors for the vertex attributes and indices
    positions.reserve(numVertices);
    normals.reserve(numVertices);
    texCoords.reserve(numVertices);
    bones.resize(numVertices);
    indices.reserve(numIndices);
        
    // Initialize the meshes in the scene one by one
    for (uint i = 0 ; i < entries.size() ; i++) {
        const aiMesh* mesh = scene->mMeshes[i];
        initMesh(i, mesh, positions, normals, texCoords, bones, indices);
    }

    if (!initMaterials(filename)) {
        return false;
    }

    // Generate and populate the buffers with vertex attributes and the indices
  	glBindBuffer(GL_ARRAY_BUFFER, buffers[POS_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions[0]) * positions.size(), 
        &positions[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(POSITION_LOCATION);
    glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);    

    glBindBuffer(GL_ARRAY_BUFFER, buffers[TEXCOORD_VB]);
	  glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords[0]) * texCoords.size(), 
	      &texCoords[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(TEX_COORD_LOCATION);
    glVertexAttribPointer(TEX_COORD_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, 0);

   	glBindBuffer(GL_ARRAY_BUFFER, buffers[NORMAL_VB]);
	  glBufferData(GL_ARRAY_BUFFER, sizeof(normals[0]) * normals.size(), 
	      &normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(NORMAL_LOCATION);
    glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

   	glBindBuffer(GL_ARRAY_BUFFER, buffers[BONE_VB]);
	  glBufferData(GL_ARRAY_BUFFER, sizeof(bones[0]) * bones.size(), 
	      &bones[0], GL_STATIC_DRAW);

    int i;
    for (i = 0; i < 4; i++)
    {
      glEnableVertexAttribArray(BONE_ID_LOCATION + i);
      glVertexAttribIPointer(BONE_ID_LOCATION + i, 4, GL_INT, 
          sizeof(VertexBoneData), (const GLvoid*)(sizeof(GLint) * i * 4));
    }

    for (int j = 0; j < 4; j++)
    {
      glEnableVertexAttribArray(BONE_WEIGHT_LOCATION + j);    
      glVertexAttribPointer(BONE_WEIGHT_LOCATION + j, 4, GL_FLOAT, GL_FALSE, 
         sizeof(VertexBoneData), (const GLvoid*)(sizeof(GLfloat) * i * 4));
      i++;
    }
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[INDEX_BUFFER]);
	  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), 
	      &indices[0], GL_STATIC_DRAW);

    return glGetError() == GL_NO_ERROR;
  }


  void Mesh::initMesh(uint meshIndex,
                      const aiMesh* mesh,
                      std::vector<glm::vec3>& positions,
                      std::vector<glm::vec3>& normals,
                      std::vector<glm::vec2>& texCoords,
                      std::vector<VertexBoneData>& bones,
                      std::vector<uint>& indices)
  {    
    const aiVector3D zero(0.0f, 0.0f, 0.0f);
    
    for (unsigned int i = 0 ; i < mesh->mNumVertices; i++) 
    {
      const aiVector3D* pos      = &(mesh->mVertices[i]);
      const aiVector3D* normal   = &(mesh->mNormals[i]);
      const aiVector3D* texCoord = mesh->HasTextureCoords(0) ? 
        &(mesh->mTextureCoords[0][i]) : &zero;

      positions.push_back(glm::vec3(pos->x, pos->y, pos->z));
      normals.push_back(glm::vec3(normal->x, normal->y, normal->z));
      texCoords.push_back(glm::vec2(texCoord->x, texCoord->y));        
    }
    
    loadBones(meshIndex, mesh, bones);
    
    // Populate the index buffer
    for (uint i = 0 ; i < mesh->mNumFaces ; i++) 
    {
      const aiFace& face = mesh->mFaces[i];
      assert(face.mNumIndices == 3);
      indices.push_back(face.mIndices[0]);
      indices.push_back(face.mIndices[1]);
      indices.push_back(face.mIndices[2]);
    }
  }


  void Mesh::loadBones(uint meshIndex, 
                       const aiMesh* mesh, 
                       std::vector<VertexBoneData>& bones)
  {
    for (uint i = 0 ; i < mesh->mNumBones ; i++) 
    {
      uint boneIndex = 0;        
      std::string boneName(mesh->mBones[i]->mName.data);
      
      if (boneMapping.find(boneName) == boneMapping.end()) 
      {
        // Allocate an index for a new bone
        boneIndex = numBones;
        numBones++;            
        BoneInfo bi;			
        boneInfo.push_back(bi);

        memcpy(&boneInfo[boneIndex].boneOffset, &mesh->mBones[i]->mOffsetMatrix,
            16*sizeof(GLfloat));
        // Transpose because glm is column major
        boneInfo[boneIndex].boneOffset = glm::transpose(
            boneInfo[boneIndex].boneOffset);
        boneMapping[boneName] = boneIndex;
      }
      else 
        boneIndex = boneMapping[boneName];
      
      for (uint j = 0 ; j < mesh->mBones[i]->mNumWeights ; j++) 
      {
        uint vertexID = entries[meshIndex].baseVertex + 
          mesh->mBones[i]->mWeights[j].mVertexId;
        float weight  = mesh->mBones[i]->mWeights[j].mWeight;                   
        bones[vertexID].addBoneData(boneIndex, weight);
      }
    }    
  }

  bool Mesh::initMaterials(const std::string& filename)
  {
    namespace fs = boost::filesystem;
    fs::path f(filename);
    fs::path dir = f.parent_path();

    bool ret = true;

    std::cout << filename << std::endl;

    // Initialize the materials
    for (unsigned int i = 0 ; i < scene->mNumMaterials; i++) 
    {
      const aiMaterial* material = scene->mMaterials[i];
      textures[i] = nullptr;

      if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0) 
      {
        aiString path;

        if (material->GetTexture(aiTextureType_DIFFUSE, 0, 
            &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) 
        {
          fs::path texPath(dir / path.data);
          textures[i] = new Texture(GL_TEXTURE_2D, texPath.string());

          if (!textures[i]->load()) 
          {
            LOG(ERROR) << "Error loading texture : " << texPath;
            delete textures[i];
            textures[i] = nullptr;
            ret = false;
          }
          else 
            LOG(INFO) << "Loaded texture: " << texPath;
        }
      }

      // Load a white texture in case the model does not include its own texture
      if (!textures[i]) 
      {
        fs::path texPath(dir / "white.png");
        textures[i] = new Texture(GL_TEXTURE_2D, texPath.string());
        LOG(INFO) << "Loaded texture: " << texPath;
        ret = textures[i]->load();
      }
    }

    return ret;
  }

  void Mesh::render()
  {
    glBindVertexArray(VAO);
    
    for (uint i = 0 ; i < entries.size() ; i++) 
    {
      const uint materialIndex = entries[i].materialIndex;

      assert(materialIndex < textures.size());
      
      if (textures[materialIndex]) 
        textures[materialIndex]->bind(GL_TEXTURE0);

		  glDrawElementsBaseVertex(GL_TRIANGLES, 
                               entries[i].numIndices, 
                               GL_UNSIGNED_INT, 
                               (void*)(sizeof(uint) * entries[i].baseIndex), 
                               entries[i].baseVertex);
    }
    // Make sure the VAO is not changed from the outside    
    glBindVertexArray(0);
  }
};
