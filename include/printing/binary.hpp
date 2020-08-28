#ifndef VON_NEUMANN_BINARY_HPP
#define VON_NEUMANN_BINARY_HPP

#include "word.hpp"

#include <string>

namespace vnm::format_policy
{

class binary
{
public:
    static auto format( const word& rhs ) -> std::string;
};

} // namespace vnm::format_policy

#endif
