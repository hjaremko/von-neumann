#include "machine.hpp"

#include <ostream>

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

word machine::get_ir() const
{
    return instruction_reg_;
}

word machine::get_pc() const
{
    return program_counter_;
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
            set_or( memory_.at( instruction_reg_.get_arg() ) );
            break;
        }
        case mode::indirect:
        {
            set_or( memory_.at( memory_.at( instruction_reg_.get_arg() ) ) );
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
            if ( *get_ac() == 0 )
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
            if ( *get_ac() == *get_or() )
            {
                set_ac( word { static_cast<word::type>( -1 ) } );
            }
            else
            {
                set_ac( word { 0 } );
            }

            break;
        }
        case instruction::SHZ:
        {
            if ( *get_or() < 0 )
            {
                set_ac( word( *get_ac() >> *get_or() ) );
            }
            else if ( *get_or() > 0 )
            {
                set_ac( word( *get_ac() << *get_or() ) );
            }

            break;
        }
        case instruction::SHC:
        {
            if ( get_or().is_arg_negative() )
            {
                set_ac( word( *get_ac() << *get_or() | *get_or() >> ( 16u - *get_or() ) ) );
            }
            else
            {
                set_ac( word( ( *get_ac() >> *get_or() | *get_or() << ( 16u - *get_or() ) ) ) );
            }

            break;
        }
        default:
            return false;
        }
    }

    return true;
}

void machine::put_to_memory( const word& value, const word& register_ )
{
    memory_.set( value, register_ );
}

void machine::set_memory( mem_t mem )
{
    memory_ = std::move( mem );
}

machine::mem_t machine::get_memory() const
{
    return memory_;
}

} // namespace vnm
