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
    
  Mesh::MeshEntry::MeshEntry()
  {
    VB = INVALID_OGL_VALUE;
    IB = INVALID_OGL_VALUE;
    numIndices  = 0;
    materialIndex = INVALID_MATERIAL;
  };

  Mesh::MeshEntry::~MeshEntry()
  {
    if (VB != INVALID_OGL_VALUE)
    {
      glDeleteBuffers(1, &VB);
    }

    if (IB != INVALID_OGL_VALUE)
    {
      glDeleteBuffers(1, &IB);
    }
  }

  void Mesh::MeshEntry::init(const std::vector<Vertex>& vertices,
                             const std::vector<unsigned int>& indices)
  {
    numIndices = indices.size();

    // Vertex buffer
    glGenBuffers(1, &VB);
    glBindBuffer(GL_ARRAY_BUFFER, VB);
    glBufferData(GL_ARRAY_BUFFER, 
                 sizeof(Vertex) * vertices.size(), 
                 &vertices[0], 
                 GL_STATIC_DRAW);

    // Index buffer
    glGenBuffers(1, &IB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
                 sizeof(unsigned int) * numIndices, 
                 &indices[0], 
                 GL_STATIC_DRAW);
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
  }

  bool Mesh::loadMesh(const std::string& filename)
  {
    // Release the previously loaded mesh (if it exists)
    clear();
    
    bool ret = false;
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filename, //aiProcessPreset_TargetRealtime_MaxQuality);
      aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);
    
    if (scene) 
      ret = initFromScene(scene, filename);
    else 
    {
      LOG(ERROR) << "Error parsing " << filename.c_str() << \
        " more details: " << importer.GetErrorString();
    }

    return ret;
  }

  bool Mesh::initFromScene(const aiScene* scene, const std::string& filename)
  {  
    entries.resize(scene->mNumMeshes);
    textures.resize(scene->mNumMaterials);

    LOG(INFO) << "Number of meshes: " << scene->mNumMeshes;
    LOG(INFO) << "Number of materials: " << scene->mNumMaterials;

    // Initialize the meshes in the scene one by one
    for (unsigned int i = 0 ; i < entries.size() ; i++) 
    {
      const aiMesh* mesh = scene->mMeshes[i];
      LOG(INFO) << "Initialising mesh: " << mesh->mName.data;
      initMesh(i, mesh);
    }

    return initMaterials(scene, filename);
  }

  void Mesh::initMesh(unsigned int meshIndex, const aiMesh* mesh)
  {
    entries[meshIndex].materialIndex = mesh->mMaterialIndex;

    LOG(INFO) << "Mesh " << meshIndex << " is material: " << mesh->mMaterialIndex;
    
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    const aiVector3D zero(0.0f, 0.0f, 0.0f);

    LOG(INFO) << mesh->mNumVertices;
    google::FlushLogFiles(google::INFO);

    for (unsigned int i = 0 ; i < mesh->mNumVertices; i++) 
    {
      const aiVector3D* pos      = &(mesh->mVertices[i]);
      const aiVector3D* normal   = &(mesh->mNormals[i]);
      const aiVector3D* texCoord = mesh->HasTextureCoords(0) ? 
        &(mesh->mTextureCoords[0][i]) : &zero;

      // Construct a Vertex
      Vertex v(glm::vec3(pos->x, pos->y, pos->z),
               glm::vec2(texCoord->x, texCoord->y),
               glm::vec3(normal->x, normal->y, normal->z));

      vertices.push_back(v);
    }

    for (unsigned int i = 0 ; i < mesh->mNumFaces ; i++) {
        const aiFace& face = mesh->mFaces[i];
        assert(face.mNumIndices == 3);
        indices.push_back(face.mIndices[0]);
        indices.push_back(face.mIndices[1]);
        indices.push_back(face.mIndices[2]);
    }

    entries[meshIndex].init(vertices, indices);
  }

  bool Mesh::initMaterials(const aiScene* scene, const std::string& filename)
  {
    namespace fs = boost::filesystem;
    fs::path f(filename);
    fs::path dir = f.parent_path();

    bool ret = true;

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
          fs::path fullPath(dir / path.data);
          textures[i] = new Texture(GL_TEXTURE_2D, fullPath.string());

          if (!textures[i]->load()) 
          {
            LOG(ERROR) << "Error loading texture : " << fullPath;
            delete textures[i];
            textures[i] = nullptr;
            ret = false;
          }
          else 
            LOG(INFO) << "Loaded texture: " << fullPath;
        }
      }

      // Load a white texture in case the model does not include its own texture
      if (!textures[i]) 
      {
        textures[i] = new Texture(GL_TEXTURE_2D, "./white.png");
        LOG(INFO) << "Loaded texture: ./white.png";
        ret = textures[i]->load();
      }
    }

    return ret;
  }

  void Mesh::render()
  {
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    for (unsigned int i = 0 ; i < entries.size() ; i++) 
    {
      glBindBuffer(GL_ARRAY_BUFFER, entries[i].VB);

      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
      glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20);

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, entries[i].IB);

      const unsigned int materialIndex = entries[i].materialIndex;
      if (materialIndex < textures.size() && textures[materialIndex]) 
        textures[materialIndex]->bind(GL_TEXTURE0);

      glDrawElements(GL_TRIANGLES, entries[i].numIndices, GL_UNSIGNED_INT, 0);
    }

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
  }
};
