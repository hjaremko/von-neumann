#include "machine.hpp"

#include <ostream>
#include <string>

namespace vnm
{

void machine::set_pc( const word& value )
{
    program_counter_ = value;
}

void machine::set_or( const word& value )
{
    operand_reg_ = value;
}

void machine::set_ac( const word& value )
{
    accumulator_ = value;
}

word machine::get_or() const
{
    return operand_reg_;
}

word machine::get_ac() const
{
    return accumulator_;
}

bool machine::execute()
{
    if ( instruction_reg_.is_instruction() )
    {
        switch ( instruction_reg_.get_mode() )
        {
        case mode::instant:
        {
            set_or( instruction_reg_.get_arg() );
            break;
        }
        case mode::direct:
        {
            set_or( memory_.get( instruction_reg_.get_arg() ) );
            break;
        }
        case mode::indirect:
        {
            set_or( memory_.get( memory_.get( instruction_reg_.get_arg() ) ) );
            break;
        }
        case mode::index:
        {
            set_or( get_ac() + instruction_reg_.get_arg() );
            break;
        }
        }

        switch ( instruction_reg_.get_code() )
        {
        case instruction::STOP:
        {
            return false;
        }
        case instruction::LOAD:
        {
            set_ac( get_or() );
            break;
        }
        case instruction::STORE:
        {
            memory_.set( get_ac(), get_or() );
            break;
        }
        case instruction::JUMP:
        {
            program_counter_ = get_or();
            break;
        }
        case instruction::JNEG:
        {
            if ( get_ac().is_arg_negative() )
            {
                program_counter_ = get_or();
            }

            break;
        }
        case instruction::JZERO:
        {
            if ( get_ac().get() == 0 )
            {
                program_counter_ = get_or();
            }

            break;
        }
        case instruction::ADD:
        {
            set_ac( get_ac() + get_or() );
            break;
        }
        case instruction::SUB:
        {
            set_ac( get_ac() - get_or() );
            break;
        }
        case instruction::MULT:
        {
            set_ac( get_ac() * get_or() );
            break;
        }
        case instruction::DIV:
        {
            set_ac( get_ac() / get_or() );
            break;
        }
        case instruction::AND:
        {
            set_ac( get_ac() & get_or() );
            break;
        }
        case instruction::OR:
        {
            set_ac( get_ac() | get_or() );
            break;
        }
        case instruction::NOT:
        {
            set_ac( !get_or() );
            break;
        }
        case instruction::CMP:
        {
            if ( get_ac() == get_or() )
            {
                set_ac( word{ -1 } );
            }
            else
            {
                set_ac( word{ 0 } );
            }

            break;
        }
        case instruction::SHZ:
        {
            if ( get_or().get() < 0 )
            {
                set_ac( word( get_ac().get() >> std::abs( get_or().get() ) ) );
            }
            else if ( get_or().get() > 0 )
            {
                set_ac( word( get_ac().get() << std::abs( get_or().get() ) ) );
            }

            break;
        }
        case instruction::SHC:
        {
            if ( get_or().is_arg_negative() )
            {
                set_ac( word{ static_cast<word::type>( get_ac().get() << std::abs( get_or().get_complete_arg() ) |
                              std::abs( get_or().get_complete_arg() ) >>
                              ( 16 - std::abs( get_or().get_complete_arg() ) ) ) } );
            }
            else
            {
                set_ac(
                    word{ static_cast<word::type>( get_ac().get() >> get_or().get_complete_arg() |
                          get_or().get_complete_arg() << ( 16 - get_or().get_complete_arg() ) ) } );
            }

            break;
        }
        }
    }

    return true;
}

void machine::put_to_memory( const word& value, const word& register_ )
{
    memory_.set( value, register_ );
}

void machine::set_memory( memory mem )
{
    memory_ = std::move( mem );
}

memory machine::get_memory() const
{
    return memory_;
}

void machine::print_registers_table( std::ostream& os ) const
{
    os << "--------------------------------------------------" << std::endl;
    os << "|         IR          |    |    |    |           |" << std::endl;
    os << "----------------------| PC | OR | AC |   next    |" << std::endl;
    os << "| code  | mode | arg  |    |    |    |           |" << std::endl;
    os << "--------------------------------------------------" << std::endl;
}

void machine::print_registers( std::ostream& os ) const
{
    os << std::left;
    os << "| " << std::setw( 6 );

    if ( instruction_reg_.is_instruction() )
        os << instructions_to_str.at( instruction_reg_.get_code() );
    else
        os << ' ';

    os << "|  " << std::setw( 4 ) << mode_to_str.at( instruction_reg_.get_mode() );
    os << "| " << std::setw( 5 ) << instruction_reg_.get_complete_arg();
    os << "| " << std::setw( 3 ) << program_counter_.get();
    os << "| " << std::setw( 3 ) << operand_reg_.get();
    os << "| " << std::setw( 3 ) << accumulator_.get();
    os << "| " << std::setw( 10 ) << memory_.get( program_counter_ ) << "|" << std::endl;
    os << "--------------------------------------------------" << std::endl;
}

void machine::print_memory( std::ostream& os ) const
{
    os << "--------------------------------------------------" << std::endl;

    for ( int i = 0; i < get_size(); ++i )
    {
        os << "[ " << std::left << std::setw( 3 ) << i << " ]: " << memory_.get( word{ static_cast<word::type>( i ) } )
                   << std::endl;
    }

    os << "--------------------------------------------------" << std::endl;
}

int machine::get_size() const
{
    for ( word::type i = 511; i >= 0; --i )
    {
        if ( memory_.get( word{ i } ).get() != 0 )
        {
            return i + 2;
        }
    }

    return 511;
}

} // namespace vnm
