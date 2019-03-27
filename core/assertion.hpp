#pragma once
#ifndef CORE_ASSERTION_H
#define CORE_ASSERTION_H

#include "headers.hpp"

#define SOURCE std::string("File \"" + std::string(__FILE__) + "\":" + std::to_string(__LINE__)) + ": "

#define ASSERT(message) \
		std::cout << "[Assertion] " << SOURCE << message << std::endl; \
		std::exit(1); \

#define ASSERT_IF_NULL(condition, message) \
    if (!condition) { \
		std::cout << "[Assertion] " << SOURCE << message << std::endl; \
		std::exit(1); \
    }

#define ASSERT_IF_TRUE(condition, message) \
	if ((condition)) { \
		std::cout << "[Assertion] " << SOURCE << message << std::endl; \
		std::exit(1); \
	} \

#define ASSERT_IF_FALSE(condition, message) \
	if (!(condition)) { \
		std::cout << "[Assertion] " << SOURCE << message << std::endl; \
		std::exit(1); \
	} \

#define CHECK_IF_TRUE(condition, message) \
	if ((condition)) { \
		std::cout << "[Warning] " << SOURCE << message << std::endl; \
	} \

#define CHECK_IF_FALSE(condition, message) \
	if (!(condition)) { \
		std::cout << "[Warning] " << SOURCE << message << std::endl; \
	} \


#endif // !CORE_ASSERTION_H