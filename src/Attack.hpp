
#ifndef _ATTACK_MAIN
#define _ATTACK_MAIN

// library includes
#include "Generic.hpp"
#include "Data.hpp"

class Attack {
	private:
		/// debugging code switch (private)
		static constexpr bool DBG = false;

	// private data, functions
	private:

	// constructors, destructors, if any non-implicit
	private:

	// public data, functions
	public:
		static void initGraph(std::unordered_map<std::string, Data::Node>& nodes, Data const& data);
		static bool checkGraphForCycles(Data::Node const* node);
		//static void explorePath(Data& data, Data::path const& path, std::vector<Data::path> assigned_paths, std::list<Data::path> remaining_paths, unsigned recursive_level = 1);
};

#endif
