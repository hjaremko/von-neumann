#include "parsing/interpreter.hpp"

#include "parsing/line_interpreter.hpp"
#include "parsing/scanner.hpp"

#include <algorithm>
#include <fstream>

namespace
{

auto make_halt_message( unsigned int error_count ) -> std::string
{
    auto ss { std::stringstream {} };
    ss << "Execution halted: " << error_count << " errors";
    return ss.str();
}

auto read_input( std::istream& istream ) -> std::string
{
    istream.exceptions( std::ifstream::failbit | std::ifstream::badbit );
    return { std::istreambuf_iterator<char> { istream },
             std::istreambuf_iterator<char> {} };
}

} // namespace

namespace vnm
{

interpreter::interpreter( std::istream& istream )
{
    auto input { read_input( istream ) };
    errors_ = error_reporter( input );
    tokens_ = scanner( input, errors_ ).scan_tokens();
    check_for_errors();
}

auto interpreter::interpret() -> machine::mem_t
{
    auto ram { machine::mem_t {} };
    auto i { 0 };

    for ( const auto& line : tokens_ )
    {
        ram[ i++ ] =
            line_interpreter { line, errors_ }.parse_line().value_or( 0 );
    }

    check_for_errors();
    return ram;
}

void interpreter::check_for_errors()
{
    if ( errors_.has_errors() )
    {
        auto count = errors_.count();
        errors_.print_errors();
        errors_.clear();
        throw std::runtime_error( make_halt_message( count ) );
    }
}

} // namespace vnm
