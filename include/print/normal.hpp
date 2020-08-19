#ifndef VON_NEUMANN_NORMAL_HPP
#define VON_NEUMANN_NORMAL_HPP

#include "word.hpp"

#include <ostream>

namespace vnm::print_policy
{

class normal
{
public:
    static void print_word( std::ostream& os_, const word& rhs );
};

} // namespace vnm::print_policy

#endif // VON_NEUMANN_NORMAL_HPP
