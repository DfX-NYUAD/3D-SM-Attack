
#include "IO.hpp"
#include "Attack.hpp"
#include "Data.hpp"

int main (int argc, char** argv) {
	Data data;
	bool success;
	unsigned trials;
	std::vector<std::thread> threads;
	std::mutex m;

	std::cout << std::endl;
	std::cout << "Proximity attack on F2F interconnects camouflaging" << std::endl;
	std::cout << "--------------------------------------------------" << std::endl << std::endl;

	// parse program parameters and test files
	IO::parseParametersFiles(data, argc, argv);

	// parse cell inputs
	IO::parseCells(data, false);
	// parse cell outputs
	IO::parseCells(data, true);

	// parse bottom tier netlist
	IO::parseNetlist(data, false);
	// parse top tier netlist
	IO::parseNetlist(data, true);

	// parse the F2F mappings
	IO::parseMappings(data);

	// init random generator
	std::srand(std::time(nullptr));

	// prepare attack runs
	threads.reserve(data.threads);
	success = false;
	trials = 0;

	// run attack in parallel threads
	while (!success) {

		// init threads
		for (unsigned t = 1; t <= data.threads; t++) {
			threads.emplace_back( std::thread(Attack::trial, std::ref(data), std::ref(success), std::ref(trials), std::ref(m)) );
		}
		// join threads; the main thread execution will pause until all threads are done
		for (std::thread& t : threads) {
			t.join();
		}
		// clean up all threads
		threads.clear();

		if (!success) {
			std::cout << "Attack> Trials so far: " << trials << std::endl;
		}
	}
};

void Attack::rewriteConnectivity(std::pair<std::string, std::string> const& a, Data::Gate& gate, Data& data) {

	//if (Attack::DBG) {
	//	std::cout << "DBG> Mapping: " << a.first << " -> " << a. second << std::endl;
	//	std::cout << "DBG> Gate: " << gate.name << std::endl;
	//}

	// first is pin name, second is net/pin name
	for (auto& output : gate.outputs) {

		// the gate's output matches with the assignment driver or the assignment sink
		if (output.second == a.first || output.second == a.second) {

			if (Attack::DBG) {
				std::cout << "DBG>  Affected gate: " << gate.name;
				std::cout << "; mapping: " << a.first << " -> " << a. second;
				std::cout << "; affected output: " << output.first << "(" << output.second << ")" << std::endl;
			}

			// in case the assignment driver is a PO, rename the gate output towards that PO
			if (a.first.find("_") == std::string::npos) {
				output.second = a.first;
			}
			// regular case, no PO; rename gate output towards new wire
			else {
				output.second = a.first + "_" + a.second;
			}

			// also memorize that new wire
			data.netlist.wires.insert(output.second);

			if (Attack::DBG) {
				std::cout << "DBG>   New output: " << output.second << std::endl;
			}
		}
	}

	// first is pin name, second is net/pin name
	for (auto& input : gate.inputs) {

		// the gate's input matches with the assignment driver or the assignment sink
		if (input.second == a.first || input.second == a.second) {

			if (Attack::DBG) {
				std::cout << "DBG>  Affected gate: " << gate.name;
				std::cout << "; mapping: " << a.first << " -> " << a. second;
				std::cout << "; affected input: " << input.first << "(" << input.second << ")" << std::endl;
			}

			// in case the assignment driver is a PO, rename the gate input towards that PO
			if (a.first.find("_") == std::string::npos) {
				input.second = a.first;
			}
			// regular case, no PO; rename gate input towards new wire
			else {
				input.second = a.first + "_" + a.second;
			}

			// also memorize that new wire
			data.netlist.wires.insert(input.second);

			if (Attack::DBG) {
				std::cout << "DBG>   New input: " << input.second << std::endl;
			}
		}
	}
}

