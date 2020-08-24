#ifndef MACHINE_HPP
#define MACHINE_HPP

#include "memory.hpp"

#include <functional>
#include <iostream>

namespace vnm
{

class machine
{
public:
    static constexpr auto memory_size { 512 };
    using mem_t = memory<memory_size>;

    void put_to_memory( const word&, const word& );
    void tick();
    auto execute() -> bool;

    mem_t ram;
    word program_counter;
    word instruction_reg;
    word accumulator;
    word operand_reg;

private:
    auto get_argument( mode m ) -> std::function<word()>;
    auto execute_instruction( instruction m ) -> std::function<void()>;
    auto shz() -> std::function<void()>;
    auto shc() -> std::function<void()>;

    struct arg_type
    {
        int16_t val : 9;
    };

    const std::array<std::function<word()>, 4> mode_map_ {
        [ & ]() { return instruction_reg.get_arg(); },
        [ & ]() { return ram[ instruction_reg.get_arg() ]; },
        [ & ]() { return ram[ ram[ instruction_reg.get_arg() ] ]; },
        [ & ]() { return accumulator + instruction_reg.get_arg(); },
    };

    const std::array<std::function<void()>, 16> instr_map_ {
        [ & ]() {},
        [ & ]() { accumulator = operand_reg; },
        [ & ]() { ram[ operand_reg ] = accumulator; },
        [ & ]() { program_counter = operand_reg; },
        [ & ]() {
            if ( accumulator.is_arg_negative() )
            {
                program_counter = operand_reg;
            }
        },
        [ & ]() {
            if ( *accumulator == 0 )
            {
                program_counter = operand_reg;
            }
        },
        [ & ]() { accumulator = accumulator + operand_reg; },
        [ & ]() { accumulator = accumulator - operand_reg; },
        [ & ]() { accumulator = accumulator * operand_reg; },
        [ & ]() { accumulator = accumulator / operand_reg; },
        [ & ]() { accumulator = accumulator & operand_reg; },
        [ & ]() { accumulator = accumulator | operand_reg; },
        [ & ]() { accumulator = !operand_reg; },
        [ & ]() {
            accumulator = *accumulator == *operand_reg
                              ? static_cast<word::type>( -1 )
                              : 0;
        },
        shz(),
        shc(),
    };
};

} // namespace vnm

#endif
