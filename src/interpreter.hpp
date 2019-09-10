#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "machine.hpp"
#include "token.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <streambuf>
#include <string>

using namespace std::string_literals;

namespace vnm
{

class interpreter
{
public:
    explicit interpreter( const std::string& );
    ~interpreter();

    void interpret( machine& );

private:
    class scanner
    {
    public:
        explicit scanner( std::string );

        [[nodiscard]] bool at_end() const;
        void scan_token();
        std::vector<token> scan_tokens();

    private:
        char advance();
        [[nodiscard]] char peek() const;
        void add_token( token::type );
        void add_token( token::type, int );
        void number();
        void string();

        std::string m_source;
        std::vector<token> m_tokens;
        unsigned m_start{ 0 };
        unsigned m_current{ 0 };
        unsigned m_line{ 1 };
    };

    static void error( const std::string&, const token&, const std::string& );

    static unsigned m_error_count;
    std::fstream m_file;
};

} // namespace vnm

#endif
