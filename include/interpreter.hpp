#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "machine.hpp"
#include "token.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <streambuf>
#include <string>
#include <vector>

using namespace std::string_literals;

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
        explicit scanner( std::string );

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
        std::vector<token> tokens_;
        unsigned start_ { 0 };
        unsigned current_ { 0 };
        unsigned line_ { 1 };
    };

    static void error( const std::string&, const token&, const std::string& );

    static inline unsigned error_count_ { 0 };
    std::istream& input_stream_;
};

} // namespace vnm

#endif
