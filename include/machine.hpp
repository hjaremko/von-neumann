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
    void tick();
    [[nodiscard]] memory get_memory() const;
    [[nodiscard]] word get_or() const;
    [[nodiscard]] word get_ac() const;
    [[nodiscard]] int get_size() const;
    bool execute();

private:
    memory m_mem;
    word m_program_counter;
    word m_instruction_reg;
    word m_accumulator;
    word m_operand_reg;
};

inline void machine::tick()
{
    m_instruction_reg = m_mem.get( m_program_counter );
    ++m_program_counter;
}

} // namespace vnm

#endif
