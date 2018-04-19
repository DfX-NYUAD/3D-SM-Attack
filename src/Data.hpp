
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
		// threads
		unsigned threads = 1;

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

			//enum class type_enum : unsigned {gate, input, output};
			//type_enum type;

			//std::vector<Node const*> parents;
			std::vector<Node const*> children;

			mutable bool visited = false;
			mutable bool recursion = false;

			Node(
				std::string n = "",
				//type_enum type,
				//std::vector<Node const*> p = std::vector<Node const*>(),
				std::vector<Node const*> c = std::vector<Node const*>()
			) :
				name(n),
				//type(t),
				//parents(p),
				children(c) {}
		};
		// names/identifier for global source/sink
		struct globalNodeNames {
			std::string source = "GLOBAL_SOURCE";
			std::string sink = "GLOBAL_SINK";
		} globalNodeNames;

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
			std::set<std::string> inputs;
			std::set<std::string> outputs;
			// wires may be redundant in top/bottom tier, so we also use a std::set here
			std::set<std::string> wires;
			// gates should not redundant; for better access, use map
			std::unordered_map<std::string, Gate> gates;
		} netlist;
		
		// PODs for F2F vias/pins
		struct F2F {
			// output pins of bottom to input pins of top
			std::unordered_multimap<std::string, std::string> bottom_to_top;
			// output pins of bottom
			std::set<std::string> keys_bottom_to_top;

			// output pins of top to input pins of bottom
			std::unordered_multimap<std::string, std::string> top_to_bottom;
			// output pins of top
			std::set<std::string> keys_top_to_bottom;
		} F2F;

		// the final assignments, to be populated in Attack::initGraph as well
		struct AssignmentF2F {

			std::unordered_map<std::string, std::string> bottom_to_top;
			std::unordered_map<std::string, std::string> top_to_bottom;

		};
};

#endif
