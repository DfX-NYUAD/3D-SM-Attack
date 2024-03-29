
#ifndef _ATTACK_MAIN
#define _ATTACK_MAIN

// library includes
#include "Generic.hpp"
#include "Data.hpp"

class Attack {
	private:
		/// debugging code switch (private)
		static constexpr bool DBG = false;
		static constexpr bool DBG_VERBOSE = false;

	// private data, functions
	private:
		static void rewriteConnectivity(
			std::pair<std::string, std::string> const& a,
			Data::Gate& gate,
			Data& data
		);
		static bool tackleF2F(
				std::unordered_set<std::string>& output_set,
				std::unordered_multimap<std::string, std::string>& input_map,
				std::unordered_multimap<std::string, std::string> const& inverted_input_map,
				std::unordered_map<std::string, Data::Node>& nodes,
				Data::AssignmentF2F& assignment,
				bool const& top_to_bottom
		);

	// constructors, destructors, if any non-implicit
	private:

	// public data, functions
	public:
		static void trial(
				Data& data,
				bool& success,
				unsigned& trials,
				std::mutex& m,
				std::chrono::time_point<std::chrono::system_clock>& start_time
			);
		static bool tackleGraph(
				std::unordered_map<std::string, Data::Node>& nodes,
				Data::AssignmentF2F& assignment,
				Data const& data
			);
		static void evaluateAndOutput(
				Data::AssignmentF2F const& assignment,
				Data& data
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
