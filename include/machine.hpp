#ifndef MACHINE_HPP
#define MACHINE_HPP

#include "memory.hpp"

namespace vnm
{

class machine
{
public:
    using mem_t = memory<512>;

    void put_to_memory( const word&, const word& );
    void tick();
    auto execute() -> bool;

    mem_t ram;
    word program_counter;
    word instruction_reg;
    word accumulator;
    word operand_reg;
};

} // namespace vnm

#endif
