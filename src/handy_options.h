#pragma once

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <glm/glm.hpp>
#include <exception>
#include <string>
#include <iostream>
#include <vector>

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
        pose(27),
        display(true),
        cmdOptions(getCommandLineOnlyArgs()),
        cfgOptions(getConfigFileOnlyArgs()),
        genOptions(getGeneralArgs())
      { }

      bool parse(int argc, char* argv[]);
      void printHelp();
      bool showDisplay() { return display; }

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

      glm::vec3& getCameraPos() { return camPos; }
      glm::vec3& getCameraTarget() { return camTarget; }
      glm::vec3& getCameraUp() { return camUp; }

      std::vector<double>& getPoseVector() { return pose; }

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
      void setCamTargetX(float x) { camTarget.x = x; }
      void setCamTargetY(float y) { camTarget.y = y; }
      void setCamTargetZ(float z) { camTarget.y = z; }
      void setCamUpX(float x) { camUp.x = x; }
      void setCamUpY(float y) { camUp.y = y; }
      void setCamUpZ(float z) { camUp.z = z; }

      // Pose :(
      void setPose0(float p) { pose[0] = p; }
      void setPose1(float p) { pose[1] = p; }
      void setPose2(float p) { pose[2] = p; }
      void setPose3(float p) { pose[3] = p; }
      void setPose4(float p) { pose[4] = p; }
      void setPose5(float p) { pose[5] = p; }
      void setPose6(float p) { pose[6] = p; }
      void setPose7(float p) { pose[7] = p; }
      void setPose8(float p) { pose[8] = p; }
      void setPose9(float p) { pose[9] = p; }
      void setPose10(float p) { pose[10] = p; }
      void setPose11(float p) { pose[11] = p; }
      void setPose12(float p) { pose[12] = p; }
      void setPose13(float p) { pose[13] = p; }
      void setPose14(float p) { pose[14] = p; }
      void setPose15(float p) { pose[15] = p; }
      void setPose16(float p) { pose[16] = p; }
      void setPose17(float p) { pose[17] = p; }
      void setPose18(float p) { pose[18] = p; }
      void setPose19(float p) { pose[19] = p; }
      void setPose20(float p) { pose[20] = p; }
      void setPose21(float p) { pose[21] = p; }
      void setPose22(float p) { pose[22] = p; }
      void setPose23(float p) { pose[23] = p; }
      void setPose24(float p) { pose[24] = p; }
      void setPose25(float p) { pose[25] = p; }
      void setPose26(float p) { pose[26] = p; }

      void setDisplay(bool d) { display = d; }

      // Logfile
      std::string logfile;
      std::string configFile;
      std::string handFile;
      // Shaders
      std::string shaderDir, fragmentShader, vertexShader;
      // Camera properties
      glm::vec3 camPos, camTarget, camUp;
      // Pose
      std::vector<double> pose;
      // Render without viewer?
      bool display;

      po::options_description cmdOptions, cfgOptions, genOptions;
  };
};
