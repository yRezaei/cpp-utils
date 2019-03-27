#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP

#include <string>
#include <sstream>
#include <vector>
#include <array>
#include <memory>
#include <map>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <type_traits>
#include <tarja/core/file_utility.hpp>

#include "string_utility.hpp"
#include "logger.hpp"

class ConfigParser
{
private:
    std::map< std::string, std::string > data_;
    fs::path file_path_;
    bool is_changed;
    std::shared_ptr<Logger> logger_;

public:
    ConfigParser(int argc, char** argv): is_changed(false)
    {
        logger_ = LoggerFactory::instance().get_logger("logger");
    }

    ConfigParser(const std::string& file_path): is_changed(false)
    {
        logger_ = LoggerFactory::instance().get_logger("logger");
        file_path_ = fs::path(file_path);
        load_from_file();
    }

    ~ConfigParser()
    {
        if(is_changed)
            save_to_file();
    }

    std::string to_string()
    {
        std::stringstream stream;
        for(auto& m : data_)
            stream << m.first << "=" << m.second << std::endl;
        return stream.str();
    }

    template<typename T>
    void init(const std::string& key, T& value);

    template<typename T>
    void init(const std::string& tag, std::vector<T>& value);

    template<typename T, std::size_t S>
    void init(const std::string& tag, std::array<T, S>& value);

    template<typename T1, typename T2>
    void init(const std::string& tag, std::map<T1, T2>& var);

    template<typename T>
    void set(const std::string &key, const T& value);

    template<typename T>
    void set(const std::string& tag, const std::vector<T>& value);

    template<typename T, std::size_t S>
    void set(const std::string& tag, const std::array<T, S>& value);

    template<typename Key, typename Value>
    void set(const std::string& tag, const std::map<Key, Value>& var);

private:
    void save_to_file()
    {
        std::ofstream conf_file(file_path_);

        if(conf_file.is_open())
        {
            for(auto& m : data_)
                conf_file << m.first << "=" << m.second << std::endl;
        }
    }

    template<typename T>
    std::string type_to_string(T v );

    template<typename T>
    T string_to_type(const std::string& v );

    void load_from_file()
    {
        std::ifstream conf_file(file_path_);
        std::string line;

        if(conf_file.is_open())
        {
            while(std::getline(conf_file, line))
            {
                line.erase(std::remove_if(line.begin(),
                              line.end(), [](unsigned char x){return std::isspace(x);}), line.end());

                if(line.find("#") != std::string::npos)
                    continue;

                auto tokkens = str::split(line, "=", false);
                if(tokkens.size() != 2)
                    continue;

                data_[tokkens[0]] = tokkens[1];
            }
            conf_file.close();
        }
        else
            logger_->error(str::format("ConfigParser::load_from_file() : file {} not found.", file_path_));
    }

};

template<typename T>
inline std::string ConfigParser::type_to_string(T v )
{
    std::stringstream stream;
    stream << v;
    return stream.str();
}

template<>
inline std::string ConfigParser::type_to_string<bool>(bool v )
{
    return (v ? "TRUE" : "FALSE");
}

template<>
inline std::string ConfigParser::type_to_string<const char*>(const char* v )
{
    std::stringstream stream;
    stream << v;
    return stream.str();
}

template<>
inline std::string ConfigParser::type_to_string<char>(char v )
{
    return std::string(&v, &v+1);
}

template<>
inline std::string ConfigParser::type_to_string<unsigned char>(unsigned char v )
{
    return std::string(&v, &v+1);
}

template<>
inline std::string ConfigParser::type_to_string<std::string>(std::string v )
{
    return v;
}

template<>
inline std::string ConfigParser::type_to_string<std::int16_t>(std::int16_t v )
{
    return std::to_string(v);
}

template<>
inline std::string ConfigParser::type_to_string<std::uint16_t>(std::uint16_t v )
{
    return std::to_string(v);
}

template<>
inline std::string ConfigParser::type_to_string<std::int32_t>(std::int32_t v )
{
    return std::to_string(v);
}

template<>
inline std::string ConfigParser::type_to_string<std::uint32_t>(std::uint32_t v )
{
    return std::to_string(v);
}

template<>
inline std::string ConfigParser::type_to_string<std::int64_t>(std::int64_t v )
{
    return std::to_string(v);
}

template<>
inline std::string ConfigParser::type_to_string<std::uint64_t>(std::uint64_t v )
{
    return std::to_string(v);
}

template<>
inline std::string ConfigParser::type_to_string<float>(float v )
{
    return std::to_string(v);
}

template<>
inline std::string ConfigParser::type_to_string<double>(double v )
{
            return std::to_string(v);
}

