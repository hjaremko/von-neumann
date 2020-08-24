#include "word.hpp"

#include <sstream>

auto vnm::operator>>( std::istream& os, vnm::word& rhs ) -> std::istream&
{
    auto line { std::string {} };
    auto code { std::string {} };
    auto mode { std::string {} };
    auto args { vnm::word::type {} };

    std::getline( os, line, ';' );
    auto ss { std::stringstream( line ) };

    ss >> code >> mode >> args;

    rhs = vnm::word { code, mode, args };
    return os;
}

auto vnm::operator==( const vnm::word& lhs, const vnm::word& rhs ) -> bool
{
    return lhs.get() == rhs.get() &&
           lhs.is_instruction() == rhs.is_instruction();
}
