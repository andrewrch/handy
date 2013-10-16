#include "handy_options.h"
#include <string>
#include <functional>

namespace handy
{
	HandyOptionsParser::HandyOptionsParser()
	{
	
	}

	po::options_description HandyOptionsParser::initialiseCommandLineArgs()
	{
		po::options_description desc("Command line options");
			// Declare the supported options.
			desc.add_options()
			// Some weird operator overloading/chaining going on here
			("help", "produce help message")
			("logfile", po::value<std::string>()->default_value("handy_logfile")
			 	->notifier(std::bind1st(std::mem_fun(&HandyOptions::setLogfile), &options)), 
			 "specify log file")
		;

		return desc;
	}

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
		catch (boost::program_options::unknown_option& e)
		{
			std::cerr << "Unknown option provided: " << e.what() << std::endl;
			carryOn = false;
		}
		catch (boost::program_options::invalid_command_line_syntax& e)
		{
			std::cerr << "Invalid option syntax: " << e.what() << std::endl;
			carryOn = false;
		}

		if (vm.count("help")) 
		{
			std::cout << desc;
			return false;
		}

		// Call notify to tidy everything up and store to the HandyOptions
		// structure
		po::notify(vm);

		return carryOn;
	}
}
