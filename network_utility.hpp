#pragma once
#ifndef NETWORK_UTILITY_HPP
#define NETWORK_UTILITY_HPP

#include "networking.hpp"
#include "network_message.hpp"

namespace net
{

    template<typename T>
    inline void add_msg(std::stringstream& stream, const T& net_msg)
    {
        static_assert(std::is_base_of<BaseMessage, T>::value, "Only onjects that are driven from NetworkMessage is allowd.");
        stream << net_msg;
    }

    template<typename T , typename ... Args>
    inline void add_msg(std::stringstream& stream, const T& net_msg, const Args&... args)
    {
        stream << net_msg;
        add_msg(stream, args...);
    }

    template<typename T , typename ... Args>
    inline void send_msg(std::shared_ptr<Client> socket, const T& net_msg, const Args&... args)
    {
        std::stringstream stream;
        add_msg(stream, net_msg, args...);
        socket->send(stream.str());
    }

    template<typename T , typename ... Args>
    inline void send_msg(std::shared_ptr<Server> socket, const std::string& reciver_identity, const T& net_msg, const Args&... args)
    {
        std::stringstream stream;
        add_msg(stream, net_msg, args...);
        socket->send(reciver_identity, stream.str());
    }
}
#endif // !NETWORK_UTILITY_HPP
