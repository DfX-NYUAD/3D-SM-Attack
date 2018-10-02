
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

	// set locale for output; for using thousand separators
	std::cout.imbue(std::locale(""));

	// parse program parameters and test files
	IO::parseParametersFiles(data, argc, argv);

	// parse cell inputs
	IO::parseCells(data, false);
	// parse cell outputs
	IO::parseCells(data, true);

	// for the netlist, parse regular ports to begin with
	IO::parseRegularPorts(data);
	// parse bottom tier netlist
	IO::parseNetlist(data, false);
	// parse top tier netlist
	IO::parseNetlist(data, true);

	// finally, parse the F2F mappings
	IO::parseMappings(data);

	// init random generator with high-resolution timing seed
	auto now = std::chrono::high_resolution_clock::now();
	auto nanos = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();
	std::srand(nanos);

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
			if (data.netlist.outputs.find(a.first) != data.netlist.outputs.end()) {
				output.second = a.first;
			}
			// regular case, no PO; rename gate output towards new wire
			else {
				output.second = a.first + "_" + a.second;
			}

			// also memorize that new wire; by using set, no redundant wires arise
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
			if (data.netlist.outputs.find(a.first) != data.netlist.outputs.end()) {
				input.second = a.first;
			}
			// regular case, no PO; rename gate input towards new wire
			else {
				input.second = a.first + "_" + a.second;
			}

			// also memorize that new wire; by using set, no redundant wires arise
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
	for (auto& gate : data.netlist.gates) {

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

	// output all inputs for module ports
	for (auto const& input : data.netlist.inputs) {
		out << input << ", ";
	}

	// output all outputs for module ports
	unsigned count = 1;
	for (auto const& output : data.netlist.outputs) {

		// the last output has no comma following, and it also closes the port list
		if (count == data.netlist.outputs.size()) {
			out << output << ");" << std::endl;
		}
		else {
			out << output << ", ";
			count++;
		}
	}
	out << std::endl;

	// output all inputs
	for (auto const& input : data.netlist.inputs) {
		out << "input " << input << ";" << std::endl;
	}
	out << std::endl;

	// output all outputs
	for (auto const& output : data.netlist.outputs) {
		out << "output " << output << ";" << std::endl;
	}
	out << std::endl;

	// output all wires
	for (auto const& wire : data.netlist.wires) {
		out << "wire " << wire << ";" << std::endl;
	}
	out << std::endl;

	// output all gates 
	for (auto const& gate: data.netlist.gates) {
		unsigned outputs_remaining = gate.outputs.size();
		unsigned inputs_remaining = gate.inputs.size();

		out << gate.type << " ";
		out << gate.name << " ";

		out << "(";

		for (auto const& input : gate.inputs) {

			// corner case: no outputs are present and this is the last input, so end port mapping already here
			if (outputs_remaining == 0 && inputs_remaining == 1) {
				out << "." << input.first << "(" << input.second << ")";
			}
			else {
				out << "." << input.first << "(" << input.second << "), ";
				inputs_remaining--;
			}
		}

		// only one output
		if (outputs_remaining == 1) {
			out << "." << gate.outputs.begin()->first << "(" << gate.outputs.begin()->second << ")";
		}
		// multiple outputs (or zero outputs)
		else {
			for (auto const& output : gate.outputs) {

				if (outputs_remaining == 1) {
					out << "." << output.first << "(" << output.second << ")";
				}
				else {
					out << "." << output.first << "(" << output.second << "), ";
					outputs_remaining--;
				}
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

void Attack::trial(Data& data, bool& success, unsigned& trials, std::mutex& m) {
	bool success_trial;

	// in case there's already a successful run, no need to continue
	if (success) {
		return;
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

	// init the graph, attack/tackle also the F2F nodes
	//
	// It's important to understand that the based graph (w/o F2F connections) CANNOT be initialized once in the beginning, and then
	// copied for each thread -- the pointers for children in the graph would be based on the initial data, hence, adding any F2F edges
	// even on the copy of the graph would impose those edits on the initial data of the base graph.
	// Besides, initializing the graph once but then having to rewrite all the pointers for any copy would probably not help much for
	// performance.
	//
	success_trial = Attack::tackleGraph(nodes, assignment, data);

	// graph could be build up
	if (success_trial) {

		// now check for cycles, start from global source
		//
		// returns true if cycle found; hence negate to indicate success (no cycle)
		success_trial = !Attack::checkGraphForCycles(
				&(nodes[data.globalNodeNames.source])
			);

		// in case the run was the first successful one, evaluate that run and generate obtained netlist (using the mutex)
		if (success_trial & !success) {
			m.lock();

			success = true;

			// evaluate assignment and output netlist
			Attack::evaluateAndOutput(assignment, data);

			m.unlock();
		}
	}
	// handling of the graph may fail in case not all drivers can be mapped to some sink
	else {
		std::cout << "Attack> Failed to assign all F2F mappings; check the mappings.file ..." << std::endl;
	}

	if (Attack::DBG) {
		if (success_trial) {
			std::cout << "DBG> Attack trial SUCCESS" << std::endl;
		}
		else {
			std::cout << "DBG> Attack trial FAIL" << std::endl;
		}
	}
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

bool Attack::tackleGraph(std::unordered_map<std::string, Data::Node>& nodes, Data::AssignmentF2F& assignment, Data const& data) {
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

	// add inputs as nodes
	// 
	for (auto const& input : data.netlist.inputs) {

		nodes.insert(std::make_pair(
					input,
					Data::Node(input)
				));

		// also add new node for primary inputs as child to global source
		nodes[data.globalNodeNames.source].children.emplace_back( &(nodes[input]) );
	}

	// add outputs as nodes
	for (auto const& output : data.netlist.outputs) {

		nodes.insert(std::make_pair(
					output,
					Data::Node(output)
				));

		// also add global sink as child for new node
		nodes[output].children.emplace_back( &(nodes[data.globalNodeNames.sink]) );
	}

	// add F2F ports as nodes
	for (auto const& port : data.netlist.F2F) {

		nodes.insert(std::make_pair(
					port,
					Data::Node(port)
				));
	}

	// add gates as nodes
	for (auto const& gate : data.netlist.gates) {

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
	for (auto const& gate : data.netlist.gates) {
		auto const& gate_iter = nodes.find(gate.name);

		// check all the inputs of the gate
		//
		// gate.inputs: cell pin is key, pin/net name is value
		for (auto const& input_iter : gate.inputs) {
			auto const& node_iter = nodes.find(input_iter.second);

			// there's a node matching the input of the gate
			if (node_iter != nodes.end()) {

				// memorize the gate's node as child of the node
				node_iter->second.children.emplace_back(
						&(gate_iter->second)
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
				gate_iter->second.children.emplace_back(
						&(node_iter->second)
					);

				//// memorize the gate's node as parent of the node
				//node_iter->second.parents.emplace_back(
				//		&(nodes.find(gate.name)->second)
				//	);
			}
		}
	}

	// connect F2F nodes -- heart of the attack
	//

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
	success &= Attack::tackleF2F(output_bottom_set, input_top_map, data.F2F.inverted_bottom_to_top, nodes, assignment, false);

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

//	// dbg
//	//
//	// connect bottom-to-top nodes
//	for (auto const& output_bottom : data.F2F.keys_bottom_to_top) {
//
//		auto const& iter_range = data.F2F.bottom_to_top.equal_range(output_bottom);
//		for (auto iter = iter_range.first; iter != iter_range.second; ++iter) {
//
//			auto const& input_top = (*iter).second;
//
//			// pick the correct mapping based on names
//			// TODO does not capture any 1:1 mapping with different names, e.g. for POs
//			//
//			if (input_top.find(output_bottom) == std::string::npos) {
//				continue;
//			}
//
//			std::cout << "output_bottom: " << output_bottom << std::endl;
//			std::cout << "    input_top: " << input_top << std::endl;
//
//			// memorize the picked node as child for the output node
//			nodes.find(output_bottom)->second.children.emplace_back(
//				&(nodes.find(input_top)->second)
//			);
//
//			// also memorize the assignment
//			assignment.bottom_to_top.insert(std::make_pair(
//						output_bottom,
//						input_top
//					));
//		}
//	}

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
	success &= Attack::tackleF2F(output_top_set, input_bottom_map, data.F2F.inverted_top_to_bottom, nodes, assignment, true);

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

//	// dbg
//	//
//	// connect top-to-bottom nodes
//	for (auto const& output_top : data.F2F.keys_top_to_bottom) {
//
//		auto const& iter_range = data.F2F.top_to_bottom.equal_range(output_top);
//		for (auto iter = iter_range.first; iter != iter_range.second; ++iter) {
//
//			auto const& input_bottom = (*iter).second;
//
//			// pick the correct mapping based on names
//			// TODO does not capture any 1:1 mapping with different names, e.g. for POs
//			//
//			if (output_top.find(input_bottom) == std::string::npos) {
//				continue;
//			}
//
//			std::cout << "  output_top: " << output_top << std::endl;
//			std::cout << "input_bottom: " << input_bottom << std::endl;
//
//			// memorize the picked node as child for the output node
//			nodes.find(output_top)->second.children.emplace_back(
//					&(nodes.find(input_bottom)->second)
//				);
//
//			// also memorize the assignment
//			assignment.top_to_bottom.insert(std::make_pair(
//						output_top,
//						input_bottom
//					));
//		}
//	}

	// dbg log
	//
	if (Attack::DBG) {

		std::cout << "DBG> Print netlist graph, with F2F connections: " << std::endl;

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
	
bool Attack::tackleF2F(std::unordered_set<std::string>& output_set, std::unordered_multimap<std::string, std::string>& input_map, std::unordered_multimap<std::string, std::string> const& inverted_input_map, std::unordered_map<std::string, Data::Node>& nodes, Data::AssignmentF2F& assignment, bool const& top_to_bottom) {

	// tackle F2F outputs to inputs for the other tier until all are resolved
	//
	while (!output_set.empty()) {

		if (Attack::DBG) {
			std::cout << "DBG> Remaining output_set [" << output_set.size() << "]:" << std::endl;

			for (auto const& output : output_set) {
				std::cout << "DBG>  " << output << std::endl;
			}
		}

		// pick key/output randomly
		auto const& output = std::next(output_set.begin(),
				Attack::rand(0, output_set.size())
			);

		if (Attack::DBG) {
			std::cout << "DBG> output: " << *output << std::endl;
			std::cout << "DBG>  Remaining inputs -- in other tier! -- for that output [" << input_map.count(*output) << "]:" << std::endl;

			auto iter = input_map.equal_range(*output);
			for (auto input = iter.first; input != iter.second; ++input) {
				std::cout << "DBG>   " << (*input).second << std::endl;
			}
		}

		// pick assignment for that key/output randomly
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
			std::cout << "DBG>   Picking the following input port from the other tier as being driven by \"" << *output << "\": \"" << input << "\"" << std::endl;
		}

		// memorize the node related to the picked assignment as child for the output node
		nodes.find(*output)->second.children.emplace_back(
			&(nodes.find(input)->second)
		);

		if (Attack::DBG) {
			std::cout << "DBG>   Memorize the assignment in the graph..." << std::endl;
			std::cout << "DBG>    Node " << nodes.find(*output)->second.name << "'s new set of children:" << std::endl;
			for (auto const* child : nodes.find(*output)->second.children) {
				std::cout << "DBG>     " << child->name << std::endl;
			}
		}


		// also memorize the picked assignment
		if (top_to_bottom) {
			assignment.top_to_bottom.insert(std::make_pair(
						*output,
						input
					));
			}
		else {
			assignment.bottom_to_top.insert(std::make_pair(
						*output,
						input
					));
		}

		// erase all other assignments from any output to this same input, to avoid multi-driver assignments
		// 
		// also, considering there are only 1-to-1 F2F mappings, erase all other possible mappings for the output/driver just
		// assigned
		//
		auto outputs = inverted_input_map.equal_range(input);

		for (auto output_iter = outputs.first; output_iter != outputs.second; ++output_iter) {

			auto inputs = input_map.equal_range(output_iter->second);

			for (auto input_iter = inputs.first; input_iter != inputs.second; ) {

				if (input_iter->second == input) {

					if (Attack::DBG) {
						std::cout << "DBG>   Removing the following other, previously possible driver/assignment for \"" << input << "\": ";
						std::cout << "\"" << input_iter->first << "\" -> \"" << input_iter->second << "\"";
						std::cout << std::endl;
					}

					input_iter = input_map.erase(input_iter);
				}
				else if (input_iter->first == *output) {

					if (Attack::DBG) {
						std::cout << "DBG>   Removing the following other, previously possible driver/assignment for \"" << *output << "\": ";
						std::cout << "\"" << input_iter->first << "\" -> \"" << input_iter->second << "\"";
						std::cout << std::endl;
					}

					input_iter = input_map.erase(input_iter);
				}
				else {
					if (Attack::DBG) {
						std::cout << "DBG>   NOT removing the following assignment: ";
						std::cout << "\"" << input_iter->first << "\" -> \"" << input_iter->second << "\"";
						std::cout << std::endl;
					}

					++input_iter;
				}
			}
		}

		// erase key/output from set of yet unassigned outputs
		output_set.erase(output);
	}

	return true;
}
