//A simple class for handling GLSL shader compilation
//Author: Movania Muhammad Mobeen
#pragma once

#include <GL/glew.h>
#include <map>
#include <string>

namespace handy
{
  class Shader
  {
    public:
      Shader();
      ~Shader();      
      void loadFromString(GLenum whichShader, const std::string& source);
      void loadFromFile(GLenum whichShader, const std::string& filename);
      void createAndLinkProgram();
      void use();
      void unUse();
      void addAttribute(const std::string& attribute);
      GLuint addUniform(const std::string& uniform);
      GLuint getProgram() const;
      //An indexer that returns the location of the attribute/uniform
      GLuint operator[](const std::string& attribute);
      GLuint operator()(const std::string& uniform);
      //Program deletion
      void deleteProgram() 
      {
        glDeleteProgram(this->program);
        this->program=-1;
      }
    private:
      enum ShaderType {VERTEX_SHADER, FRAGMENT_SHADER, GEOMETRY_SHADER};
      GLuint program;
      int totalShaders;
      GLuint shaders[3];//0->vertexshader, 1->fragmentshader, 2->geometryshader
      std::map<std::string,GLuint> attributeList;
      std::map<std::string,GLuint> uniformLocationList;
  };      
};
