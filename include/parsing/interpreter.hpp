#ifndef VON_NEUMANN_INTERPRETER_HPP
#define VON_NEUMANN_INTERPRETER_HPP

#include "error_reporter.hpp"
#include "machine.hpp"

namespace vnm
{

class interpreter
{
public:
    explicit interpreter( std::istream& );

    [[nodiscard]] auto interpret() -> machine::mem_t;

private:
    auto interpret_line( const std::vector<token>& line ) -> word;
    void check_for_errors();

    std::vector<std::vector<token>> tokens_;
    error_reporter errors_;
};

} // namespace vnm

#endif
