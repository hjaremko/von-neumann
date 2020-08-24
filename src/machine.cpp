#include "machine.hpp"

namespace vnm
{

auto machine::execute() -> bool
{
    if ( !instruction_reg.is_instruction() )
    {
        return true;
    }

    if ( instruction_reg == stop )
    {
        return false;
    }

    operand_reg = get_argument( instruction_reg.get_mode() )();
    execute_instruction( instruction_reg.get_code() )();

    return true;
}

void machine::put_to_memory( const word& value, const word& addr )
{
    ram[ addr ] = value;
}

void machine::tick()
{
    instruction_reg = ram[ program_counter++ ];
}

auto machine::get_argument( mode m ) -> std::function<word()>
{
    constexpr auto ARGUMENT_SIZE = 9U;
    return mode_map_.at( static_cast<word::type>( m ) >> ARGUMENT_SIZE );
}

auto machine::execute_instruction( instruction i ) -> std::function<void()>
{
    constexpr auto ARGUMENT_AND_MODE_SIZE = 11U;
    return instr_map_.at( static_cast<word::type>( i ) >>
                          ARGUMENT_AND_MODE_SIZE );
}

auto machine::shz() -> std::function<void()>
{
    return [ & ]() {
        if ( operand_reg.is_arg_negative() )
        {
            auto shift = static_cast<word::type>(
                -arg_type { static_cast<int16_t>( *operand_reg ) }.val );
            accumulator = word( *accumulator >> shift );
        }
        else if ( !operand_reg.is_arg_negative() )
        {
            accumulator = word( *accumulator << *operand_reg );
        }
    };
}

auto machine::shc() -> std::function<void()>
{
    return [ & ]() {
        auto rot_left = []( uint16_t value, uint16_t count ) {
            return value << count | value >> ( 16U - count );
        };

        auto rot_right = []( uint16_t value, uint16_t count ) {
            return value >> count | value << ( 16U - count );
        };

        accumulator =
            word( operand_reg.is_arg_negative()
                      ? rot_right( *accumulator,
                                   static_cast<uint16_t>( -arg_type {
                                       static_cast<int16_t>( *operand_reg ) }
                                                               .val ) )
                      : rot_left( *accumulator, *operand_reg ) );
    };
}

} // namespace vnm
