#ifndef MACRO_UTILITY_HPP
#define MACRO_UTILITY_HPP

#include <cstring>

#ifdef _WIN32
	#define __SHORT_FILE__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#else
	#define __SHORT_FILE__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif

#endif // !MACRO_UTILITY_HPP