template<>
inline std::string ConfigParser::type_to_string<long double>(long double v )
{
    return std::to_string(v);
}


template<typename T>
inline T ConfigParser::string_to_type(const std::string& v )
{
    T t;
    std::stringstream stream(v);
    stream >> t;
    return t;
}

template<>
inline bool ConfigParser::string_to_type<bool>(const std::string& v )
{
    return (v =="TRUE" ? true : false);
}

template<>
inline std::string ConfigParser::string_to_type<std::string>(const std::string& v )
{
    return v;
}

template<typename T>
inline void ConfigParser::init(const std::string& tag, T& value)
{
    if(data_.find(tag) != data_.end())
    {
        value = string_to_type<T>(data_[tag]);
    }
}

template<typename T>
inline void ConfigParser::init(const std::string& tag, std::vector<T>& value)
{
    if(data_.find(tag) != data_.end())
    {
        auto tmp_str = data_[tag];
        auto open_bracket_count = std::count(tmp_str.begin(), tmp_str.end(), '{');
        auto close_bracket_count = std::count(tmp_str.begin(), tmp_str.end(), '}');
        if(open_bracket_count == close_bracket_count && open_bracket_count == 1)
        {
            auto tokkens = str::split(tmp_str, "{, }", false);
            if(value.size() == tokkens.size())
            {
                for(auto i = 0; i < value.size(); i++)
                    value[i]= string_to_type<T>(tokkens[i]);
            }
        }
    }
}

template<typename T, std::size_t S>
inline void ConfigParser::init(const std::string& tag, std::array<T, S>& value)
{
    if(data_.find(tag) != data_.end())
    {
        auto tokkens = str::split(data_[tag], "{, }", false);
        if(value.size() == tokkens.size())
        {
            for(auto i = 0; i < value.size(); i++)
                value[i] = string_to_type<T>(tokkens[i]);
        }
    }
}

template<typename T1, typename T2>
inline void ConfigParser::init(const std::string& tag, std::map<T1, T2>& var)
{
    if(data_.find(tag) != data_.end())
    {
        auto& tmp_str = data_[tag];
        auto open_bracket_count = std::count(tmp_str.begin(), tmp_str.end(), '{');
        auto close_bracket_count = std::count(tmp_str.begin(), tmp_str.end(), '}');
        if((open_bracket_count == close_bracket_count) && (open_bracket_count + close_bracket_count > 2))
        {
            auto tokkens = str::split(tmp_str, "{,}", false);
            if(tokkens.size() % 2 == 0) 
            {
                for(auto i = 0u; i < tokkens.size(); i += 2)
                    var[tokkens[i]] = string_to_type<T2>(tokkens[i+1]);
            }
            else
                logger_->error("ConfigParser::init(const std::string& tag, std::map<Key, Value>& var) : There is not enough {key, value} combination.");
        }
        else
            logger_->error("ConfigParser::init(const std::string& tag, std::map<Key, Value>& var) : data must containe dictionary format.");
    }
    else
        logger_->error(str::format("ConfigParser::init(const std::string& tag, std::map<Key, Value>& var) : no tag exist with name '{}'.", tag));
}


template<typename T>
inline void ConfigParser::set(const std::string& key, const T& value)
{
    data_[key] = type_to_string<T>(value);
    is_changed = true;
}


template<typename T>
inline void ConfigParser::set(const std::string& key, const std::vector<T>& value)
{
    if(!value.empty())
    {
        std::stringstream stream;
        for(auto i =0; i < value.size()-1; i++)
            stream << type_to_string<T>(value[i]) << ",";
        stream << type_to_string<T>(value[value.size()-1]);
        data_[key] = stream.str();
    }
    else
        data_[key] = "";

    is_changed = true;
}

template<typename T, std::size_t S>
inline void ConfigParser::set(const std::string& key, const std::array<T, S>& value)
{
    if(!value.empty())
    {
        std::stringstream stream;
        for(auto i =0; i < value.size()-1; i++)
            stream << type_to_string<T>(value[i]) << ",";
        stream << type_to_string<T>(value[value.size()-1]);
        data_[key] = stream.str();
    }
    else
        data_[key] = "";
    is_changed = true;
}

template<typename T1, typename T2>
void ConfigParser::set(const std::string& name, const std::map<T1, T2>& var)
{
    if(!var.empty())
    {
        std::stringstream stream("{");
        std::string separator;
        for(auto const& [key, value] : var) {
            stream << separator << "{" << type_to_string<T1>(key) << ", " << type_to_string<T2>(value) << "}";
            separator = ",";
        }
        stream << "}";
        data_[name] = stream.str();
    }
    else
    {
        data_.erase(name);
    }
}


#endif //!CONFIG_PARSER_HPP
