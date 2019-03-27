#pragma once
#ifndef BASE_FLAG_HPP
#define BASE_FLAG_HPP

//Microsoft (R) C/C++ Optimizing Compiler Version 19.11.25547 for x64
#include <iostream>
#include <bitset>
#include <cassert>

template<typename T_ENUM>
//typename std::enable_if<std::is_enum<T_ENUM>::value, void>::type
struct BaseFlag
{
	BaseFlag() = default;
	BaseFlag(const BaseFlag &o) : bitset(o.bitset) {}
	BaseFlag(const T_ENUM &f) { bitset[f] = true; }

	BaseFlag& operator |= (const T_ENUM &val)
	{
		set_true(val);
		return *this;
	}

	BaseFlag& operator &= (const T_ENUM &val)
	{
		bool tmp = bitset[static_cast<utype>(val)] ? true : false;
		bitset.reset();
		bitset[static_cast<utype>(val)] = tmp;
		return *this;
	}

	BaseFlag& operator |= (const BaseFlag &o)
	{
		bitset |= o.bitset;
		return *this;
	}

	BaseFlag& operator &= (const BaseFlag &o)
	{
		bitset &= o.bitset;
		return *this;
	}

	BaseFlag& operator = (const BaseFlag &o)
	{
		bitset = o.bitset;
	}

	/**
	* Return a bitset containing the result of the
	* bitwise AND between *this and val.
	*
	* The resulting bitset can contain at most 1 bit.
	*/
	BaseFlag operator & (const T_ENUM& val)
	{
		BaseFlag ret(*this);
		ret &= val;

		assert(ret.bitset.count() <= 1);
		return ret;
	}

	/**
	* Perform a AND binary operation between *this and
	* `val` and return the result as a copy.
	*/
	BaseFlag operator & (const BaseFlag &val)
	{
		BaseFlag ret(*this);
		ret.bitset &= val.bitset;

		return ret;
	}

	/**
	* Return a bitset containing the result of the
	* bitwise OR between *this and val.
	*
	* The resulting bitset contains at least 1 bit.
	*/
	BaseFlag operator | (const T_ENUM& val)
	{
		BaseFlag ret(*this);
		ret |= val;

		assert(ret.bitset.count() >= 1);
		return ret;
	}

	/**
	* Perform a OR binary operation between *this and
	* `val` and return the result as a copy.
	*/
	BaseFlag operator | (const BaseFlag &val)
	{
		BaseFlag ret(*this);
		ret.bitset |= val.bitset;

		return ret;
	}

	BaseFlag operator ~ ()
	{
		BaseFlag cp(*this);
		cp.bitset.flip();

		return cp;
	}

	/**
	* The bitset evaluates to true if any bit is set.
	*/
	operator bool() const
	{
		return bitset.any();
	}

	/**
	* Below are the method from std::bitset that we expose.
	*/

	bool operator == (const BaseFlag &o) const
	{
		return bitset == o.bitset;
	}

	std::size_t size() const
	{
		return bitset.size();
	}

	std::size_t count() const
	{
		return bitset.count();
	}

	BaseFlag& set()
	{
		bitset.set();
		return *this;
	}

	BaseFlag& reset()
	{
		bitset.reset();
		return *this;
	}

	BaseFlag& flip()
	{
		bitset.flip();
		return *this;
	}

	BaseFlag &set(const T_ENUM &val, bool value = true)
	{
		bitset.set(static_cast<utype>(val), value);
		return *this;
	}

	BaseFlag &reset(const T_ENUM& val)
	{
		bitset.reset(static_cast<utype>(val));
		return *this;
	}

	BaseFlag &flip(const T_ENUM& val)
	{
		bitset.flip(static_cast<utype>(val));
		return *this;
	}

	bool operator[](const T_ENUM& val)
	{
		return bitset[static_cast<utype>(val)];
	}

	/**
	* Overload for std::ostream
	*/
	friend std::ostream& operator << (std::ostream& stream, const BaseFlag& me)
	{
		return stream << me.bitset;
	}

private:
	using enum_type = std::underlying_type_t<T_ENUM>;
	std::bitset<static_cast<enum_type>(T_ENUM::TOTAL_NR_OF_ELEMENTS)> bitset;

	void set_true(const T_ENUM& val)
	{
		bitset[static_cast<enum_type>(val)] = 1;
	}

};

/**
* Provide a free operator allowing to combine two enumeration
* member into a FlagSet.
*/
template<typename T_ENUM>
std::enable_if_t<std::is_enum<T_ENUM>::value, BaseFlag<T_ENUM>> operator | (const T_ENUM& lhs, const T_ENUM& rhs)
{
	BaseFlag<T_ENUM> bs;
	bs |= lhs;
	bs |= rhs;

	return bs;
}

#endif //!BASE_FLAG_HPP
