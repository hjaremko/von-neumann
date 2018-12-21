#ifndef MACHINE_HPP
#define MACHINE_HPP

#include "memory.hpp"

namespace vnm
{

class machine
{
    public:
        machine();

        void print_registers_table( std::ostream& ) const;
        void print_registers( std::ostream& ) const;
        void print_memory( std::ostream& ) const;
        void put_to_memory( const word, const word );
        void set_pc( const word );
        void set_or( const word );
        void set_ac( const word );
        void get_from_memory();
        word get_or() const;
        word get_ac() const;
        bool execute();
        int get_size() const;

    private:
        memory m_mem;
        word   m_program_counter;
        word   m_instruction_reg;
        word   m_accumulator;
        word   m_operand_reg;
};

}

#endif
