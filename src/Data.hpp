
#ifndef _ATTACK_DATA
#define _ATTACK_DATA

// library includes
#include "Generic.hpp"

class Data {
	private:
		/// debugging code switch (private)
		static constexpr bool DBG = false;

	// private data, functions
	private:

	// constructors, destructors, if any non-implicit
	private:

	// public data
	public:
		// POD for files names
		struct files {
			std::string bottom_netlist;
			std::string top_netlist;
			std::string obfuscated_mappings;
			std::string cells_inputs;
			std::string cells_outputs;
			std::string out_netlist;
		} files;

		// PODs for cells
		struct Cell {
			std::string type;
			std::vector<std::string> inputs;
			std::vector<std::string> outputs;
		};
		// actual container
		// mapping: name, cell
		std::unordered_map<std::string, Cell> cells;

		// PODs for graph data
		struct Node {
			// name of gate or pin
			std::string name;

			//enum class type : unsigned {gate, input_pin, output_pin};

			std::vector<Node const*> parents;
			std::vector<Node const*> children;

			Node(
				std::string n = "",
				std::vector<Node const*> p = std::vector<Node const*>(),
				std::vector<Node const*> c = std::vector<Node const*>()
			) : name(n), parents(p), children(c) {}
		};
		// global source/sink
		struct globalNodes {
			Node source = Node("GLOBAL_SOURCE");
			Node sink = Node("GLOBAL_SINK");
		} globalNodes;
		// actual container
		// mapping: name, node
		std::unordered_map<std::string, Node> nodes;

		// PODs for gates
		struct Gate {
			std::string type;
			std::string name;
			// cell pin name, net/pin name
			std::unordered_map<std::string, std::string> inputs;
			std::unordered_map<std::string, std::string> outputs;
		};

		// PODs for netlists
		struct Netlist {
			// I/Os can definitely be redundant in top/bottom tier, so we use a std::set here to avoid duplicates
			std::set<std::string> inputs_global;
			std::set<std::string> outputs_global;
			// wires may be redundant in top/bottom tier, so we also use a std::set here
			std::set<std::string> wires;
			// gates should not redundant; for better access, use map
			std::unordered_map<std::string, Gate> gates;
		} netlist;
		
		// PODs for F2F vias/pins
		struct F2F {
			std::vector<std::string> bottom_inputs;
			std::vector<std::string> bottom_outputs;
			std::vector<std::string> top_inputs;
			std::vector<std::string> top_outputs;
		} F2F;
};

#endif
