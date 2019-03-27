#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <fstream>
#include <map>
#include "singleton.hpp"
#include "string_utility.hpp"
#include "cmd_utility.hpp"
#include "time_utility.hpp"
#include "enum_utility.hpp"
#include "macro_utility.hpp"

class Logger
{
protected:
    std::string name_;

public:
    ~Logger() {}
    const std::string name() { return name_; }
    virtual void info(const std::string& msg) = 0;
    virtual void success(const std::string& msg) = 0;
    virtual void warning(const std::string& msg) = 0;
    virtual void error(const std::string& msg) = 0;
};

class LoggerFactory: public Singleton<LoggerFactory>
{
private:
    friend class Singleton<LoggerFactory>;

    class ConsoleLogger : public Logger
    {
    public:
        ConsoleLogger(const std::string& logger_name) { name_ = logger_name; }
        ~ConsoleLogger() {}

        void info(const std::string& msg)
        {
            std::cout << "[" << TimeStamp().to_str() << "] " << "[ " 
                        << cmd::color::style::bold << cmd::color::fgB::blue << "Info" << cmd::color::fg::reset << "  ] --> "
                        << msg << std::endl;
        }

        void success(const std::string& msg)
        {
            std::cout << "[" << TimeStamp().to_str() << "] " << "[" 
                        << cmd::color::style::bold << cmd::color::fgB::green << "Success" << cmd::color::fg::reset << "] --> "
                        << msg << std::endl;
        }

        void warning(const std::string& msg)
        {
            std::cout << "[" << TimeStamp().to_str() << "] " << "[" 
                        << cmd::color::style::bold << cmd::color::fgB::yellow << "Warning" << cmd::color::fg::reset << "] "
                        << msg << std::endl;
        }

        void error(const std::string& msg)
        {
            std::cout << "[" << TimeStamp().to_str() << "] " << "[ " 
                        << cmd::color::style::bold << cmd::color::fgB::red << "Error" << cmd::color::fg::reset << " ] --> "
                        << msg << std::endl;
        }

    };

    class FileLogger : public Logger
    {
    private:
        const std::string file_name_;
        std::ofstream log_file_;
    public:
        FileLogger(const std::string& logger_name, const std::string& file_name ) : file_name_(file_name) 
        { 
            name_ = logger_name; 
            log_file_.open(file_name);
            if(log_file_.is_open())
                log_file_ << "[" << TimeStamp().to_str() << "] " << str::format(" Logger '{}' started.", logger_name) << std::endl;
        }

        ~FileLogger() 
        {
            if(log_file_.is_open())
                log_file_.close();
        }

        void info(const std::string& msg)
        {
            if(log_file_.is_open())
                log_file_ << "[" << TimeStamp().to_str() << "] [ Info  ] --> " << msg << std::endl;
        }

        void success(const std::string& msg)
        {
            if(log_file_.is_open())
                log_file_ << "[" << TimeStamp().to_str() << "] [Success] --> " << msg << std::endl;
        }

        void warning(const std::string& msg)
        {
            if(log_file_.is_open())
                log_file_ << "[" << TimeStamp().to_str() << "] [Warning] --> " << msg << std::endl;
        }

        void error(const std::string& msg)
        {
            if(log_file_.is_open())
                log_file_ << "[" << TimeStamp().to_str() << "] [ Error ] --> " << msg << std::endl;
        }

    };

private:
    std::map<std::string, std::shared_ptr<Logger>> loggers_;

private:
    LoggerFactory() { }
    ~LoggerFactory() { }

public:
    std::shared_ptr<Logger> create_console(const std::string& name);
    std::shared_ptr<Logger> create_file(const std::string& name, const std::string& file_name);
    std::shared_ptr<Logger> get_logger(const std::string& name);
};

inline std::shared_ptr<Logger> LoggerFactory::create_console(const std::string& name)
{
    auto itr = loggers_.find(name);
    if(itr == loggers_.end())
    {
        loggers_[name] = std::make_shared<ConsoleLogger>(name);
        return loggers_[name];
    }
    else {
        loggers_[name]->warning(str::format("Attempt to create a logger with name '{}', which is already created.", name));
        return loggers_[name];
    }
}

inline std::shared_ptr<Logger> LoggerFactory::create_file(const std::string& name, const std::string& file_name)
{
    auto itr = loggers_.find(name);
    if(itr == loggers_.end())
    {
        loggers_[name] = std::make_shared<FileLogger>(name, file_name);
        return loggers_[name];
    }
    else {
        loggers_[name]->warning(str::format("Attempt to create a logger with name '{}', which is already created.", name));
        return loggers_[name];
    }
}

inline std::shared_ptr<Logger> LoggerFactory::get_logger(const std::string& name)
{
    auto itr = loggers_.find(name);
    if(itr == loggers_.end())
    {
        throw std::runtime_error(str::format("LoggerFactory::get_logger({}) : no such a looger exist.", name));
    }
    else
        return loggers_[name];
}

#define Log_Info(logger, msg) \
    logger->info(str::format("[{}:{}]: {}", __SHORT_FILE__, __LINE__, msg)); \

#define Log_Success(logger, msg) \
    logger->success(str::format("[{}:{}]: {}", __SHORT_FILE__, __LINE__, msg)); \

#define Log_Warning(logger, msg) \
        logger->warning(str::format("[{}:{}]: {}", __SHORT_FILE__, __LINE__, msg)); \

#define Log_Error(logger, msg) \
    logger->error(str::format("[{}:{}]: {}", __SHORT_FILE__, __LINE__, msg)); \

#endif // !LOGGER_HPP