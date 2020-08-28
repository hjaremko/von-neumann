#ifndef VON_NEUMANN_SIGNED_HPP
#define VON_NEUMANN_SIGNED_HPP

#include "word.hpp"

#include <string>

namespace vnm::format_policy
{

class with_sign
{
public:
    static auto format( const word& rhs ) -> std::string;

private:
    struct s
    {
        int val : 9;
    };
};

} // namespace vnm::format_policy

#endif
