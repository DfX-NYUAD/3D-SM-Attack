
#include "IO.hpp"
#include "Data.hpp"

// parse program parameters and test files
void IO::parseParametersFiles(Data& data, int const& argc, char** argv) {

	// print command-line parameters
	if (argc < 7) {
		std::cout << "IO> Usage: " << argv[0] << " top.v bottom.v mappings.file cells.inputs cells.outputs out.v" << std::endl;
		std::cout << "IO> " << std::endl;
		std::cout << "IO> Mandatory parameter ``top.v'': Netlist for top tier" << std::endl;
		std::cout << "IO> Mandatory parameter ``bottom.v'': Netlist for bottom tier" << std::endl;
		std::cout << "IO> Mandatory parameter ``mappings.file'': File with the obfuscated mappings for the F2F interconnects" << std::endl;
		std::cout << "IO> Mandatory parameter ``cells.inputs'': All cells and all their inputs" << std::endl;
		std::cout << "IO> Mandatory parameter ``cells.outputs'': All cells and all their outputs" << std::endl;
		std::cout << "IO> Mandatory parameter ``out.v'': Recovered netlist" << std::endl;
		exit(1);
	}

	// read in mandatory parameters
	data.files.top_netlist = argv[1];
	data.files.bottom_netlist = argv[2];
	data.files.obfuscated_mappings = argv[3];
	data.files.cells_inputs = argv[4];
	data.files.cells_outputs = argv[5];
	data.files.out_netlist = argv[6];

	// test input files
	IO::testFile(data.files.top_netlist);
	IO::testFile(data.files.bottom_netlist);
	IO::testFile(data.files.obfuscated_mappings);
	IO::testFile(data.files.cells_inputs);
	IO::testFile(data.files.cells_outputs);
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

void IO::parseCells(Data& data, bool const& outputs) {
	std::ifstream in;
	std::string line;
	std::string tmpstr;
	std::string file;

	if (outputs) {
		file = data.files.cells_outputs;
	}
	else {
		file = data.files.cells_inputs;
	}
	in.open(file.c_str());

	std::cout << "IO> Parsing the cells and their ";
	if (outputs) {
		std::cout << "outputs ...";
	}
	else {
		std::cout << "inputs ...";
	}
	std::cout << std::endl;

	while (std::getline(in, line)) {

		Data::Cell cell;
		std::istringstream linestream(line);

		// first is cell type
		linestream >> cell.type;

		// then, any number of I/O pins can follow
		if (outputs) {
			while (!linestream.eof()) {

				linestream >> tmpstr;
				cell.outputs.emplace_back(tmpstr);
			}
		}
		else {
			while (!linestream.eof()) {

				linestream >> tmpstr;
				cell.inputs.emplace_back(tmpstr);
			}
		}

		// memorize parsed cell; either update existing one or insert new one
		//
		auto const iter = data.cells.find(cell.type);
		if (iter != data.cells.end()) {

			if (outputs) {
				iter->second.outputs = cell.outputs;
			}
			else {
				iter->second.inputs = cell.inputs;
			}
		}
		else {
			data.cells.insert(std::make_pair(
						cell.type,
						cell
					));
		}
	}

	// dbg log of parsed tuples
	//
	if (IO::DBG) {

		std::cout << "IO_DBG> Print all cells: " << std::endl;

		for (auto const& cell_iter : data.cells) {
			Data::Cell const& cell = cell_iter.second;

			std::cout << "IO_DBG>  " << cell.type;

			std::cout << " OUT = ( ";
			for (auto const& output : cell.outputs) {
				std::cout << output << " ";
			}
			std::cout << ")";
			std::cout << " IN = ( ";
			for (auto const& input : cell.inputs) {
				std::cout << input << " ";
			}
			std::cout << ")";

			std::cout << std::endl;
		}
	}

	std::cout << "IO> Done" << std::endl;
	std::cout << "IO>  Cells: " << data.cells.size() << std::endl;
	std::cout << "IO> " << std::endl;
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
			tmpstr = tmpstr.substr(0, tmpstr.find(";"));

			// differentiate between global inputs and F2F inputs; the latter have an "_" in the name
			// 
			// F2F input
			if (tmpstr.find("_") != std::string::npos) {
				netlist->inputs_F2F.insert(tmpstr);
			}
			// global input
			else {
				netlist->inputs_global.insert(tmpstr);
			}
		}
	}
	
	// reset file handler
	in.clear() ;
	in.seekg(0, in.beg);

	// 2) parse outputs, line by line
	//
	while (std::getline(in, line)) {

		// skip all the irrelevant lines
		if (!(line.find("output") != std::string::npos && line.find(";") != std::string::npos)) {
			continue;
		}
		// process all the relevant lines
		else {
			std::istringstream linestream(line);

			// drop "output";
			linestream >> tmpstr;

			// parse the output name, without the final ";"
			linestream >> tmpstr;
			tmpstr = tmpstr.substr(0, tmpstr.find(";"));

			// differentiate between global outputs and F2F outputs; the latter have an "_" in the name
			// 
			// F2F output
			if (tmpstr.find("_") != std::string::npos) {
				netlist->outputs_F2F.insert(tmpstr);
			}
			// global output
			else {
				netlist->outputs_global.insert(tmpstr);
			}
		}
	}
	
	// reset file handler
	in.clear() ;
	in.seekg(0, in.beg);

	// 3) parse wires, line by line
	//
	while (std::getline(in, line)) {

		// skip all the irrelevant lines
		if (!(line.find("wire") != std::string::npos && line.find(";") != std::string::npos)) {
			continue;
		}
		// process all the relevant lines
		else {
			std::istringstream linestream(line);

			// drop "wire";
			linestream >> tmpstr;

			// parse the wire name, without the final ";"
			linestream >> tmpstr;
			tmpstr = tmpstr.substr(0, tmpstr.find(";"));

			netlist->wires.insert(tmpstr);
		}
	}

	// finally, close file
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

		std::cout << "IO_DBG> Print all outputs_global: " << std::endl;

		for (auto const& output : netlist->outputs_global) {

			std::cout << "IO_DBG>  " << output;
			std::cout << std::endl;
		}

		std::cout << "IO_DBG> Print all outputs_F2F: " << std::endl;

		for (auto const& output : netlist->outputs_F2F) {

			std::cout << "IO_DBG>  " << output;
			std::cout << std::endl;
		}

		std::cout << "IO_DBG> Print all wires: " << std::endl;

		for (auto const& wire : netlist->wires) {

			std::cout << "IO_DBG>  " << wire;
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

