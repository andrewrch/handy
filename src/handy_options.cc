#include "handy_options.h"
#include <string>
#include <functional>
// Logging
#include <glog/logging.h>

namespace handy
{
  void HandyOptions::printHelp()
  {
    po::options_description desc;
    desc.add(cmdOptions).add(cfgOptions).add(genOptions);
    std::cout << desc << std::endl;
  }

  po::options_description HandyOptions::getCommandLineOnlyArgs()
  {
    po::options_description desc("Command line only options");
      // Declare the supported options.
    desc.add_options()
    // Some weird operator overloading/chaining going on here
      ("help", "produce help message")
      ("config", po::value<std::string>()->default_value("handy.cfg")
       ->notifier(std::bind1st(std::mem_fun(&HandyOptions::setConfigFile), 
           this)), "config file location")
    ;

    return desc;
  }

  po::options_description HandyOptions::getGeneralArgs()
  {
    po::options_description desc("General options");
    desc.add_options()
      ("logfile", po::value<std::string>()->default_value("handy_logfile")
       ->notifier(std::bind1st(std::mem_fun(&HandyOptions::setLogfile), 
           this)), "specify log file")
      // First camera position
      ("camera-pos-x", po::value<float>()->default_value(10.0f)
       ->notifier(std::bind1st(std::mem_fun(&HandyOptions::setCamPosX),
         this)), "Camera x position")
      ("camera-pos-y", po::value<float>()->default_value(0.0f)
       ->notifier(std::bind1st(std::mem_fun(&HandyOptions::setCamPosY),
         this)), "Camera y position")
      ("camera-pos-z", po::value<float>()->default_value(0.0f)
       ->notifier(std::bind1st(std::mem_fun(&HandyOptions::setCamPosZ),
         this)), "Camera z position")
      // Now the look at point
      ("camera-look-x", po::value<float>()->default_value(0.0f)
       ->notifier(std::bind1st(std::mem_fun(&HandyOptions::setCamLookX),
         this)), "Camera x lookat")
      ("camera-look-y", po::value<float>()->default_value(0.0f)
       ->notifier(std::bind1st(std::mem_fun(&HandyOptions::setCamLookY),
         this)), "Camera y lookat")
      ("camera-look-z", po::value<float>()->default_value(0.0f)
       ->notifier(std::bind1st(std::mem_fun(&HandyOptions::setCamLookZ),
         this)), "Camera z lookat")
      // And the up direction
      ("camera-up-x", po::value<float>()->default_value(0.0f)
       ->notifier(std::bind1st(std::mem_fun(&HandyOptions::setCamUpX),
         this)), "Camera x up direction")
      ("camera-up-y", po::value<float>()->default_value(1.0f)
       ->notifier(std::bind1st(std::mem_fun(&HandyOptions::setCamUpY),
         this)), "Camera y up direction")
      ("camera-up-z", po::value<float>()->default_value(0.0f)
       ->notifier(std::bind1st(std::mem_fun(&HandyOptions::setCamUpZ),
         this)), "Camera z up direction")
      // Shaders
      ("shader-dir", po::value<std::string>()->default_value("src/shaders")->
       notifier(std::bind1st(std::mem_fun(&HandyOptions::setShaderDir),
         this)), "Directory containing shaders")
      ("fragment-shader", po::value<std::string>()->default_value("bones.glslf")->
       notifier(std::bind1st(std::mem_fun(&HandyOptions::setFragmentShader),
           this)), "Fragment shader")
      ("vertex-shader", po::value<std::string>()->default_value("bones.glslv")->
       notifier(std::bind1st(std::mem_fun(&HandyOptions::setVertexShader),
           this)), "Vertex shader")
      // Hand model
      ("hand-model", po::value<std::string>()->default_value("hand.dae")->
       notifier(std::bind1st(std::mem_fun(&HandyOptions::setHandFile),
           this)), "Location of the hand model file")
    ;
      // Pose
      // Tomorrow morning....
    return desc;

  }

  po::options_description HandyOptions::getConfigFileOnlyArgs()
  {
    po::options_description desc("Configuration file only options");
    desc.add_options()
    ;

    return desc;
  }

  /**
   * Parses the command line arguments and extracts all required
   * information.
   *
   * @return boolean value. This is true if the user has requested the help
   * message
   */
  bool HandyOptions::parse(int argc, char* argv[])
  {
    po::variables_map vm;
    // Now try processing the args and config
    po::options_description cmdDesc;
    cmdDesc.add(cmdOptions).add(genOptions);
    try
    {
      po::store(po::parse_command_line(argc, argv, cmdDesc), vm);
    }
    catch (boost::program_options::error& e)
    {
      std::cerr << "An error has occured when parsing program options: (";
      std::cerr << e.what() << ")" << std::endl;
      throw HandyOptionsParsingException("Parsing failed");
    }
    // Call this so that command line args have higher priority than 
    // config file args (vm is only updated by first attemp to update value)
    po::notify(vm);

    po::options_description cfgDesc;
    cfgDesc.add(cfgOptions).add(genOptions);
    try
    {
      po::store(po::parse_config_file<char>(configFile.data(), cfgDesc), vm);
    }
    catch (boost::program_options::error& e)
    {
      std::cerr << "An error has occured when parsing program options: (";
      std::cerr << e.what() << ")" << std::endl;
      throw HandyOptionsParsingException("Parsing failed");
    }

    // Call notify to tidy everything up and store to the HandyOptions
    // structure
    po::notify(vm);

    /* WARNING!!!!!
     * Multiple returns.  This ONLY happens if help is required from command 
     * line */
    if (vm.count("help")) 
      return true;
    return false;
  }
}
