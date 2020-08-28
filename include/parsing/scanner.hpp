#ifndef VON_NEUMANN_SCANNER_HPP
#define VON_NEUMANN_SCANNER_HPP

#include "error_reporter.hpp"
#include "token.hpp"

#include <variant>

namespace vnm
{

class scanner
{
public:
    explicit scanner( std::string, error_reporter& );

    auto scan_tokens() -> std::vector<std::vector<token>>;

private:
    enum class parse_result
    {
        ok,
        error,
    };

    auto scan_token() -> std::variant<token, error>;
    auto advance() -> char;
    [[nodiscard]] auto peek() const -> char;
    [[nodiscard]] auto at_end() const -> bool;
    [[nodiscard]] auto make_token_string() const -> std::string;
    [[nodiscard]] auto make_token( token::type token_type ) const -> token;
    void add_token( token&& t );
    auto make_int_token() -> token;
    //    void add_newline_token();
    auto ignore_comment() -> char;
    auto parse_token( char c ) -> std::variant<token, error>;
    auto parse_number() -> token;
    auto parse_string() -> std::variant<token, error>;

    std::string source_;
    error_reporter& errors_;
    std::vector<std::vector<token>> tokens_;
    unsigned start_ { 0 };
    unsigned current_ { 0 };
    unsigned line_ { 1 };
};

} // namespace vnm

#endif // VON_NEUMANN_SCANNER_HPP
