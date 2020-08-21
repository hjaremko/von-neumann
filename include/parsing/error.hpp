#ifndef VON_NEUMANN_ERROR_HPP
#define VON_NEUMANN_ERROR_HPP

#include "token.hpp"

#include <string>

namespace vnm
{

struct error
{
    token invalid_token;
    unsigned int column { 0 };
    std::string line { 0 };
    std::string message;

    [[nodiscard]] auto make_error_message() const -> std::string;
    [[nodiscard]] auto make_tildes() const -> std::string;
    static auto make_spaces( unsigned int amount ) -> std::string;
};

} // namespace vnm

#endif // VON_NEUMANN_ERROR_HPP
