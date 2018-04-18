
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

	// initialize the graph
	//
	// actual container
	// mapping: name, node
	std::unordered_map<std::string, Data::Node> nodes;
	Attack::initGraph(nodes, data);
//	auto nodes_copy = nodes;

	// check for cycles, start from global source
	Attack::checkGraphForCycles(
			&(nodes[data.globalNodeNames.source])
		);

//	std::cout << "BLUBB" << std::endl;
//
//	std::unordered_map<std::string, Data::Node> nodes_copy;
//	Attack::initGraph(nodes_copy, data);
//	Attack::checkGraphForCycles(
//			&(nodes_copy[data.globalNodeNames.source])
//		);

//	// explore all the possible driver->cross->sink paths and the related mappings
//	Attack::determineAllPaths(data);
//
//	// output mappings
//	//
//	std::cout << "Attack> Possible scenarios/mappings for an attacker to consider: " << data.mappings.size() << std::endl;
//	std::cout << "Attack>" << std::endl;
//
//	unsigned i = 1;
//	for (auto const& mapping : data.mappings) {
//
//		std::cout << "Attack> Mapping " << i << ":" << std::endl;
//		i++;
//
//		for (auto const& path : mapping.second) {
//			std::cout << "Attack>  Path: " << path.driver->name << " => (" << path.cross->name << " =>) " << path.sink->name << std::endl;
//		}
//	}
//
//	// also write to output file if desired
//	if (data.files.out_netlist != "") {
//		std::ofstream out;
//		std::ifstream in;
//
//		out.open(data.files.out_netlist.c_str());
//
//		// first copy-paste pin files
//
//		in.open(data.files.top_netlist.c_str());
//
//		out << data.files.top_netlist << ":" << std::endl;
//		out << in.rdbuf();
//		out << std::endl;
//
//		in.close();
//		in.open(data.files.bottom_netlist.c_str());
//
//		out << data.files.bottom_netlist << ":" << std::endl;
//		out << in.rdbuf();
//		out << std::endl;
//
//		in.close();
//
//		// now, write out mappings
//		unsigned i = 1;
//		for (auto const& mapping : data.mappings) {
//
//			out << "Mapping " << i << ":" << std::endl;
//			i++;
//
//			for (auto const& path : mapping.second) {
//				out << " Path: " << path.driver->name << " => (" << path.cross->name << " =>) " << path.sink->name << std::endl;
//			}
//		}
//	}
};

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

