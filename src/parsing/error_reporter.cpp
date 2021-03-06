#include "parsing/error_reporter.hpp"

#include <fmt/core.h>
#include <sstream>

namespace
{

auto get_token_column( const token& invalid_token, const std::string& line )
    -> unsigned int
{
    auto token_pos = line.find( invalid_token.lexeme );
    return token_pos == std::string::npos ? line.length() - 1 : token_pos + 1;
}

} // namespace

vnm::error_reporter::error_reporter( std::string source )
    : source_( std::move( source ) )
{
}

void vnm::error_reporter::report( const std::string& error_message,
                                  const token& invalid_token )
{
    auto line = get_line_containing_token( invalid_token );
    auto column = get_token_column( invalid_token, line );
    errors_.push_back( error { invalid_token, column, line, error_message } );
}

void vnm::error_reporter::print_errors() const
{
    for ( const auto& error : errors_ )
    {
        fmt::print( "{}\n", error.make_error_message() );
    }
}

auto vnm::error_reporter::has_errors() const -> bool
{
    return !errors_.empty();
}

auto vnm::error_reporter::count() const -> unsigned int
{
    return errors_.size();
}

auto vnm::error_reporter::get_line_containing_token(
    const token& invalid_token ) -> std::string
{
    auto source_ss { std::stringstream { std::string { source_ } } };
    auto line { std::string {} };

    for ( int i = 0; i < invalid_token.line; ++i )
    {
        std::getline( source_ss, line );
    }

    return line;
}

void vnm::error_reporter::clear()
{
    errors_.clear();
}
