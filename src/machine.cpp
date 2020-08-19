#include "machine.hpp"

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

auto machine::get_or() const -> word
{
    return operand_reg_;
}

auto machine::get_ac() const -> word
{
    return accumulator_;
}

auto machine::get_ir() const -> word
{
    return instruction_reg_;
}

auto machine::get_pc() const -> word
{
    return program_counter_;
}

auto machine::execute() -> bool
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
            struct arg
            {
                int16_t val : 9;
            };

            if ( get_or().is_arg_negative() )
            {
                auto shift =
                    static_cast<word::type>( -arg { static_cast<int16_t>( *get_or() ) }.val );
                set_ac( word( *get_ac() >> shift ) );
            }
            else if ( !get_or().is_arg_negative() )
            {
                set_ac( word( *get_ac() << *get_or() ) );
            }

            break;
        }
        case instruction::SHC:
        {
            struct arg
            {
                int16_t val : 9;
            };

            auto rot_left = []( uint16_t value, uint16_t count ) {
                return value << count | value >> ( 16U - count );
            };

            auto rot_right = []( uint16_t value, uint16_t count ) {
                return value >> count | value << ( 16U - count );
            };

            set_ac( word( get_or().is_arg_negative()
                              ? rot_right( *get_ac(),
                                           static_cast<uint16_t>(
                                               -arg { static_cast<int16_t>( *get_or() ) }.val ) )
                              : rot_left( *get_ac(), *get_or() ) ) );

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

void machine::set_memory( const mem_t& mem )
{
    memory_ = mem;
}

auto machine::get_memory() const -> machine::mem_t
{
    return memory_;
}

} // namespace vnm
