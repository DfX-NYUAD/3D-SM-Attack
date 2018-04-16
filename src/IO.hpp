#ifndef _ATTACK_IO
#define _ATTACK_IO

// library includes
#include "Generic.hpp"
// forward declarations
class Data;

/// IO handler
class IO {
	private:
		/// debugging code switch (private)
		static constexpr bool DBG = true;

	// private data, functions
	private:

	// constructors, destructors, if any non-implicit
	private:
		/// empty default constructor; private in order to avoid instances of ``static'' class
		IO() {
		}

	// public data, functions
	public:
		static void testFile(std::string const& file);
		static void parseParametersFiles(Data& data, int const& argc, char** argv);
		static void parseNetlist(Data& data, bool const& top_tier);
};

#endif
