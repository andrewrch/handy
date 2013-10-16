#pragma once

#include <boost/program_options.hpp>
#include <exception>
#include <string>
#include <iostream>

namespace handy
{
  // This is probably bad practise but it's only in my
  // namespace!
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
        logfile("")
      { }

      void setLogfile(std::string l) { logfile = l; };
      std::string getLogfile() { return logfile; };
    private:
      std::string logfile;
  };

  class HandyOptionsParser
  {
    public:
      HandyOptionsParser() {};
      bool parse(int argc, char* argv[]);
      HandyOptions getHandyOptions() { return options; };

    private:
      po::options_description initialiseCommandLineArgs();
      HandyOptions options;
  };
};
