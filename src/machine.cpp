#include "machine.hpp"

namespace vnm
{

auto machine::execute() -> bool
{
    if ( instruction_reg.is_instruction() )
    {
        switch ( instruction_reg.get_mode() )
        {
        case mode::instant:
        {
            operand_reg = instruction_reg.get_arg();
            break;
        }
        case mode::direct:
        {
            operand_reg = ram[ instruction_reg.get_arg() ];
            break;
        }
        case mode::indirect:
        {
            operand_reg = ram[ ram[ instruction_reg.get_arg() ] ];
            break;
        }
        case mode::index:
        {
            operand_reg = accumulator + instruction_reg.get_arg();
            break;
        }
        }

        switch ( instruction_reg.get_code() )
        {
        case instruction::STOP:
        {
            return false;
        }
        case instruction::LOAD:
        {
            accumulator = operand_reg;
            break;
        }
        case instruction::STORE:
        {
            ram[ operand_reg ] = accumulator;
            break;
        }
        case instruction::JUMP:
        {
            program_counter = operand_reg;
            break;
        }
        case instruction::JNEG:
        {
            if ( accumulator.is_arg_negative() )
            {
                program_counter = operand_reg;
            }

            break;
        }
        case instruction::JZERO:
        {
            if ( *accumulator == 0 )
            {
                program_counter = operand_reg;
            }

            break;
        }
        case instruction::ADD:
        {
            accumulator = accumulator + operand_reg;
            break;
        }
        case instruction::SUB:
        {
            accumulator = accumulator - operand_reg;
            break;
        }
        case instruction::MULT:
        {
            accumulator = accumulator * operand_reg;
            break;
        }
        case instruction::DIV:
        {
            accumulator = accumulator / operand_reg;
            break;
        }
        case instruction::AND:
        {
            accumulator = accumulator & operand_reg;
            break;
        }
        case instruction::OR:
        {
            accumulator = accumulator | operand_reg;
            break;
        }
        case instruction::NOT:
        {
            accumulator = !operand_reg;
            break;
        }
        case instruction::CMP:
        {
            if ( *accumulator == *operand_reg )
            {
                accumulator = word { static_cast<word::type>( -1 ) };
            }
            else
            {
                accumulator = word { 0 };
            }

            break;
        }
        case instruction::SHZ:
        {
            struct arg
            {
                int16_t val : 9;
            };

            if ( operand_reg.is_arg_negative() )
            {
                auto shift =
                    static_cast<word::type>( -arg { static_cast<int16_t>( *operand_reg ) }.val );
                accumulator = word( *accumulator >> shift );
            }
            else if ( !operand_reg.is_arg_negative() )
            {
                accumulator = word( *accumulator << *operand_reg );
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

            accumulator =
                word( operand_reg.is_arg_negative()
                          ? rot_right( *accumulator,
                                       static_cast<uint16_t>(
                                           -arg { static_cast<int16_t>( *operand_reg ) }.val ) )
                          : rot_left( *accumulator, *operand_reg ) );

            break;
        }
        }
    }

    return true;
}

void machine::put_to_memory( const word& value, const word& addr )
{
    ram[ addr ] = value;
}

void machine::tick()
{
    instruction_reg = ram[ program_counter ];
    ++program_counter;
}

} // namespace vnm
