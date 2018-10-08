
#include "IO.hpp"
#include "Data.hpp"

// parse program parameters and test files
void IO::parseParametersFiles(Data& data, int const& argc, char** argv) {

	// print command-line parameters
	if (argc < 8) {
		std::cout << "IO> Usage: " << argv[0] << " top.v bottom.v wrapper.v mappings.file cells.inputs cells.outputs out.v [threads]" << std::endl;
		std::cout << "IO> " << std::endl;
		std::cout << "IO> Mandatory parameter ``top.v'': Netlist for top tier" << std::endl;
		std::cout << "IO> Mandatory parameter ``bottom.v'': Netlist for bottom tier" << std::endl;
		std::cout << "IO> Mandatory parameter ``wrapper.v'': Netlist for wrapping bottom and top tier -- only used for parsing of regular IO ports" << std::endl;
		std::cout << "IO> Mandatory parameter ``mappings.file'': File with the obfuscated mappings for the F2F interconnects" << std::endl;
		std::cout << "IO> Mandatory parameter ``cells.inputs'': All cells and all their inputs" << std::endl;
		std::cout << "IO> Mandatory parameter ``cells.outputs'': All cells and all their outputs" << std::endl;
		std::cout << "IO> Mandatory parameter ``out.v'': Recovered netlist" << std::endl;
		std::cout << "IO> Optional parameter ``threads'': threads for parallel attack" << std::endl;
		exit(1);
	}

	// read in mandatory parameters
	data.files.top_netlist = argv[1];
	data.files.bottom_netlist = argv[2];
	data.files.wrapper_netlist = argv[3];
	data.files.obfuscated_mappings = argv[4];
	data.files.cells_inputs = argv[5];
	data.files.cells_outputs = argv[6];
	data.files.out_netlist = argv[7];

	// read in optional arguments
	if (argc == 9) {
		data.threads = std::stoi(argv[8]);
	}

	// test input files
	IO::testFile(data.files.top_netlist);
	IO::testFile(data.files.bottom_netlist);
	IO::testFile(data.files.wrapper_netlist);
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

void IO::parseMappings(Data& data) {
	std::ifstream in;
	std::string line;
	std::string tmpstr;
	std::string output_tuple;
	std::string output;

	in.open(data.files.obfuscated_mappings.c_str());

	std::cout << "IO> Parsing the obfuscated F2F mappings ...";
	std::cout << std::endl;

	while (std::getline(in, line)) {

		//Data::Cell cell;
		std::istringstream linestream(line);

		// first is output
		linestream >> output_tuple;
		// name of output pin
		output = output_tuple.substr(output_tuple.find("/") + 1);

		// second is " : "; drop
		linestream >> tmpstr;

		// then, any number of input pins to map to can follow
		//
		// differentiate between bottom-to-top and top-to bottom mappings
		//
		// bottom-to-top
		if (output_tuple.find("bottom/") != std::string::npos) {

			while (!linestream.eof()) {

				linestream >> tmpstr;

				data.F2F.bottom_to_top.insert(std::make_pair(
							output,
							// name of input pin, without prefix
							tmpstr.substr(tmpstr.find("/") + 1)
						));
				// also memorize the inverted
				data.F2F.inverted_bottom_to_top.insert(std::make_pair(
							// name of input pin, without prefix
							tmpstr.substr(tmpstr.find("/") + 1),
							output
						));

				// also memorize all the output pins separately
				// note that also POs may be among them
				data.F2F.keys_bottom_to_top.insert(output);
			}
		}
		else {
			while (!linestream.eof()) {

				linestream >> tmpstr;

				data.F2F.top_to_bottom.insert(std::make_pair(
							output,
							// name of input pin, without prefix
							tmpstr.substr(tmpstr.find("/") + 1)
						));
				// also memorize the inverted
				data.F2F.inverted_top_to_bottom.insert(std::make_pair(
							// name of input pin, without prefix
							tmpstr.substr(tmpstr.find("/") + 1),
							output
						));

				// also memorize all the output pins separately
				// note that also POs may be among them
				data.F2F.keys_top_to_bottom.insert(output);
			}
		}
	}

	// dbg log of parsed tuples
	//
	if (IO::DBG) {

		std::cout << "IO_DBG> Print all mappings for bottom_to_top: " << std::endl;

		for (auto const& mapping : data.F2F.bottom_to_top) {

			std::cout << "IO_DBG>  \"" << mapping.first << "\" : \"" << mapping.second << "\"" << std::endl;
		}

		std::cout << "IO_DBG> Print all mappings for top_to_bottom: " << std::endl;

		for (auto const& mapping : data.F2F.top_to_bottom) {

			std::cout << "IO_DBG>  \"" << mapping.first << "\" : \"" << mapping.second << "\"" << std::endl;
		}
	}

	std::cout << "IO> Done" << std::endl;
	std::cout << "IO>  F2F outputs in bottom tier (there may be POs among them): " << data.F2F.keys_bottom_to_top.size() << std::endl;
	std::cout << "IO>   All the mapping candidates from bottom to top tier: " << data.F2F.bottom_to_top.size() << std::endl;
	std::cout << "IO>  F2F outputs in top tier (there may be POs among them): " << data.F2F.keys_top_to_bottom.size() << std::endl;
	std::cout << "IO>   All the mapping candidates from top to bottom tier: " << data.F2F.top_to_bottom.size() << std::endl;
	std::cout << "IO> " << std::endl;

	// sanity check
	if (data.F2F.bottom_to_top.empty() || data.F2F.top_to_bottom.empty()) {
		std::cout << "IO> No F2F mappings found; exiting ..." << std::endl;
		std::cout << "IO> Check your mappings file \"" << data.files.obfuscated_mappings << "\"" << std::endl;
		exit(1);
	}
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

			std::cout << "IO_DBG>  \"" << cell.type << "\"";

			std::cout << " OUT = ( ";
			for (auto const& output : cell.outputs) {
				std::cout << "\"" << output << "\" ";
			}
			std::cout << ")";
			std::cout << " IN = ( ";
			for (auto const& input : cell.inputs) {
				std::cout << "\"" << input << "\" ";
			}
			std::cout << ")";

			std::cout << std::endl;
		}
	}

	std::cout << "IO> Done" << std::endl;
	std::cout << "IO>  Cells: " << data.cells.size() << std::endl;
	std::cout << "IO> " << std::endl;

	// sanity check
	if (data.cells.empty()) {
		std::cout << "IO> No cells found; exiting ..." << std::endl;
		std::cout << "IO> Check your cells file \"" << file << "\"" << std::endl;
		exit(1);
	}
}

