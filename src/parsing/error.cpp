#include "parsing/error.hpp"

#include <fmt/format.h>

namespace
{

auto get_line_containing_token( const token& invalid_token,
                                std::string_view source ) -> std::string
{
    auto source_ss { std::stringstream { std::string { source } } };
    auto line { std::string {} };

    for ( int i = 0; i < invalid_token.line; ++i )
    {
        std::getline( source_ss, line );
    }

    return line;
}

auto get_token_column( const token& invalid_token, const std::string& line )
    -> unsigned int
{
    auto token_pos = line.find( invalid_token.lexeme );
    return token_pos == std::string::npos ? line.length() - 1 : token_pos + 1;
}

} // namespace

vnm::error::error( token invalid_token,
                   std::string error_message,
                   std::string_view source )
    : invalid_token( std::move( invalid_token ) ),
      column( get_token_column( invalid_token, line ) ),
      line( get_line_containing_token( invalid_token, source ) ),
      message( std::move( error_message ) )
{
}

auto vnm::error::make_error_message() const -> std::string
{
    return fmt::format( "{}:{}: {}\n"
                        "\t{}\n"
                        "\t{}^{}",
                        invalid_token.line,
                        column,
                        message,
                        line,
                        make_spaces( column - 1 ),
                        make_tildes() );
}

auto vnm::error::make_tildes() const -> std::string
{
    return std::string(
        !invalid_token.lexeme.empty() ? invalid_token.lexeme.length() - 1 : 0U,
        '~' );
}

auto vnm::error::make_spaces( unsigned int amount ) -> std::string
{
    return std::string( amount, ' ' );
}