void Attack::evaluateAndOutput(Data::AssignmentF2F const& assignment, Data& data) {
	std::ofstream out;
	unsigned total_connections;
	unsigned correct_connections;

	std::cout << "Attack>" << std::endl;
	std::cout << "Attack> Success! Found F2F assignment without cycles" << std::endl;
	std::cout << "Attack>" << std::endl;
	std::cout << "Attack> Writing out netlist ..." << std::endl;

	out.open(data.files.out_netlist.c_str());

	// 1) rewrite the gates' connectivity, based on F2F mappings
	//
	// also add new wires to model those F2F mappings
	//
	if (Attack::DBG) {
		std::cout << "DBG> Rewriting gates' connectivity, adding wires to model F2F mappings ... " << std::endl;
		std::cout << "DBG>" << std::endl;
	}
	for (auto& gate_iter: data.netlist.gates) {
		auto& gate = gate_iter.second;

		// check all the mappings, revise gate connectivity if it's covered by one assignment
		//
		// note that some inputs as well as some outputs can be affected by that mapping, inputs namely when the output is reused in
		// the bottom tier for another gate's input
		//
		for (auto const& a : assignment.bottom_to_top) {

			Attack::rewriteConnectivity(a, gate, data);
		}
		for (auto const& a : assignment.top_to_bottom) {

			Attack::rewriteConnectivity(a, gate, data);
		}
	}

	// 2) write out netlist
	//
	out << "// Netlist recovered by proximity attack on obfuscated F2F mappings" << std::endl;
	out << "//" << std::endl;
	out << std::endl;
	out << "module recovered(";

	// count all the global outputs
	unsigned output_count = 0;
	for (auto const& output : data.netlist.outputs) {

		// ignore F2F outputs, those with "_"
		if (output.find("_") == std::string::npos) {
			output_count++;
		}
	}
	// count all the global inputs
	unsigned input_count = 0;
	for (auto const& input : data.netlist.inputs) {

		// ignore F2F inputs, those with "_"
		if (input.find("_") == std::string::npos) {
			input_count++;
		}
	}

	// output all inputs for module ports
	for (auto const& input : data.netlist.inputs) {

		// ignore F2F inputs, those with "_"
		if (input.find("_") == std::string::npos) {
			out << input << ", ";
		}
	}
	// output all outputs for module ports
	unsigned count = output_count;
	for (auto const& output : data.netlist.outputs) {

		// ignore F2F outputs, those with "_"
		if (output.find("_") == std::string::npos) {

			// the last output has no comma following, and closes the port list
			if (count == 1) {
				out << output << ");" << std::endl;
				out << std::endl;
			}
			else {
				out << output << ", ";
				count--;
			}
		}
	}

	// output all inputs for input statement
	count = input_count;
	out << "input ";
	for (auto const& input : data.netlist.inputs) {

		// ignore F2F inputs, those with "_"
		if (input.find("_") == std::string::npos) {

			// the last input has no comma following, and closes the statement
			if (count == 1) {
				out << input << ";" << std::endl;
				out << std::endl;
			}
			else {
				out << input << ", ";
				count--;
			}
		}
	}
	// output all outputs for output statement
	count = output_count;
	out << "output ";
	for (auto const& output : data.netlist.outputs) {

		// ignore F2F outputs, those with "_"
		if (output.find("_") == std::string::npos) {

			// the last output has no comma following, and closes the statement
			if (count == 1) {
				out << output << ";" << std::endl;
				out << std::endl;
			}
			else {
				out << output << ", ";
				count--;
			}
		}
	}

	// output all wires for wire statement
	count = data.netlist.wires.size();
	out << "wire ";
	for (auto const& wire : data.netlist.wires) {

		// the last wire has no comma following, and closes the statement
		if (count == 1) {
			out << wire << ";" << std::endl;
			out << std::endl;
		}
		else {
			out << wire << ", ";
			count--;
		}
	}

	// output all gates 
	for (auto const& gate_iter: data.netlist.gates) {
		Data::Gate const& gate = gate_iter.second;

		out << gate.type << " ";
		out << gate.name << " ";

		out << "(";

		for (auto const& input : gate.inputs) {
			out << "." << input.first << "(" << input.second << "), ";
		}

		unsigned count = gate.outputs.size();
		for (auto const& output : gate.outputs) {

			if (count == 1) {
				out << "." << output.first << "(" << output.second << ")";
			}
			else {
				out << "." << output.first << "(" << output.second << "), ";
			}
		}

		out << ");" << std::endl;
	}

	out << std::endl;
	out << "endmodule" << std::endl;
	out << std::endl;

	std::cout << "Attack> Done" << std::endl;
	std::cout << "Attack>" << std::endl;
	std::cout << "Attack> Evaluating connections:" << std::endl;

	// 3) evaluate the CCR: correct_connections over total_connections
	//
	total_connections = assignment.bottom_to_top.size() + assignment.top_to_bottom.size();
	correct_connections = 0;

	out << "//" << std::endl;
	out << "// F2F assignmets:" << std::endl;
	out << "//" << std::endl;
	out << "//  Bottom to top:" << std::endl;
	for (auto const& a : assignment.bottom_to_top) {

		out << "//   " << a.first << " -> " << a.second;

		// evaluate correct connections by name
		//
		// corner case: if only one assignment is possible, this is consider as correct; this is mimicked by using the same
		// string
		if (data.F2F.bottom_to_top.count(a.first) == 1) {
			out << " (correct)" << std::endl;
			correct_connections++;
		}
		// regular case: names match
		else if (a.second.find(a.first) != std::string::npos) {
			out << " (correct)" << std::endl;
			correct_connections++;
		}
		else {
			out << " (not correct)" << std::endl;
		}
	}
	out << "//" << std::endl;

	out << "//  Top to bottom:" << std::endl;
	out << "//" << std::endl;
	for (auto const& a : assignment.top_to_bottom) {

		out << "//   " << a.first << " -> " << a.second;

		// evaluate correct connections by name
		//
		// corner case: if only one assignment is possible, this is consider as correct; this is mimicked by using the same
		// string
		if (data.F2F.top_to_bottom.count(a.first) == 1) {
			out << " (correct)" << std::endl;
			correct_connections++;
		}
		// regular case: names match
		else if (a.first.find(a.second) != std::string::npos) {
			out << " (correct)" << std::endl;

			correct_connections++;
		}
		else {
			out << " (not correct)" << std::endl;
		}
	}
	out << "//" << std::endl;

	std::cout << "Attack>  Correct connections: " << correct_connections << std::endl;
	out << "// Correct connections: " << correct_connections << std::endl;

	std::cout << "Attack>  Total connections: " << total_connections << std::endl;
	out << "// Total connections: " << total_connections << std::endl;

	std::cout << "Attack>  Correct connections ratio: " << static_cast<double>(correct_connections) / total_connections << std::endl;
	out << "// Correct connections ratio: " << static_cast<double>(correct_connections) / total_connections << std::endl;

	out.close();
}