void IO::parseRegularPorts(Data& data) {
	std::ifstream in;
	std::string line;
	std::string tmpstr;

	in.open(data.files.wrapper_netlist.c_str());

	std::cout << "IO> Parsing the wrapper netlist for the regular ports, not the F2F ports ...";
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

			data.netlist.inputs.insert(tmpstr);
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

			data.netlist.outputs.insert(tmpstr);
		}
	}
	
	// close file
	in.close();

	// dbg log of parsed tuples
	//
	if (IO::DBG) {

		std::cout << "IO_DBG> Print all inputs: " << std::endl;

		for (auto const& input : data.netlist.inputs) {

			std::cout << "IO_DBG>  \"" << input << "\"";
			std::cout << std::endl;
		}

		std::cout << "IO_DBG> Print all outputs: " << std::endl;

		for (auto const& output : data.netlist.outputs) {

			std::cout << "IO_DBG>  \"" << output << "\"";
			std::cout << std::endl;
		}
	}

	std::cout << "IO> Done" << std::endl;
	std::cout << "IO>  Regular input ports: " << data.netlist.inputs.size() << std::endl;
	std::cout << "IO>  Regular output ports: " << data.netlist.outputs.size() << std::endl;
	std::cout << "IO> " << std::endl;

	// sanity check
	if (data.netlist.inputs.empty() || data.netlist.outputs.empty()) {
		std::cout << "IO> No inputs/outputs found; exiting ..." << std::endl;
		std::cout << "IO> Check your wrapper netlist \"" << data.files.wrapper_netlist << "\"" << std::endl;
		exit(1);
	}
};

