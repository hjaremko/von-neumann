#include "word.hpp"

#include <sstream>

std::istream& operator>>( std::istream& os, vnm::word& rhs )
{
    auto line { std::string{} };
    auto code { std::string{} };
    auto mode { std::string{} };
    auto args { vnm::word::type{} };

    std::getline( os, line );
    auto ss { std::stringstream( line ) };
    std::getline( ss, line, ';' );
    ss.str( line );

    ss >> code >> mode >> args;

    rhs = vnm::word{ code, mode, args };
    return os;
}
