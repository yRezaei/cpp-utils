#pragma once
#ifndef PROFILING_UTILITY_HPP
#define PROFILING_UTILITY_HPP

#include "headers.hpp"

namespace profile
{
	template <typename F>
	double execution_time(const std::string& session_name, F&& f)
	{
		auto start = std::chrono::system_clock::now();
		f;
		auto elapsed_time = std::chrono::duration<double, std::milli>(std::chrono::system_clock::now() - start).count();
#ifndef PRINT_PROFILE_LOG
		std::cout << "-----------------------------------------------------------------------" << std::endl << std::left << std::setfill(' ')
			<< std::setw(40) << "Session name" << std::setw(30) << "Execution time [ms]" << std::endl
			<< std::setw(40) << session_name.substr(0, 40) << std::setw(30) << std::scientific << elapsed_time << std::endl
			<< "-----------------------------------------------------------------------" << std::endl;
#endif // PRINT_PROFILE_LOG
		return elapsed_time;
	}


	class Profile : public Singleton<Profile>
	{
	private:
		friend class Singleton<Profile>;
		std::unordered_map<std::string, Clock::time_point> sessions_;
	public:
		Profile() {}
		~Profile() {}

		void start(const std::string& session_name)
		{
			sessions_[session_name] = Clock::now();
		}

		double end(const std::string& session_name)
		{
			auto elapsed_time = std::chrono::duration<double, std::milli>(Clock::now() - sessions_[session_name]).count();
			std::cout << "Elapsed time for '" << session_name << "': " << std::right << std::setfill(' ') << std::setw(15) << elapsed_time << "[ms]" << std::endl;
			return elapsed_time;
		}
	};

	template <typename FUNC, typename ... Args>
	std::size_t time_it(FUNC&& f, Args... args)
	{
		auto start_time = std::chrono::system_clock::now();
		f(args...);
		return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start_time).count();
	}

}

#endif // !PROFILING_UTILITY_HPP

