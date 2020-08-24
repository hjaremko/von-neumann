#ifndef VON_NEUMANN_BINARY_HPP
#define VON_NEUMANN_BINARY_HPP

#include "word.hpp"

#include <ostream>

namespace vnm::print_policy
{

class binary
{
public:
    static void print_word( std::ostream& os_, const word& rhs );
};

} // namespace vnm::print_policy

#endif
