#include "handy_options.h"
#include <string>
#include <functional>

namespace handy
{
  po::options_description HandyOptionsParser::initialiseCommandLineArgs()
  {
    po::options_description desc("Command line options");
      // Declare the supported options.
    desc.add_options()
    // Some weird operator overloading/chaining going on here
      ("help", "produce help message")
      ("logfile", po::value<std::string>()->default_value("handy_logfile")
       ->notifier(std::bind1st(std::mem_fun(&HandyOptions::setLogfile), 
           &options)), "specify log file")
      ("config", po::value<std::string>()->default_value("handy.cfg")
       ->notifier(std::bind1st(std::mem_fun(&HandyOptions::setConfigFile), 
           &options)), "config file location")
    ;

    return desc;
  }

  po::options_description HandyOptionsParser::initialiseConfigFileArgs()
  {
    po::options_description desc("Configuration file options");
    desc.add_options()
      // First camera position
      ("camera_pos_x", po::value<float>()->default_value(0.0f)
       ->notifier(std::bind1st(std::mem_fun(&HandyOptions::setCamPosX),
         &options)), "Camera x position")
      ("camera_pos_y", po::value<float>()->default_value(10.0f)
       ->notifier(std::bind1st(std::mem_fun(&HandyOptions::setCamPosY),
         &options)), "Camera y position")
      ("camera_pos_z", po::value<float>()->default_value(0.0f)
       ->notifier(std::bind1st(std::mem_fun(&HandyOptions::setCamPosZ),
         &options)), "Camera z position")
      // Now the look at point
      ("camera_look_x", po::value<float>()->default_value(0.0f)
       ->notifier(std::bind1st(std::mem_fun(&HandyOptions::setCamLookX),
         &options)), "Camera x lookat")
      ("camera_look_y", po::value<float>()->default_value(0.0f)
       ->notifier(std::bind1st(std::mem_fun(&HandyOptions::setCamLookY),
         &options)), "Camera y lookat")
      ("camera_look_z", po::value<float>()->default_value(0.0f)
       ->notifier(std::bind1st(std::mem_fun(&HandyOptions::setCamLookZ),
         &options)), "Camera z lookat")
      // And the up direction
      ("camera_up_x", po::value<float>()->default_value(0.0f)
       ->notifier(std::bind1st(std::mem_fun(&HandyOptions::setCamUpX),
         &options)), "Camera x up direction")
      ("camera_up_y", po::value<float>()->default_value(1.0f)
       ->notifier(std::bind1st(std::mem_fun(&HandyOptions::setCamUpY),
         &options)), "Camera y up direction")
      ("camera_up_z", po::value<float>()->default_value(0.0f)
       ->notifier(std::bind1st(std::mem_fun(&HandyOptions::setCamUpZ),
         &options)), "Camera z up direction")
      // Shaders
      ("shader_dir", po::value<std::string>()->default_value("src/shaders")->
       notifier(std::bind1st(std::mem_fun(&HandyOptions::setShaderDir),
         &options)), "Directory containing shaders")
      ("fragment_shader", po::value<std::string>()->default_value("bones.glslf")->
       notifier(std::bind1st(std::mem_fun(&HandyOptions::setFragmentShader),
           &options)), "Fragment shader")
      ("vertex_shader", po::value<std::string>()->default_value("bones.glslv")->
       notifier(std::bind1st(std::mem_fun(&HandyOptions::setFragmentShader),
           &options)), "Vertex shader")
      // Hand model
      ("hand_model", po::value<std::string>()->default_value("hand.dae")->
       notifier(std::bind1st(std::mem_fun(&HandyOptions::setHandFile),
           &options)), "Location of the hand model file")
      // Pose
      // Tomorrow morning....
    ;

    return desc;
  }

  /**
   * Parses the command line arguments and extracts all required
   * information.
   *
   * @return boolean value.  Only ever false if the user has requested
   * help.
   */
  bool HandyOptionsParser::parse(int argc, char* argv[])
  {
    bool carryOn = true;
    po::variables_map vm;

    // Now try processing the args and config
    po::options_description desc;
    desc.add(initialiseCommandLineArgs());
    try
    {
      po::store(po::parse_command_line(argc, argv, desc), vm);
    }
    catch (boost::program_options::error& e)
    {
      std::cerr << "An error has occured when parsing program options: (";
      std::cerr << e.what() << ")" << std::endl;
      throw HandyOptionsParsingException("Parsing failed");
    }
    po::notify(vm);

    if (vm.count("help")) 
    {
      std::cout << desc;
      carryOn = false;
    }

    desc.add(initialiseConfigFileArgs());
    try
    {
      po::store(po::parse_config_file<char>(options.getConfigFile().data(), desc), vm);
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

    return carryOn;
  }
}
