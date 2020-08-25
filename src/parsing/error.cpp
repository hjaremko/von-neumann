#include "parsing/error.hpp"

#include <sstream>

auto vnm::error::make_error_message() const -> std::string
{
    auto ss { std::stringstream {} };

    ss << invalid_token.line << ":" << column << ": " << message << std::endl;
    ss << "\t" << line << std::endl;
    ss << "\t" << make_spaces( column - 1 ) << "^" << make_tildes();

    return ss.str();
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
