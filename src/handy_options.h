#pragma once

#include <boost/program_options.hpp>
#include <exception>
#include <string>
#include <iostream>

/*
 * Structure to hold program options
 */
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
  class HandyOptionsException : public std::runtime_error
  {
    public:
      HandyOptionsException(const std::string& msg) 
        : std::runtime_error(msg)
      { }
  };

  /*
   *
   * If anythign gets complicated in here I can use PO to attach 
   * notify events
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
      HandyOptionsParser();
      bool parse(int argc, char* argv[]);
      HandyOptions getHandyOptions() { return options; };

    private:
      po::options_description initialiseCommandLineArgs();
      HandyOptions options;
  };
};
