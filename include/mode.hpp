#ifndef VON_NEUMANN_MODE_HPP
#define VON_NEUMANN_MODE_HPP

#include <cstdint>

namespace vnm
{

enum class mode : uint16_t
{
    instant = 0b0'0000'00'000000000,  //$
    direct = 0b0'0000'01'000000000,   //@
    indirect = 0b0'0000'10'000000000, //&
    index = 0b0'0000'11'000000000     //+
};

}

#endif // VON_NEUMANN_MODE_HPP