bool Attack::trial(Data& data, bool& success, unsigned& trials, std::mutex& m) {
	bool success_trial;

	// in case there's already a successful run, no need to continue
	if (success) {
		return true;
	}
	// in case there's no successful run yet, increase trials counter (using mutex)
	else {
		m.lock();
		trials++;
		m.unlock();
	}

	if (Attack::DBG) {
		std::cout << "DBG> Attack trial ..." << std::endl;
	}

	// graph container
	// mapping: name, node
	std::unordered_map<std::string, Data::Node> nodes;
	Data::AssignmentF2F assignment;

	// init the graph, and also connect the F2F nodes; may fail in case not all drivers can be mapped to some sink
	success_trial = Attack::initGraph(nodes, assignment, data, true);

	if (success_trial) {

		// check for cycles, start from global source
		//
		// returns true if cycle found; hence negate to indicate success (no cycle)
		success_trial = !Attack::checkGraphForCycles(
				&(nodes[data.globalNodeNames.source])
			);
	}

	// in case the run was successful, evaluate that run (using the mutex)
	if (success_trial) {
		m.lock();

		success = true;

		// evaluate assignment and output netlist
		Attack::evaluateAndOutput(assignment, data);

		m.unlock();
	}

	if (Attack::DBG) {
		if (success_trial) {
			std::cout << "DBG> Attack trial SUCCESS" << std::endl;
		}
		else {
			std::cout << "DBG> Attack trial FAIL" << std::endl;
		}
	}

	// for multithreading not required; success is passed as reference
	return success;
}

