
#ifdef _WIN32

#elif __APPLE__
//return MAC;
#elif __linux
#include <unistd.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#elif __ANDROID__
//return ANDROID;
#endif

#include <cstdlib>
#include <cstring>
#include <utility>
#include <exception>

#include "string_utility.hpp"

namespace net
{
    enum IP_Type
    {
        IP4, 
        IP6
    };

#ifdef _WIN32
    inline std::pair<bool, std::string> get_ip_address(const std::string& interface_name, IP_Type ip_type)
    {
		return std::pair{false, ""};
    }
#elif __APPLE__
//return MAC;
#elif __linux
    inline std::pair<bool, std::string> get_ip_address(const std::string& interface_name, IP_Type ip_type)
    {
        struct ifaddrs * ifAddrStruct=NULL;
        struct ifaddrs * ifa=NULL;
        void * tmpAddrPtr=NULL;
        auto result = std::pair{false, std::string()};

        getifaddrs(&ifAddrStruct);

        for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) 
        {
            if (!ifa->ifa_addr) {
                continue;
            }
                
            if(ip_type == net::IP4 && ifa->ifa_addr->sa_family == AF_INET)
            {
                tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
                char addressBuffer[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
                if(interface_name == std::string(ifa->ifa_name))
                {
                    result = std::pair{true, std::string(addressBuffer)};
                    break;
                }
            }
            else if(ip_type == net::IP6 && ifa->ifa_addr->sa_family == AF_INET6)
            { // check it is IP6
                tmpAddrPtr=&((struct sockaddr_in6 *)ifa->ifa_addr)->sin6_addr;
                char addressBuffer[INET6_ADDRSTRLEN];
                inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);
                if(interface_name == std::string(ifa->ifa_name)) {
                    result = std::pair{true, std::string(addressBuffer)};
                    break;
                } 
            }
        }
        if (ifAddrStruct!=NULL) 
            freeifaddrs(ifAddrStruct);
        
        return result;
    }
#elif __ANDROID__
//return ANDROID;
#endif

} // namespace net