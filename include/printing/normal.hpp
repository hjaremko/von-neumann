#ifndef VON_NEUMANN_NORMAL_HPP
#define VON_NEUMANN_NORMAL_HPP

#include "word.hpp"

#include <string>

namespace vnm::format_policy
{

class normal
{
public:
    static auto format( const word& rhs ) -> std::string;
};

} // namespace vnm::format_policy

#endif // VON_NEUMANN_NORMAL_HPP
