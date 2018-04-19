
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
		static bool trial(
				Data const& data,
				bool& success,
				unsigned& trials,
				std::mutex& m
			);
		static void initGraph(
				std::unordered_map<std::string, Data::Node>& nodes,
				Data::AssignmentF2F& assignment,
				Data const& data,
				bool const& connectF2F
			);
		static void evaluateAndOutput(
				Data::AssignmentF2F const& assignment,
				Data const& data
			);

		static bool checkGraphForCycles(Data::Node const* node);

		// random-number function
		//
		// note: range is [min, max)
		inline static int rand(int const& min, int const& max) {
			if (max == min) {
				return min;
			}
			else {
				return min + (std::rand() % (max - min));
			}
		};
};

#endif
