#include "parsing/error.hpp"

#include <fmt/format.h>

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
