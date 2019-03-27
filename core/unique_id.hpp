#ifndef UNIQUE_ID
#define UNIQUE_ID

#include "singleton.hpp"

template <typename T>
class UniqueID
{
private:
    T id_;

public:
	UniqueID() : id_(0)
    {
        static_assert(( std::is_same<std::uint8_t, T>::value  || 
                        std::is_same<std::uint16_t, T>::value || 
                        std::is_same<std::uint32_t, T>::value || 
                        std::is_same<std::uint64_t, T>::value), 
                        "Only onjects that are driven from NetworkMessage is allowd.");
    }
    ~UniqueID() {}

    T next() { return ++id_; }
	T current() { return id_; }
};

#endif //!UNIQUE_ID