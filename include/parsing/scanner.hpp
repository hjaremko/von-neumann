#ifndef VON_NEUMANN_SCANNER_HPP
#define VON_NEUMANN_SCANNER_HPP

#include "error_reporter.hpp"
#include "token.hpp"

namespace vnm
{

class scanner
{
public:
    explicit scanner( std::string, error_reporter& );

    auto scan_tokens() -> std::vector<std::vector<token>>;

private:
    void scan_token();
    auto advance() -> char;
    [[nodiscard]] auto peek() const -> char;
    [[nodiscard]] auto at_end() const -> bool;
    [[nodiscard]] auto make_token_string() const -> std::string;
    [[nodiscard]] auto make_token( token::type token_type ) const -> token;
    void add_token( token&& t );
    void add_token( token::type );
    void add_int_token();
    void add_newline_token();
    void ignore_comment();
    void parse_token( char c );
    void parse_number();
    void parse_string();

    std::string source_;
    error_reporter& errors_;
    std::vector<std::vector<token>> tokens_;
    unsigned start_ { 0 };
    unsigned current_ { 0 };
    unsigned line_ { 1 };
};

} // namespace vnm

#endif // VON_NEUMANN_SCANNER_HPP
