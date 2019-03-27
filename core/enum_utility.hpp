#pragma once
#ifndef ENUM_UTILITY_HPP
#define ENUM_UTILITY_HPP

#include <string>
#include <sstream>
#include <vector>
#include <regex>
#include <algorithm>

namespace internal_use
{
    static std::vector<std::string> get_arguments(const std::string& source)
    {
        using iterator = std::regex_token_iterator<std::string::const_iterator>;
		std::vector<std::string> tokens;
		std::regex reg("[, \n]");
		std::transform(
			iterator(source.begin(), source.end(), reg, -1),
			iterator(),
			std::back_inserter(tokens),
			[](auto m) { return std::string(m.first, m.second); }
		);

		auto new_end = std::remove_if(tokens.begin(), tokens.end(), [](std::string const& str) { return (str.empty() ? true : false); });
		tokens.erase(new_end, tokens.end());

		return tokens;	
    }
}

/*This declares a zero-based enum (int) which its arguments sequentially increments.*/

#define DECLARE_ENUM(enam_name, ...)  \
	enum enam_name { __VA_ARGS__, MAX_NUMBER_OF_##enam_name}; \
	static const std::vector<std::string> enam_name##Strings = internal_use::get_arguments(#__VA_ARGS__); \

/*This declares a zero-based enum with type which its arguments sequentially increments.*/
#define DECLARE_ENUM_WITH_TYPE(enam_name, enum_type, ...)  \
	enum enam_name : enum_type { __VA_ARGS__ , MAX_NUMBER_OF_##enam_name}; \
	static const std::vector<std::string> enam_name##Strings = internal_use::get_arguments(#__VA_ARGS__); \


#endif