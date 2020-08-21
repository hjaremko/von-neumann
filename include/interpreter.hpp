#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "error_reporter.hpp"
#include "machine.hpp"
#include "token.hpp"

namespace vnm
{

class interpreter
{
public:
    explicit interpreter( std::istream& );

    [[nodiscard]] auto interpret() -> machine::mem_t;

private:
    class scanner
    {
    public:
        explicit scanner( std::string, error_reporter& );

        [[nodiscard]] auto at_end() const -> bool;
        void scan_token();
        auto scan_tokens() -> std::vector<token>;

    private:
        auto advance() -> char;
        [[nodiscard]] auto peek() const -> char;
        void add_token( token::type );
        void add_token( token::type, int );
        void number();
        void string();

        std::string source_;
        error_reporter& errors_;
        std::vector<token> tokens_;
        unsigned start_ { 0 };
        unsigned current_ { 0 };
        unsigned line_ { 1 };
    };

    [[nodiscard]] auto read_input() const -> std::string;

    std::istream& input_stream_;
};

} // namespace vnm

#endif
