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
      ("camera-target-x", po::value<float>()->default_value(0.0f)
       ->notifier(std::bind1st(std::mem_fun(&HandyOptions::setCamTargetX),
         this)), "Camera x target")
      ("camera-target-y", po::value<float>()->default_value(0.0f)
       ->notifier(std::bind1st(std::mem_fun(&HandyOptions::setCamTargetY),
         this)), "Camera y target")
      ("camera-target-z", po::value<float>()->default_value(0.0f)
       ->notifier(std::bind1st(std::mem_fun(&HandyOptions::setCamTargetZ),
         this)), "Camera z target")
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
      // Pose
      ("pose0", po::value<double>()->default_value(0)->
       notifier(std::bind1st(std::mem_fun(&HandyOptions::setPose0),
           this)), "Set 1st element of pose vector")
      ("pose1", po::value<double>()->default_value(0)->
       notifier(std::bind1st(std::mem_fun(&HandyOptions::setPose1),
           this)), "Set 2nd element of pose vector")
      ("pose2", po::value<double>()->default_value(0)->
       notifier(std::bind1st(std::mem_fun(&HandyOptions::setPose2),
           this)), "Set 3rd element of pose vector")
      ("pose3", po::value<double>()->default_value(0)->
       notifier(std::bind1st(std::mem_fun(&HandyOptions::setPose3),
           this)), "Set 4th element of pose vector")
      ("pose4", po::value<double>()->default_value(0)->
       notifier(std::bind1st(std::mem_fun(&HandyOptions::setPose4),
           this)), "Set 5th element of pose vector")
      ("pose5", po::value<double>()->default_value(0)->
       notifier(std::bind1st(std::mem_fun(&HandyOptions::setPose5),
           this)), "Set 6th element of pose vector")
      ("pose6", po::value<double>()->default_value(0)->
       notifier(std::bind1st(std::mem_fun(&HandyOptions::setPose6),
           this)), "Set 7th element of pose vector")
      ("pose7", po::value<double>()->default_value(0)->
       notifier(std::bind1st(std::mem_fun(&HandyOptions::setPose7),
           this)), "Set 8th element of pose vector")
      ("pose8", po::value<double>()->default_value(0)->
       notifier(std::bind1st(std::mem_fun(&HandyOptions::setPose8),
           this)), "Set 9th element of pose vector")
      ("pose9", po::value<double>()->default_value(0)->
       notifier(std::bind1st(std::mem_fun(&HandyOptions::setPose9),
           this)), "Set 10th element of pose vector")
      ("pose10", po::value<double>()->default_value(0)->
       notifier(std::bind1st(std::mem_fun(&HandyOptions::setPose10),
           this)), "Set 11th element of pose vector")
      ("pose11", po::value<double>()->default_value(0)->
       notifier(std::bind1st(std::mem_fun(&HandyOptions::setPose11),
           this)), "Set 12th element of pose vector")
      ("pose12", po::value<double>()->default_value(0)->
       notifier(std::bind1st(std::mem_fun(&HandyOptions::setPose12),
           this)), "Set 13th element of pose vector")
      ("pose13", po::value<double>()->default_value(0)->
       notifier(std::bind1st(std::mem_fun(&HandyOptions::setPose13),
           this)), "Set 14th element of pose vector")
      ("pose14", po::value<double>()->default_value(0)->
       notifier(std::bind1st(std::mem_fun(&HandyOptions::setPose14),
           this)), "Set 15th element of pose vector")
      ("pose15", po::value<double>()->default_value(0)->
       notifier(std::bind1st(std::mem_fun(&HandyOptions::setPose15),
           this)), "Set 16th element of pose vector")
      ("pose16", po::value<double>()->default_value(0)->
       notifier(std::bind1st(std::mem_fun(&HandyOptions::setPose16),
           this)), "Set 17th element of pose vector")
      ("pose17", po::value<double>()->default_value(0)->
       notifier(std::bind1st(std::mem_fun(&HandyOptions::setPose17),
           this)), "Set 18th element of pose vector")
      ("pose18", po::value<double>()->default_value(0)->
       notifier(std::bind1st(std::mem_fun(&HandyOptions::setPose18),
           this)), "Set 19th element of pose vector")
      ("pose19", po::value<double>()->default_value(0)->
       notifier(std::bind1st(std::mem_fun(&HandyOptions::setPose19),
           this)), "Set 20th element of pose vector")
      ("pose20", po::value<double>()->default_value(0)->
       notifier(std::bind1st(std::mem_fun(&HandyOptions::setPose20),
           this)), "Set 21st element of pose vector")
      ("pose21", po::value<double>()->default_value(0)->
       notifier(std::bind1st(std::mem_fun(&HandyOptions::setPose21),
           this)), "Set 22nd element of pose vector")
      ("pose22", po::value<double>()->default_value(0)->
       notifier(std::bind1st(std::mem_fun(&HandyOptions::setPose22),
           this)), "Set 23rd element of pose vector")
      ("pose23", po::value<double>()->default_value(0)->
       notifier(std::bind1st(std::mem_fun(&HandyOptions::setPose23),
           this)), "Set 24th element of pose vector")
      ("pose24", po::value<double>()->default_value(0)->
       notifier(std::bind1st(std::mem_fun(&HandyOptions::setPose24),
           this)), "Set 25th element of pose vector")
      ("pose25", po::value<double>()->default_value(0)->
       notifier(std::bind1st(std::mem_fun(&HandyOptions::setPose25),
           this)), "Set 26th element of pose vector")
      ("pose26", po::value<double>()->default_value(0)->
       notifier(std::bind1st(std::mem_fun(&HandyOptions::setPose26),
           this)), "Set 27th element of pose vector")
    ;
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