bool Attack::checkGraphForCycles(Data::Node const* node) {

	if (Attack::DBG) {
		std::cout << "DBG> Check graph for cycles; consider node: " << node->name << std::endl;
	}

	// node not visited/checked yet
	//
	//
	if (!node->visited) {

		if (Attack::DBG) {
			std::cout << "DBG>  Proceed with node " << node->name <<";";
			std::cout << " not visited yet; mark as visited and as part of recursion" << std::endl;
		}

		// mark as visited/checked, and also mark as part of this recursion
		node->visited = node->recursion = true;

		// now, check all children in depth-first manner
		//
		for (unsigned c = 0; c < node->children.size(); c++) {
			auto const* child = node->children[c];

			if (Attack::DBG) {
				std::cout << "DBG>   Consider node " << node->name << "'s child: " << child->name;
				std::cout << "; child " << c + 1 << "/" << node->children.size() << std::endl;
			}

			// child not visited yet; check recursively whether some cycle can be found
			//
			if (!child->visited && Attack::checkGraphForCycles(child)) {

				if (Attack::DBG) {
					std::cout << "DBG> Return from recursive check of node " << node->name << "'s child: " << child->name;
					std::cout << "; a cycle was found ..." << std::endl;
				}

				return true;
			}
			// child already visited; in case it has been visited during the current recursive call, then we found a cycle/backedge
			// http://www.geeksforgeeks.org/detect-cycle-in-a-graph/
			//
			else if (child->recursion) {

				if (Attack::DBG) {
					std::cout << "DBG>   Cycle found; passed this node " << child->name << " already during recursion" << std::endl;
				}

				return true;
			}
			// child already visited, but not part of the recursion anymore; represents a transitive edge from one parent node
			// to some child node, which is fine
			// 
			else {
				if (Attack::DBG) {
					std::cout << "DBG>   Cleared node " << node->name << "'s child: " << child->name;
					std::cout << "; child " << c + 1 << "/" << node->children.size() << std::endl;
				}
			}
		}
	}

	if (Attack::DBG) {
		std::cout << "DBG> Check graph for cycles; DONE consider node: " << node->name << std::endl;
	}

	// after return from recursion; mark as "not anymore part of a recursion"
	node->recursion = false;

	return false;
}

