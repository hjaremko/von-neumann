#ifndef VON_NEUMANN_ERROR_REPORTER_HPP
#define VON_NEUMANN_ERROR_REPORTER_HPP

#include "parsing/error.hpp"
#include "token.hpp"

#include <vector>

namespace vnm
{

class error_reporter
{
public:
    error_reporter() = default;
    explicit error_reporter( std::string source );

    error_reporter( const error_reporter& ) = default;
    error_reporter( error_reporter&& ) = default;
    auto operator=( const error_reporter& ) -> error_reporter& = default;
    auto operator=( error_reporter&& ) -> error_reporter& = default;
    ~error_reporter() = default;

    void report( const std::string& error_message, const token& invalid_token );
    void report( error&& );
    void print_errors() const;
    void clear();
    [[nodiscard]] auto has_errors() const -> bool;
    [[nodiscard]] auto count() const -> unsigned int;

private:
//    auto get_line_containing_token( const token& invalid_token ) -> std::string;

    std::string source_;
    std::vector<error> errors_;
};

} // namespace vnm

#endif // VON_NEUMANN_ERROR_REPORTER_HPP
