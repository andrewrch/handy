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

    if (vm.count("help")) 
    {
      std::cout << desc;
      carryOn = false;
    }

    // Call notify to tidy everything up and store to the HandyOptions
    // structure
    po::notify(vm);

    return carryOn;
  }
}