void Attack::initGraph(std::unordered_map<std::string, Data::Node>& nodes, Data const& data) {

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

	// add primary inputs as nodes
	for (auto const& input : data.netlist.inputs_global) {

		nodes.insert(std::make_pair(
					input,
					Data::Node(input)
				));

		// also add new node for primary inputs as child to global source
		nodes[data.globalNodeNames.source].children.emplace_back( &(nodes[input]) );
	}

	// add primary outputs as nodes
	for (auto const& output : data.netlist.outputs_global) {

		nodes.insert(std::make_pair(
					output,
					Data::Node(output)
				));

		// also add global sink as child for new node
		nodes[output].children.emplace_back(
				&(nodes[data.globalNodeNames.sink])
			);
	}

	// add F2F inputs/outputs as nodes
	for (auto const& input : data.F2F.bottom_inputs) {

		nodes.insert(std::make_pair(
					input,
					Data::Node(input)
				));
	}
	for (auto const& input : data.F2F.top_inputs) {

		nodes.insert(std::make_pair(
					input,
					Data::Node(input)
				));
	}
	for (auto const& output : data.F2F.bottom_outputs) {

		nodes.insert(std::make_pair(
					output,
					Data::Node(output)
				));
	}
	for (auto const& output : data.F2F.top_outputs) {

		nodes.insert(std::make_pair(
					output,
					Data::Node(output)
				));
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

//// explore paths recursively, work on copies of paths from prior recursion level
////
//void Attack::explorePath(Data& data, Data::path const& path, std::vector<Data::path> assigned_paths, std::list<Data::path> remaining_paths, unsigned recursive_level) {
//
//	// update the list of paths so far assigned
//	assigned_paths.push_back(path);
//	
//	// remove all the paths which share the driver, the cross, or the sink
//	for (auto iter = remaining_paths.begin(); iter != remaining_paths.end();) {
//		auto curr_path = *iter;
//
//		if (curr_path.driver->id == path.driver->id) {
//			iter = remaining_paths.erase(iter);
//		}
//		else if (curr_path.cross->id == path.cross->id) {
//			iter = remaining_paths.erase(iter);
//		}
//		else if (curr_path.sink->id == path.sink->id) {
//			iter = remaining_paths.erase(iter);
//		}
//		else {
//			++iter;
//		}
//	}
//
//	// dbg log
//	if (Attack::DBG) {
//
//		std::cout << "ATTACK_DBG>";
//		for (unsigned indent = 0; indent < recursive_level; indent++) {
//			std::cout << "\t";
//		}
//		std::cout << " RECURSIVE LEVEL: " << recursive_level << std::endl;
//
//		std::cout << "ATTACK_DBG>";
//		for (unsigned indent = 0; indent < recursive_level; indent++) {
//			std::cout << "\t";
//		}
//		std::cout << "  REMAINING PATHS: " << remaining_paths.size() << std::endl;
//
//		for (auto const& path : remaining_paths) {
//
//			std::cout << "ATTACK_DBG>";
//			for (unsigned indent = 0; indent < recursive_level; indent++) {
//				std::cout << "\t";
//			}
//			std::cout << "   PATH: " << path.driver->name << " => " << path.cross->name << " => " << path.sink->name << std::endl;
//		}
//	}
//
//	// if some paths remain, continue recursively
//	//
//	if (!remaining_paths.empty()) {
//		for (auto const& path : remaining_paths) {
//
//			if (Attack::DBG) {
//				std::cout << "ATTACK_DBG>";
//				for (unsigned indent = 0; indent < recursive_level; indent++) {
//					std::cout << "\t";
//				}
//				std::cout << "  PATH TO EXPLORE: " << path.driver->name << " => " << path.cross->name << " => " << path.sink->name << std::endl;
//			}
//
//			// pass assigned_paths and remaining_paths as copy
//			Attack::explorePath(data, path, assigned_paths, remaining_paths, recursive_level + 1);
//
//			// dbg log after return from recursive calls
//			if (Attack::DBG) {
//
//				std::cout << "ATTACK_DBG>";
//				for (unsigned indent = 0; indent < recursive_level; indent++) {
//					std::cout << "\t";
//				}
//				std::cout << " DONE RECURSIVE LEVEL: " << recursive_level << std::endl;
//			}
//		}
//	}
//	// no path remains, so we're done with this recursive round of path assignments
//	//
//	// memorize assignments in global data structure, without cross
//	//
//	else {
//		// check whether any driver remains unassigned (due to suboptimal selection of crosses), put dummy assignments
//		for (auto const& driver_iter : data.graph.drivers) {
//			auto const& driver = driver_iter.second;
//
//			bool driver_covered = false;
//
//			for (auto const& path : assigned_paths) {
//
//				if (path.driver->id == driver.id) {
//
//					driver_covered = true;
//					break;
//				}
//			}
//
//			if (!driver_covered) {
//				assigned_paths.push_back( Data::path(&driver, &data.dummies.dummyCross, &data.dummies.dummySink) );
//			}
//		}
//		// check whether any sink remains unassigned (due to suboptimal selection of crosses), put dummy assignments
//		for (auto const& sink_iter : data.graph.sinks) {
//			auto const& sink = sink_iter.second;
//
//			bool sink_covered = false;
//
//			for (auto const& path : assigned_paths) {
//
//				if (path.sink->id == sink.id) {
//
//					sink_covered = true;
//					break;
//				}
//			}
//
//			if (!sink_covered) {
//				assigned_paths.push_back( Data::path(&data.dummies.dummyDriver, &data.dummies.dummyCross, &sink) );
//			}
//		}
//
//		// sort list by 1) driver id and 2) sink id
//		std::sort(assigned_paths.begin(), assigned_paths.end(),
//				// lambda expression
//				[](Data::path const& p1, Data::path const& p2) {
//
//					if (p1.driver->id < p2.driver->id) {
//						return true;
//					}
//					else if ((p1.driver->id == p2.driver->id) && (p1.sink->id < p2.sink->id)) {
//						return true;
//					}
//					else {
//						return false;
//					}
//				});
//
//		// dbg log
//		if (Attack::DBG) {
//
//			std::cout << "ATTACK_DBG>";
//			for (unsigned indent = 0; indent < recursive_level; indent++) {
//				std::cout << "\t";
//			}
//			std::cout << " DONE; ASSIGNED PATHS:" << std::endl;
//
//			for (auto const& path : assigned_paths) {
//				std::cout << "ATTACK_DBG>";
//				for (unsigned indent = 0; indent < recursive_level; indent++) {
//					std::cout << "\t";
//				}
//				std::cout << "  " << path.driver->name << " => " << path.cross->name << " => " << path.sink->name << std::endl;
//			}
//		}
//
//		// insert assignment into global map
//		//
//		std::pair<
//			std::map< std::list<std::pair<unsigned, unsigned>>, std::vector<Data::path> >::iterator,
//			bool > ret;
//
//		// generate key for this assignment
//		std::list< std::pair<unsigned, unsigned> > key;
//
//		for (auto const& path : assigned_paths) {
//
//			key.push_back(std::make_pair(
//						path.driver->id,
//						path.sink->id
//					));
//		}
//
//		// actual insertion
//		ret = data.mappings.emplace(key, assigned_paths);
//
//		// dbg log
//		if (Attack::DBG) {
//
//			std::cout << "ATTACK_DBG>";
//			for (unsigned indent = 0; indent < recursive_level; indent++) {
//				std::cout << "\t";
//			}
//			if (ret.second) {
//				std::cout << " NEW ASSIGNMENT" << std::endl;
//			}
//			else {
//				std::cout << " ASSIGNMENT ALREADY THERE; NOT ADDED" << std::endl;
//			}
//		}
//	}
//};
//
//void Attack::determineAllPaths(Data& data) {
//	std::list<Data::path> paths;
//
//	// first, determine all possible paths
//	//
//	for (auto const& driver_iter : data.graph.drivers) {
//		auto const& driver = driver_iter.second;
//
//		for (auto* cross: driver.crosses) {
//
//			for (auto* sink : cross->sinks) {
//
//				paths.emplace_back( Data::path(&driver, cross, sink) );
//			}
//		}
//	}
//
//	std::cout << "Attack> All possible paths: " << paths.size() << std::endl;
//	for (auto const& path : paths) {
//		std::cout << "Attack>  Path: " << path.driver->name << " => " << path.cross->name << " => " << path.sink->name << std::endl;
//	}
//
//	// then, try each path individually in an recursive manner
//	//
//	unsigned paths_remaining = paths.size();
//	for (auto& path : paths) {
//
//		unsigned mappings_before = data.mappings.size();
//		paths_remaining--;
//
//		std::cout << "Attack> Path under exploration: ";
//		std::cout << path.driver->name << " => " << path.cross->name << " => " << path.sink->name;
//		std::cout << " ..." << std::endl;
//
//		// enter recursion
//		Attack::explorePath(data, path, std::vector<Data::path>(), paths);
//
//		std::cout << "Attack>  Done; added mappings: " << data.mappings.size() - mappings_before;
//		std::cout << " paths remaining: " << paths_remaining << std::endl;
//	}
//};
