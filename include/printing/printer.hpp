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
        os_ << "--------------------------------------------------"
            << std::endl;
        os_ << "|         IR          |    |    |    |           |"
            << std::endl;
        os_ << "----------------------| PC | OR | AC |   next    |"
            << std::endl;
        os_ << "| code  | mode | arg  |    |    |    |           |"
            << std::endl;
        os_ << "--------------------------------------------------"
            << std::endl;
    }

    void print_registers() const override
    {
        os_ << std::left;
        os_ << "| " << std::setw( 6 );

        const auto& ir { machine_.instruction_reg };
        const auto& ir_code { ir.get_code() };
        const auto& ir_mode { ir.get_mode() };
        const auto& ir_arg { ir.get_arg() };
        const auto& pc { machine_.program_counter };
        const auto& oreg { machine_.operand_reg };
        const auto& ac { machine_.accumulator };
        const auto& next { machine_.ram[ pc ] };

        os_ << ( ir.is_instruction() ? instructions_to_str.at( ir_code )
                                     : " " );

        os_ << "|  " << std::setw( 4 ) << mode_to_str.at( ir_mode );
        os_ << "| " << std::setw( 5 );
        word_printer::print_word( os_, ir_arg );
        os_ << "| " << std::setw( 3 ) << *pc;
        os_ << "| " << std::setw( 3 ) << *oreg;
        os_ << "| " << std::setw( 3 ) << *ac;
        os_ << "| " << std::setw( 10 );
        word_printer::print_word( os_, next );
        os_ << "|" << std::endl;
        os_ << "--------------------------------------------------"
            << std::endl;
    }

    void print_memory() const override
    {
        os_ << "--------------------------------------------------"
            << std::endl;

        word::type i = 0;
        // for ( ; machine_.memory.get( i ) != vnm::stop; ++i )
        for ( ; i < get_size(); ++i )
        {
            print_memory_cell( i );
        }

        print_memory_cell( i );

        os_ << "--------------------------------------------------"
            << std::endl;
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
        for ( auto i = 511; i >= 0; --i )
        {
            if ( machine_.ram[ i ] == vnm::stop )
            {
                return i + 2;
            }
        }

        return 511;
    }

    std::ostream& os_;
    const machine& machine_;
};

} // namespace vnm

#endif // VON_NEUMANN_PRINTER_HPP
