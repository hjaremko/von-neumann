#ifndef VON_NEUMANN_LINE_INTERPRETER_HPP
#define VON_NEUMANN_LINE_INTERPRETER_HPP

#include "parsing/error_reporter.hpp"
#include "parsing/token.hpp"
#include "word.hpp"

#include <optional>

namespace vnm
{

class line_interpreter
{
public:
    explicit line_interpreter( const std::vector<token>& line,
                               error_reporter& errors );
    auto parse_line() -> std::optional<word>;

private:
    auto parse_instruction() -> std::optional<word>;
    auto parse_mode() -> std::optional<word>;
    auto parse_argument() -> std::optional<word>;
    auto parse_number() -> std::optional<word>;
    void check_for_newline( const token& );

    const std::vector<token>& line_;
    error_reporter& errors_;
};

} // namespace vnm
#endif // VON_NEUMANN_LINE_INTERPRETER_HPP
