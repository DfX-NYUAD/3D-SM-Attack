
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
		// name, cell
		std::unordered_map<std::string, Cell> cells;

		// PODs for graph data
		struct Node {
			// name of gate or pin
			std::string name;
			std::vector<Node const*> parents;
			std::vector<Node const*> children;
		};
		// name, node
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
			std::set<std::string> inputs_global;
			std::set<std::string> outputs_global;
			std::set<std::string> inputs_F2F;
			std::set<std::string> outputs_F2F;
			std::set<std::string> wires;
			std::vector<Gate> gates;

			//// TODO for global input/output on whole graph
			//struct dummies {
			//	driverNode dummyDriver;
			//	crossNode dummyCross;
			//	sinkNode dummySink;
			//} dummies;
		};
		struct netlists {
			Netlist top;
			Netlist bottom;
			Netlist merged;
		} netlists;

		//// PODs for paths
		//struct path {
		//	driverNode const* driver;
		//	crossNode const* cross;
		//	sinkNode const* sink;

		//	path(driverNode const* d, crossNode const* c, sinkNode const* s) : driver(d), cross(c), sink(s) {}
		//};

		//// global data structure holding all possible mappings (set of paths)
		////std::map< std::string, std::vector<Data::path> > mappings;
		//std::map< std::list< std::pair<unsigned, unsigned> >, std::vector<Data::path> > mappings;
};

#endif
