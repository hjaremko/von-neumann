#ifndef VON_NEUMANN_PRINTER_HPP
#define VON_NEUMANN_PRINTER_HPP

#include "binary.hpp"
#include "machine.hpp"
#include "normal.hpp"
#include "printer_interface.hpp"
#include "with_sign.hpp"
#include "word.hpp"

#include <fmt/ostream.h>

namespace vnm
{

template <typename word_formatter>
class printer : public printer_interface
{
public:
    printer( std::ostream& os, const machine& machine )
        : os_( os ), machine_( machine )
    {
    }

    void print_registers_table() const override
    {
        // todo: dynamic
        fmt::print( os_,
                    "--------------------------------------------------\n"
                    "|         IR          |    |    |    |           |\n"
                    "----------------------| PC | OR | AC |   next    |\n"
                    "| code  | mode | arg  |    |    |    |           |\n"
                    "--------------------------------------------------\n" );
    }

    void print_registers() const override
    {
        const auto& ir { machine_.instruction_reg };
        const auto& next { machine_.ram[ machine_.program_counter ] };

        fmt::print( os_,
                    "| {:<6}| {:<5}| {:<5}| {:<3}| {:<3}| {:<3}| {:<10}|\n"
                    "--------------------------------------------------\n",
                    ir.is_instruction() ? instruction_to_str( ir.get_code() )
                                        : " ",
                    mode_to_str( ir.get_mode() ),
                    word_formatter::format( ir.get_arg() ),
                    *machine_.program_counter,
                    *machine_.operand_reg,
                    *machine_.accumulator,
                    word_formatter::format( next ) );
    }

    void print_memory() const override
    {
        const auto print_bar = [ this ]() {
            fmt::print(
                os_, "--------------------------------------------------\n" );
        };

        print_bar();

        for ( word::type i = 0; i <= get_size(); ++i )
        {
            print_memory_cell( i );
        }

        print_bar();
    }

private:
    void print_memory_cell( word::type i ) const
    {
        fmt::print( os_,
                    "[ {:<3}]: {}\n",
                    i,
                    word_formatter::format( machine_.ram[ i ] ) );
    }

    [[nodiscard]] auto get_size() const -> word::type
    {
        for ( auto i = machine::memory_size - 1; i >= 0; --i )
        {
            if ( machine_.ram[ i ] != 0 )
            {
                return i + 2;
            }
        }

        return machine::memory_size - 1;
    }

    std::ostream& os_;
    const machine& machine_;
};

} // namespace vnm

#endif // VON_NEUMANN_PRINTER_HPP
