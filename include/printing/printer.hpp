#ifndef VON_NEUMANN_PRINTER_HPP
#define VON_NEUMANN_PRINTER_HPP

#include "binary.hpp"
#include "machine.hpp"
#include "normal.hpp"
#include "printer_interface.hpp"
#include "with_sign.hpp"
#include "word.hpp"

namespace vnm
{

template <typename word_printer>
class printer : public printer_interface
{
public:
    printer( std::ostream& os, const machine& machine )
        : os_( os ), machine_( machine )
    {
    }

    void print_registers_table() const override
    {
        os_ << "--------------------------------------------------\n";
        os_ << "|         IR          |    |    |    |           |\n";
        os_ << "----------------------| PC | OR | AC |   next    |\n";
        os_ << "| code  | mode | arg  |    |    |    |           |\n";
        os_ << "--------------------------------------------------\n";
    }

    void print_registers() const override
    {
        os_ << std::left;
        os_ << "| " << std::setw( 6 );

        const auto& ir { machine_.instruction_reg };
        const auto& next { machine_.ram[ machine_.program_counter ] };

        os_ << ( ir.is_instruction() ? instructions_to_str.at( ir.get_code() )
                                     : " " );
        os_ << "|  " << std::setw( 4 ) << mode_to_str.at( ir.get_mode() );
        os_ << "| " << std::setw( 5 );
        word_printer::print_word( os_, ir.get_arg() );
        os_ << "| " << std::setw( 3 ) << *machine_.program_counter;
        os_ << "| " << std::setw( 3 ) << *machine_.operand_reg;
        os_ << "| " << std::setw( 3 ) << *machine_.accumulator;
        os_ << "| " << std::setw( 10 );
        word_printer::print_word( os_, next );
        os_ << "|\n";
        os_ << "--------------------------------------------------\n";
    }

    void print_memory() const override
    {
        os_ << "--------------------------------------------------\n";

        for ( word::type i = 0; i <= get_size(); ++i )
        {
            print_memory_cell( i );
        }

        os_ << "--------------------------------------------------\n";
    }

private:
    void print_memory_cell( word::type i ) const
    {
        os_ << "[ " << std::left << std::setw( 3 ) << i << " ]: ";
        word_printer::print_word( os_, machine_.ram[ i ] );
        os_ << std::endl;
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
