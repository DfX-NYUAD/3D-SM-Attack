
#include "IO.hpp"
#include "Data.hpp"

// parse program parameters and test files
void IO::parseParametersFiles(Data& data, int const& argc, char** argv) {

	// print command-line parameters
	if (argc < 5) {
		std::cout << "IO> Usage: " << argv[0] << " top.v bottom.v mappings.file out.v" << std::endl;
		std::cout << "IO> " << std::endl;
		std::cout << "IO> Mandatory parameter ``top.v'': Netlist for top tier" << std::endl;
		std::cout << "IO> Mandatory parameter ``bottom.v'': Netlist for bottom tier" << std::endl;
		std::cout << "IO> Mandatory parameter ``mappings.file'': File with the obfuscated mappings for the F2F interconnects" << std::endl;
		std::cout << "IO> Mandatory parameter ``out.v'': Recovered netlist" << std::endl;
		exit(1);
	}

	// read in mandatory parameters
	data.files.top_netlist = argv[1];
	data.files.bottom_netlist = argv[2];
	data.files.obfuscated_mappings = argv[3];
	data.files.out_netlist = argv[4];

	// test input files
	IO::testFile(data.files.top_netlist);
	IO::testFile(data.files.bottom_netlist);
	IO::testFile(data.files.obfuscated_mappings);
};

void IO::testFile(std::string const& file) {
	std::ifstream in;

	in.open(file.c_str());
	if (!in.good()) {
		std::cout << "IO> ";
		std::cout << "No such file: " << file << std::endl;
		exit(1);
	}
	in.close();
}

void IO::parseNetlist(Data& data, bool const& top_tier) {
	std::ifstream in;
	std::string line;
	std::string tmpstr;
	std::string file;
	Data::Netlist* netlist;

	if (top_tier) {
		file = data.files.top_netlist;
		netlist = &data.netlists.top;
	}
	else {
		file = data.files.bottom_netlist;
		netlist = &data.netlists.bottom;
	}
	in.open(file.c_str());

	std::cout << "IO> Parsing the netlist of the ";
	if (top_tier) {
		std::cout << "top tier ...";
	}
	else {
		std::cout << "bottom tier ...";
	}
	std::cout << std::endl;

	// 1) parse inputs, line by line
	//
	while (std::getline(in, line)) {

		// skip all the irrelevant lines
		if (!(line.find("input") != std::string::npos && line.find(";") != std::string::npos)) {
			continue;
		}
		// process all the relevant lines
		else {
			std::istringstream linestream(line);

			// drop "input";
			linestream >> tmpstr;

			// parse the input name, without the final ";"
			linestream >> tmpstr;
			tmpstr = tmpstr.substr(0, tmpstr.find(";") - 1);

			// differentiate between global inputs and F2F inputs; the latter have an "_" in the name
			// 
			// F2F input
			if (tmpstr.find("_") != std::string::npos) {
				netlist->inputs_F2F.push_back(tmpstr);
			}
			// global input
			else {
				netlist->inputs_global.push_back(tmpstr);
			}
		}
	}

	in.close();

	// dbg log of parsed tuples
	//
	if (IO::DBG) {

		std::cout << "IO_DBG> Print all inputs_global: " << std::endl;

		for (auto const& input : netlist->inputs_global) {

			std::cout << "IO_DBG>  " << input;
			std::cout << std::endl;
		}

		std::cout << "IO_DBG> Print all inputs_F2F: " << std::endl;

		for (auto const& input : netlist->inputs_F2F) {

			std::cout << "IO_DBG>  " << input;
			std::cout << std::endl;
		}
	}

	std::cout << "IO> Done" << std::endl;
	std::cout << "IO>  Global inputs: " << netlist->inputs_global.size() << std::endl;
	std::cout << "IO>  Global outputs: " << netlist->outputs_global.size() << std::endl;
	std::cout << "IO>  F2F inputs: " << netlist->inputs_F2F.size() << std::endl;
	std::cout << "IO>  F2F outputs: " << netlist->outputs_F2F.size() << std::endl;
	std::cout << "IO>  Wires: " << netlist->wires.size() << std::endl;
	std::cout << "IO>  Gates: " << netlist->gates.size() << std::endl;
	std::cout << "IO> " << std::endl;
};