void IO::parseNetlist(Data& data, bool const& top_tier) {
	std::ifstream in;
	std::string line;
	std::string tmpstr;
	std::string file;

	unsigned F2F_count = data.netlist.F2F.size();
	unsigned wires_count = data.netlist.wires.size();
	unsigned gates_count = data.netlist.gates.size();

	if (top_tier) {
		file = data.files.top_netlist;
	}
	else {
		file = data.files.bottom_netlist;
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

	// 1) parse F2F inputs, line by line
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

			// F2F ports are only those ports which are not memorized as regular ports yet
			//
			if (data.netlist.inputs.find(tmpstr) == data.netlist.inputs.end()) {
				data.netlist.F2F.insert(tmpstr);
			}
		}
	}
	
	// reset file handler
	in.clear() ;
	in.seekg(0, in.beg);

	// dbg log
	//
	if (IO::DBG) {

		std::cout << "IO_DBG> Print all F2F inputs: " << std::endl;

		for (auto const& port : data.netlist.F2F) {

			std::cout << "IO_DBG>  \"" << port << "\"";
			std::cout << std::endl;
		}
	}

	// 2) parse F2F outputs, line by line
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

			// F2F ports are only those ports which are not memorized as regular ports yet
			//
			if (data.netlist.outputs.find(tmpstr) == data.netlist.outputs.end()) {
				data.netlist.F2F.insert(tmpstr);
			}
		}
	}
	
	// reset file handler
	in.clear() ;
	in.seekg(0, in.beg);

	// dbg log
	//
	if (IO::DBG) {

		std::cout << "IO_DBG> Print all F2F inputs and outputs: " << std::endl;

		for (auto const& port : data.netlist.F2F) {

			std::cout << "IO_DBG>  \"" << port << "\"";
			std::cout << std::endl;
		}
	}

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

			data.netlist.wires.insert(tmpstr);
		}
	}

	// reset file handler
	in.clear() ;
	in.seekg(0, in.beg);

	// dbg log
	//
	if (IO::DBG) {

		std::cout << "IO_DBG> Print all wires: " << std::endl;

		for (auto const& wire : data.netlist.wires) {

			std::cout << "IO_DBG>  \"" << wire << "\"";
			std::cout << std::endl;
		}
	}

	// 4) parse gates, line by line
	//
	Data::Gate new_gate;
	Data::Cell *cell = nullptr;

	while (std::getline(in, line)) {

		// skip all the irrelevant lines
		if (!(line.find("),") != std::string::npos || line.find("));") != std::string::npos)) {
			continue;
		}
		// process all the relevant lines
		else {
			std::istringstream linestream(line);

			// differentiate the different lines
			//
			// 1st line has driving strength for cell
			if (line.find("_X") != std::string::npos) {

				// dbg
				//std::cout << "START: " << line << std::endl;

				// make that new_gate is a new instance
				new_gate = Data::Gate();

				// gate type
				linestream >> new_gate.type;
				// gate name
				linestream >> new_gate.name;

				// find related cell
				auto iter = data.cells.find(new_gate.type);
				if (iter != data.cells.end()) {
					cell = &(iter->second);
				}
				// report error otherwise
				else {
					std::cout << "IO>  Error: the gate \"" << new_gate.name << "\" is of type \"" << new_gate.type << "\"";
					std::cout << ", but this type is not covered in your cells.inputs and cells.outputs files ..." << std::endl;
					std::cout << "IO>  Exiting; check you library and cells.inputs/outputs files" << std::endl;
					exit(1);
				}
			}

			// all lines contains some output/input pin and its connectivity
			//
			// check all the output pins of the related cell
			for (auto const& pin : cell->outputs) {

				if (line.find("." + pin + "(") != std::string::npos) {

					// memorize the output pin and the net/pins it's connected to
					//
					size_t pos_begin = line.find_last_of("(") + 1;
					size_t pos_end = line.find_first_of(")");

					// remove heading/trailing whitespaces, if any, using stream operation
					std::string connected;
					std::istringstream connected_stream(line.substr(pos_begin, pos_end - pos_begin));
					connected_stream >> connected;

					new_gate.outputs.insert(std::make_pair(
								pin,
								connected
							));

					break;
				}
			}
			// check all the input pins of the related cell
			for (auto const& pin : cell->inputs) {

				if (line.find("." + pin + "(") != std::string::npos) {

					// memorize the input pin and the net/pins it's connected to
					//
					size_t pos_begin = line.find_last_of("(") + 1;
					size_t pos_end = line.find_first_of(")");

					// remove heading/trailing whitespaces, if any, using stream operation
					std::string connected;
					std::istringstream connected_stream(line.substr(pos_begin, pos_end - pos_begin));
					connected_stream >> connected;

					new_gate.inputs.insert(std::make_pair(
								pin,
								connected
							));

					break;
				}
			}

			// final line has "));"
			if (line.find("));") != std::string::npos) {

				// dbg
				//std::cout << "STOP: " << line << std::endl;

				// memorize the gate
				data.netlist.gates.emplace_back(new_gate);
			}
		}
	}

	// finally, close file
	in.close();

	// dbg log
	//
	if (IO::DBG) {

		std::cout << "IO_DBG> Print all gates: " << std::endl;

		for (Data::Gate const& gate : data.netlist.gates) {

			std::cout << "IO_DBG>  \"" << gate.type << "\" \"" << gate.name << "\"";

			std::cout << " OUT = ( ";
			for (auto const& output : gate.outputs) {
				std::cout << "\"" << output.first << "\": \"" << output.second << "\" ";
			}
			std::cout << ")";
			std::cout << " IN = ( ";
			for (auto const& input : gate.inputs) {
				std::cout << "\"" << input.first << "\": \"" << input.second << "\" ";
			}
			std::cout << ")";

			std::cout << std::endl;
		}
	}

	std::cout << "IO> Done" << std::endl;
	std::cout << "IO>  Total F2F ports (inputs/outputs): " << data.netlist.F2F.size() << std::endl;
	std::cout << "IO>  Total wires: " << data.netlist.wires.size() << std::endl;
	std::cout << "IO>  Total gates: " << data.netlist.gates.size() << std::endl;
	std::cout << "IO> Note that counts also contain the instances from the parsing of the other netlist, if done already" << std::endl;
	std::cout << "IO> " << std::endl;

	// sanity checks
	if (data.netlist.F2F.size() == F2F_count) {
		std::cout << "IO> No F2F mappings found; exiting ..." << std::endl;
		std::cout << "IO> Check your mappings file \"" << data.files.obfuscated_mappings << "\"" << std::endl;
		exit(1);
	}
	if (data.netlist.wires.size() == wires_count) {
		std::cout << "IO> No wires found; exiting ..." << std::endl;
		std::cout << "IO> Check your netlist file \"" << file << "\"" << std::endl;
		exit(1);
	}
	if (data.netlist.gates.size() == gates_count) {
		std::cout << "IO> No gates found; exiting ..." << std::endl;
		std::cout << "IO> Check your netlist file \"" << file << "\"" << std::endl;
		exit(1);
	}
};
