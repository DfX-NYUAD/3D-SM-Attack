
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

	// parse bottom tier netlist
	IO::parseNetlist(data, false);
	// parse top tier netlist
	IO::parseNetlist(data, true);

	//// parse cross to sinks mapping in the bottom tier
	//IO::parseSinkMapping(data, false);
	//// parse driver to crosses mapping in the top tier
	//IO::parseDriverMapping(data, true);

	//// parse cross to sinks mapping in the top tier
	//IO::parseSinkMapping(data, true);
	//// parse driver to crosses mapping in the bottom tier
	//IO::parseDriverMapping(data, false);

	//// TODO for global source/sink, for cycle detection
	//// initialize dummy nodes
	//data.dummies.dummyDriver.id = 0;
	//data.dummies.dummyDriver.name = "DUMMY";
	//data.dummies.dummyCross.id = 0;
	//data.dummies.dummyCross.name = "DUMMY";
	//data.dummies.dummySink.id = 0;
	//data.dummies.dummySink.name = "DUMMY";

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
//
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
