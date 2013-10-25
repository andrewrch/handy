#pragma once

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <glm/glm.hpp>
#include <exception>
#include <string>
#include <iostream>

namespace handy
{
  // This is probably bad practise but at least it's only in handy ns
  namespace po = boost::program_options;

  /**
   * HandyOptionsException class.
   *
   * We throw this to let the main method know there has been a
   * problem when checking the command line or config file options
   */
  class HandyOptionsParsingException : public std::runtime_error
  {
    public:
      HandyOptionsParsingException(const std::string& msg) 
        : std::runtime_error(msg)
      { }
  };
  class HandyOptionsRequiredArgException : public po::required_option
  {
    public:
      HandyOptionsRequiredArgException(const std::string& msg) 
        : po::required_option(msg)
      { }
  };


  /*
   * This is really just a data structure to hold program options to 
   * pass to the main application class.
   *
   * I've added get/setters so that I can use boost::program_option's notify
   * method.
   */
  class HandyOptions 
  {
    public:
      HandyOptions() :
        logfile(""),
        configFile(""),
        handFile(""),
        shaderDir(""),
        fragmentShader(""),
        vertexShader(""),
        camPos(0.0f),
        camTarget(0.0f),
        camUp(0.0f),
        cmdOptions(getCommandLineOnlyArgs()),
        cfgOptions(getConfigFileOnlyArgs()),
        genOptions(getGeneralArgs())
      { }

      bool parse(int argc, char* argv[]);
      void printHelp();

      // Make these a bit tidier/move them to .cc
      std::string& getLogfile() { return logfile; }
      std::string& getConfigFile() { return configFile; }
      std::string& getHandFile() { return handFile; }

      std::string getFragmentShader() 
      { 
        return (boost::filesystem::path(shaderDir) / fragmentShader).string(); 
      }
      std::string getVertexShader()
      {
        return (boost::filesystem::path(shaderDir) / vertexShader).string(); 
      }

      glm::vec3 getCameraPos() { return camPos; }
      glm::vec3 getCameraTarget() { return camTarget; }
      glm::vec3 getCameraUp() { return camUp; }

    private:
      po::options_description getCommandLineOnlyArgs();
      po::options_description getConfigFileOnlyArgs();
      po::options_description getGeneralArgs();
      // private setters
      void setLogfile(const std::string l) { logfile = l; }
      void setConfigFile(const std::string c) { configFile = c; }
      void setHandFile(const std::string h) { handFile = h; }
      void setShaderDir(const std::string s) { shaderDir = s; }
      void setFragmentShader(const std::string f) { fragmentShader = f; }
      void setVertexShader(const std::string v) { vertexShader = v; }
      void setCamPosX(float x) { camPos.x = x; }
      void setCamPosY(float y) { camPos.y = y; }
      void setCamPosZ(float z) { camPos.z = z; }
      void setCamLookX(float x) { camTarget.x = x; }
      void setCamLookY(float y) { camTarget.y = y; }
      void setCamLookZ(float z) { camTarget.y = z; }
      void setCamUpX(float x) { camUp.x = x; }
      void setCamUpY(float y) { camUp.y = y; }
      void setCamUpZ(float z) { camUp.z = z; }

      // Logfile
      std::string logfile;
      std::string configFile;
      std::string handFile;
      // Shaders
      std::string shaderDir, fragmentShader, vertexShader;
      // Camera properties
      glm::vec3 camPos, camTarget, camUp;
      po::options_description cmdOptions, cfgOptions, genOptions;
  };
};
