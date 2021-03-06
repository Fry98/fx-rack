/**
 * @file 	Utils.cpp
 *
 * @date 	19.1.2013
 * @author 	Zdenek Travnicek <travnicek@iim.cz>
 * @copyright GNU Public License 3.0
 *
 */

#include "iimavlib/Utils.h"
namespace iimavlib {
Log logger(std::cerr);

log_level Log::mode =
#ifdef NDEBUG
		log_level::info;
#else
		log_level::debug;
#endif
}



