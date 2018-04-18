
#include "IO.hpp"
#include "Attack.hpp"
#include "Data.hpp"

int main (int argc, char** argv) {
	Data data;

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

	// one attack trial
	std::cout << Attack::trial(data);
	std::cout << std::endl;
};

bool Attack::trial(Data const& data) {

	// graph container
	// mapping: name, node
	std::unordered_map<std::string, Data::Node> nodes;

	// init the graph, and also connect the F2F nodes
	Attack::initGraph(nodes, data, true);

	// check for cycles, start from global source
	return Attack::checkGraphForCycles(
			&(nodes[data.globalNodeNames.source])
		);
}

bool Attack::checkGraphForCycles(Data::Node const* node) {

	if (Attack::DBG) {
		std::cout << "DBG> Check graph for cycles; consider node: " << node->name << std::endl;
	}

	// node not visited/checked yet
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

void Attack::initGraph(std::unordered_map<std::string, Data::Node>& nodes, Data const& data, bool const& connectF2F) {

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
		for (auto const& output_bottom : data.F2F.keys_bottom_to_top) {

			// pick one randomly
			auto const& random = Attack::rand(0, data.F2F.bottom_to_top.count(output_bottom) - 1);
			auto const& input_top = std::next(data.F2F.bottom_to_top.equal_range(output_bottom).first, random)->second;

			// memorize the picked node as child for the output node
			nodes.find(output_bottom)->second.children.emplace_back(
				&(nodes.find(input_top)->second)
			);

//			// dbg; pick the correct mapping based on names
//			auto const& iter_range = data.F2F.bottom_to_top.equal_range(output_bottom);
//			for (auto iter = iter_range.first; iter != iter_range.second; ++iter) {
//
//				auto const& input_top = (*iter).second;
//
//				// dbg; pick the correct mapping based on names
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
//			}
		}

		// connect top-to-bottom nodes
		for (auto const& output_top : data.F2F.keys_top_to_bottom) {

			// pick one randomly
			auto const& random = Attack::rand(0, data.F2F.top_to_bottom.count(output_top) - 1);
			auto const& input_bottom = std::next(data.F2F.top_to_bottom.equal_range(output_top).first, random)->second;

			// memorize the picked node as child for the output node
			nodes.find(output_top)->second.children.emplace_back(
					&(nodes.find(input_bottom)->second)
				);

//			// dbg; pick the correct mapping based on names
//			auto const& iter_range = data.F2F.top_to_bottom.equal_range(output_top);
//			for (auto iter = iter_range.first; iter != iter_range.second; ++iter) {
//
//				auto const& input_bottom = (*iter).second;
//
//				// dbg; pick the correct mapping based on names
//				if (input_bottom.find(output_top) == std::string::npos) {
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
//			}
		}
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
}