bool Attack::initGraph(std::unordered_map<std::string, Data::Node>& nodes, Data::AssignmentF2F& assignment, Data const& data, bool const& connectF2F) {
	bool success = true;

	if (Attack::DBG) {
		std::cout << "Attack> Initializing the graph ..." << std::endl;
	}

	// add global sink/source as nodes
	nodes.insert(std::make_pair(
				data.globalNodeNames.source,
				Data::Node(data.globalNodeNames.source)
			));
	nodes.insert(std::make_pair(
				data.globalNodeNames.sink,
				Data::Node(data.globalNodeNames.sink)
			));

	// add inputs as nodes, covers both PI and F2F inputs
	for (auto const& input : data.netlist.inputs) {

		nodes.insert(std::make_pair(
					input,
					Data::Node(input)
				));

		// also add new node for primary inputs as child to global source
		//
		// avoid F2F inputs as much as possible, which have "_" in name
		// note that some F2F inputs are PIs, so they will not avoided here; also note that avoiding F2F here only saves some
		// runtime (fewer children to be checked), but it does not undermine the checking for cycles
		//
		if (input.find("_") == std::string::npos) {
			nodes[data.globalNodeNames.source].children.emplace_back( &(nodes[input]) );
		}
	}

	// add outputs as nodes, covers both PO and F2F outputs
	for (auto const& output : data.netlist.outputs) {

		nodes.insert(std::make_pair(
					output,
					Data::Node(output)
				));

		// also add global sink as child for new node
		//
		// avoid F2F outputs as much as possible, which have "_" in name
		// note that some F2F outputs are POs, so they will not avoided here; also note that avoiding F2F here only saves some
		// runtime (fewer children to be checked), but it does not undermine the checking for cycles
		//
		if (output.find("_") == std::string::npos) {
			nodes[output].children.emplace_back(
					&(nodes[data.globalNodeNames.sink])
				);
		}
	}

	// add gates as nodes
	for (auto const& gate_iter : data.netlist.gates) {
		Data::Gate const& gate = gate_iter.second;

		nodes.insert(std::make_pair(
					gate.name,
					Data::Node(gate.name)
				));
	}

	// add wires as nodes
	for (auto const& wire : data.netlist.wires) {

		nodes.insert(std::make_pair(
					wire,
					Data::Node(wire)
				));
	}

	// connect graph based on connectivity of gates
	for (auto const& gate_iter : data.netlist.gates) {
		Data::Gate const& gate = gate_iter.second;

		// check all the inputs of the gate
		//
		// gate.inputs: cell pin is key, pin/net name is value
		for (auto const& input_iter : gate.inputs) {
			auto const& node_iter = nodes.find(input_iter.second);

			// there's a node matching the input of the gate
			if (node_iter != nodes.end()) {

				// memorize the gate's node as child of the node
				node_iter->second.children.emplace_back(
						&(nodes.find(gate.name)->second)
					);

				//// memorize the node as parent for the gate's node
				//nodes.find(gate.name)->second.parents.emplace_back(
				//		&(node_iter->second)
				//	);
			}
		}

		// check all the outputs of the gate
		//
		// gate.outputs: cell pin is key, pin/net name is value
		for (auto const& output_iter : gate.outputs) {
			auto const& node_iter = nodes.find(output_iter.second);

			// there's a node matching the output of the gate
			if (node_iter != nodes.end()) {

				// memorize the node as child for the gate's node
				nodes.find(gate.name)->second.children.emplace_back(
						&(node_iter->second)
					);

				//// memorize the gate's node as parent of the node
				//node_iter->second.parents.emplace_back(
				//		&(nodes.find(gate.name)->second)
				//	);
			}
		}
	}

	// connect also F2F nodes, if desired
	if (connectF2F) {

		// connect bottom-to-top nodes
		//
		// copy keys (output_bottom)
		auto output_bottom_set = data.F2F.keys_bottom_to_top;
		// copy assignments (input_top)
		auto input_top_map = data.F2F.bottom_to_top;

		if (Attack::DBG) {
			std::cout << "DBG> Picking randomly for bottom_to_top mappings ..." << std::endl;
		}

		// pick from output_bottom_set randomly until all are considered, also keep track whether each driver could be assigned to
		// some sink
		success &= Attack::pickAssignments(output_bottom_set, input_top_map, nodes, assignment);

		if (Attack::DBG) {
			std::cout << "DBG> Done ";
			if (success) {
				std::cout << "SUCCESS";
			}
			else {
				std::cout << "FAIL";
			}
			std::cout << std::endl;
		}

//		// dbg
//		//
//		// connect bottom-to-top nodes
//		for (auto const& output_bottom : data.F2F.keys_bottom_to_top) {
//
//			auto const& iter_range = data.F2F.bottom_to_top.equal_range(output_bottom);
//			for (auto iter = iter_range.first; iter != iter_range.second; ++iter) {
//
//				auto const& input_top = (*iter).second;
//
//				// pick the correct mapping based on names
//				// TODO does not capture any 1:1 mapping with different names, e.g. for POs
//				//
//				if (input_top.find(output_bottom) == std::string::npos) {
//					continue;
//				}
//
//				std::cout << "output_bottom: " << output_bottom << std::endl;
//				std::cout << "    input_top: " << input_top << std::endl;
//
//				// memorize the picked node as child for the output node
//				nodes.find(output_bottom)->second.children.emplace_back(
//					&(nodes.find(input_top)->second)
//				);
//
//				// also memorize the assignment
//				assignment.bottom_to_top.insert(std::make_pair(
//							output_bottom,
//							input_top
//						));
//			}
//		}

		// connect top-to-bottom nodes
		//
		// copy keys (output_top)
		auto output_top_set = data.F2F.keys_top_to_bottom;
		// copy assignments (input_bottom)
		auto input_bottom_map = data.F2F.top_to_bottom;

		if (Attack::DBG) {
			std::cout << "DBG> Picking randomly for top_to_bottom mappings ..." << std::endl;
		}

		// pick from output_bottom_set randomly until all are considered, also keep track whether each driver could be assigned to
		// some sink
		success &= Attack::pickAssignments(output_top_set, input_bottom_map, nodes, assignment);

		if (Attack::DBG) {
			std::cout << "DBG> Done ";
			if (success) {
				std::cout << "SUCCESS";
			}
			else {
				std::cout << "FAIL";
			}
			std::cout << std::endl;
		}

//		// dbg
//		//
//		// connect top-to-bottom nodes
//		for (auto const& output_top : data.F2F.keys_top_to_bottom) {
//
//			auto const& iter_range = data.F2F.top_to_bottom.equal_range(output_top);
//			for (auto iter = iter_range.first; iter != iter_range.second; ++iter) {
//
//				auto const& input_bottom = (*iter).second;
//
//				// pick the correct mapping based on names
//				// TODO does not capture any 1:1 mapping with different names, e.g. for POs
//				//
//				if (output_top.find(input_bottom) == std::string::npos) {
//					continue;
//				}
//
//				std::cout << "  output_top: " << output_top << std::endl;
//				std::cout << "input_bottom: " << input_bottom << std::endl;
//
//				// memorize the picked node as child for the output node
//				nodes.find(output_top)->second.children.emplace_back(
//						&(nodes.find(input_bottom)->second)
//					);
//
//				// also memorize the assignment
//				assignment.top_to_bottom.insert(std::make_pair(
//							output_top,
//							input_bottom
//						));
//			}
//		}
	}

	// dbg log
	//
	if (Attack::DBG) {

		std::cout << "DBG> Print all nodes: " << std::endl;

		// count all edges
		int edges = 0;
		for (auto const& node_iter : nodes) {
			auto const& node = node_iter.second;

			edges += node.children.size();
			//edges += node.parents.size();

			if (Attack::DBG) {
				std::cout << "DBG>  " << node.name << ":" << std::endl;

				std::cout << "DBG>   Children [" << node.children.size() << "]:";
				for (auto const* child : node.children) {
					std::cout << " " << child->name;
				}
				std::cout << std::endl;

				//std::cout << "DBG>   Parents [" << node.parents.size() << "]:";
				//for (auto const* parent : node.parents) {
				//	std::cout << " " << parent->name;
				//}
				//std::cout << std::endl;
			}
		}

		std::cout << "Attack> Done" << std::endl;
		std::cout << "Attack>  Nodes: " << nodes.size() << std::endl;
		std::cout << "Attack>  Edges: " << edges << std::endl;
		std::cout << "Attack> " << std::endl;
	}

	return success;
}
	
