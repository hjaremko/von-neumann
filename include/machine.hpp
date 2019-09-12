#ifndef MACHINE_HPP
#define MACHINE_HPP

#include "memory.hpp"

namespace vnm
{

class machine
{
public:
    machine() = default;

    void print_registers_table( std::ostream& ) const;
    void print_registers( std::ostream& ) const;
    void print_memory( std::ostream& ) const;
    void put_to_memory( const word&, const word& );
    void set_pc( const word& );
    void set_or( const word& );
    void set_ac( const word& );
    void set_memory( memory );
    void tick();
    [[nodiscard]] memory get_memory() const;
    [[nodiscard]] word get_or() const;
    [[nodiscard]] word get_ac() const;
    [[nodiscard]] int get_size() const;
    bool execute();

private:
    memory memory_;
    word program_counter_;
    word instruction_reg_;
    word accumulator_;
    word operand_reg_;
};

inline void machine::tick()
{
    instruction_reg_ = memory_.get( program_counter_ );
    ++program_counter_;
}

} // namespace vnm

#endif