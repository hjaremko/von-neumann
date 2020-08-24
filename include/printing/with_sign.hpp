#ifndef VON_NEUMANN_SIGNED_HPP
#define VON_NEUMANN_SIGNED_HPP

#include "word.hpp"

namespace vnm::print_policy
{

class with_sign
{
public:
    static void print_word( std::ostream& os_, const word& rhs );

private:
    struct s
    {
        int val : 9;
    };
};

} // namespace vnm::print_policy

#endif
