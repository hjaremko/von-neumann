#include "error_reporter.hpp"

#include <iostream>
#include <sstream>

namespace
{

auto get_token_column( const token& invalid_token, const std::string& line ) -> unsigned int
{
    auto token_pos = line.find( invalid_token.lexeme );

    if ( token_pos == std::string::npos )
    {
        return line.length() - 1;
    }

    return token_pos + 1;
}

} // namespace

vnm::error_reporter::error_reporter( const std::string& source ) : source_( source )
{
}

void vnm::error_reporter::report( const std::string& error_message, const token& invalid_token )
{
    auto line = get_line_containing_token( invalid_token );
    auto column = get_token_column( invalid_token, line );
    errors_.push_back( error { invalid_token, column, line, error_message } );
}

void vnm::error_reporter::print_errors() const
{
    for ( const auto& error : errors_ )
    {
        std::cout << error.make_error_message() << std::endl;
    }
}

auto vnm::error_reporter::has_errors() -> bool
{
    return !errors_.empty();
}

auto vnm::error_reporter::count() -> unsigned int
{
    return errors_.size();
}

auto vnm::error_reporter::get_line_containing_token( const token& invalid_token ) -> std::string
{
    auto source_ss { std::stringstream { source_ } };
    auto line { std::string {} };

    for ( int i = 0; i < invalid_token.line; ++i )
    {
        std::getline( source_ss, line );
    }

    return line;
}

auto vnm::error_reporter::error::make_error_message() const -> std::string
{
    auto ss { std::stringstream {} };

    ss << invalid_token.line << ":" << column << ": " << message << std::endl;
    ss << "\t" << line << std::endl;
    ss << "\t" << make_spaces( column - 1 ) << "^" << make_tildes();

    return ss.str();
}

auto vnm::error_reporter::error::make_tildes() const -> std::string
{
    return std::string( invalid_token.lexeme.length() != 0 ? invalid_token.lexeme.length() - 1 : 0U,
                        '~' );
}

auto vnm::error_reporter::error::make_spaces( unsigned int amount ) -> std::string
{
    return std::string( amount, ' ' );
}
