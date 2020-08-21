#ifndef VON_NEUMANN_ERROR_REPORTER_HPP
#define VON_NEUMANN_ERROR_REPORTER_HPP

#include "token.hpp"

#include <string>
#include <vector>

namespace vnm
{

class error_reporter
{
public:
    explicit error_reporter( const std::string& source );

    void report( const std::string& error_message, const token& invalid_token );
    void print_errors() const;
    auto has_errors() -> bool;
    auto count() -> unsigned int;

private:
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

    auto get_line_containing_token( const token& invalid_token ) -> std::string;

    const std::string& source_;
    std::vector<error> errors_;
};

} // namespace vnm

#endif // VON_NEUMANN_ERROR_REPORTER_HPP
