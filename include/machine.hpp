#ifndef MACHINE_HPP
#define MACHINE_HPP

#include "memory.hpp"

namespace vnm
{

class machine
{
public:
    using mem_t = memory<512>;

    machine() = default;

    void put_to_memory( const word&, const word& );

    auto pc() -> word&;
    [[nodiscard]] auto pc() const -> word;

    void set_or( const word& );
    void set_ac( const word& );
    void set_memory( const mem_t& );
    void tick();
    [[nodiscard]] auto get_memory() const -> mem_t;
    [[nodiscard]] auto get_or() const -> word;
    [[nodiscard]] auto get_ac() const -> word;
    [[nodiscard]] auto get_ir() const -> word;
    auto execute() -> bool;

private:
    mem_t memory_;
    word program_counter_;
    word instruction_reg_;
    word accumulator_;
    word operand_reg_;
};

inline void machine::tick()
{
    instruction_reg_ = memory_.at( program_counter_ );
    ++program_counter_;
}

} // namespace vnm

#endif