bool Attack::pickAssignments(std::set<std::string>& output_set,	std::unordered_multimap<std::string, std::string>& input_map, std::unordered_map<std::string, Data::Node>& nodes, Data::AssignmentF2F& assignment) {

	// pick F2F output pins randomly until all are considered
	//
	while (!output_set.empty()) {

		if (Attack::DBG) {
			std::cout << "DBG> Remaining output_set [" << output_set.size() << "]:" << std::endl;

			for (auto const& output : output_set) {
				std::cout << "DBG>  " << output << std::endl;
			}
		}

		// pick key randomly
		auto const& output = std::next(output_set.begin(),
				Attack::rand(0, output_set.size())
			);

		if (Attack::DBG) {
			std::cout << "DBG> output: " << *output << std::endl;
			std::cout << "DBG>  Remaining inputs for that output [" << input_map.count(*output) << "]:" << std::endl;

			auto iter = input_map.equal_range(*output);
			for (auto input = iter.first; input != iter.second; ++input) {
				std::cout << "DBG>   " << (*input).second << std::endl;
			}
		}

		// pick assignment for that key randomly
		//
		// sanity check whether any assignment remains; may arise due to unlucky selection of assignments such that no input
		// remains, or due to inappropriate definition for sinks in the mappings file
		if (input_map.count(*output) == 0) {
			return false;
		}

		// CANNOT USE REFERENCE HERE; otherwise the erasing procedure below, iterating over input_map, will mess up the value of
		// input
		auto input = std::next(input_map.equal_range(*output).first,
				Attack::rand(0, input_map.count(*output))
			)->second;

		if (Attack::DBG) {
			std::cout << "DBG>   Picking the following input: " << input << std::endl;
		}

		// memorize the node related to the picked assignment as child for the output node
		nodes.find(*output)->second.children.emplace_back(
			&(nodes.find(input)->second)
		);

		// also memorize the picked assignment
		assignment.bottom_to_top.insert(std::make_pair(
					*output,
					input
				));

		// erase all other assignments having the same input, to avoid multi-driver assignments
		for (auto iter = input_map.begin(); iter != input_map.end(); ) {

			if (iter->second == input) {
			
				if (Attack::DBG) {
					std::cout << "DBG>   Removing the following assignment for \"" << input << "\": ";
					std::cout << "\"" << iter->first << "\" -> \"" << iter->second << "\"";
					std::cout << std::endl;
				}

				iter = input_map.erase(iter);
			}
			else {
				//if (Attack::DBG) {
				//	std::cout << "DBG>   NOT removing the following assignment for \"" << input << "\": ";
				//	std::cout << "\"" << iter->first << "\" -> \"" << iter->second << "\"";
				//	std::cout << std::endl;
				//}

				++iter;
			}
		}

		// erase key from set
		output_set.erase(output);
	}

	return true;
}
