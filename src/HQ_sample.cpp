#include <iostream>
#include <string>
#include <vector>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/program_options.hpp>

#include "fwd.h"

#include "HQ_XY.h"

int main(int argc, char* argv[])
{
	fs::path fpath=argv[1];

	po::options_description config_file_opts{};
 	config_file_opts.add_options()
    ("IC_file", po::value<fs::path>(), "input filename")
	("XY_file", po::value<fs::path>(), "output filename")
	("IC_Nx_max", po::value<int>(), "IC x N-max")
	("IC_Ny_max", po::value<int>(), "IC y N-max")
	("IC_dx", po::value<double>(), "IC dx")
	("IC_dy", po::value<double>(), "IC dy")
	("IC_tau0", po::value<double>(), "IC tau0")
	("N_sample", po::value<int>(), "Number of samples (by pair)");
	VarMap var_map{};
	fs::ifstream conf_file{fpath};
    po::store(po::parse_config_file(conf_file, config_file_opts), var_map);
	std::cout << "input: " << var_map["IC_file"].as<fs::path>() << std::endl;
	std::cout << "output: " << var_map["XY_file"].as<fs::path>() << std::endl;

	HQ_XY HQ_gen(var_map);
	HQ_gen.HQ_XY_sample();
	
	return 0;
}










