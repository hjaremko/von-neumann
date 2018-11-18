#ifndef MACHINE_HPP
#define MACHINE_HPP

#include "memory.hpp"

namespace vnm
{

class machine
{
    public:
        machine()
        {
        }

        void set_or( const word t_word )
        {
            m_operand_reg = t_word;
        }

        void set_ac( const word t_word )
        {
            m_accumulator = t_word;
        }

        word get_or() const
        {
            return m_operand_reg;
        }

        word get_ac() const
        {
            return m_accumulator;
        }

        void get_from_memory()
        {
            m_instruction_reg = m_mem.get( m_program_counter );
            // m_program_counter++;
            m_program_counter.set( m_program_counter.get() + 1 );

            if ( m_program_counter.get() >= 512 || m_program_counter.get() < 0 )
            {
                throw std::runtime_error( "Address exceeds device memory! Perhaps missing STOP?" );
            }
        }

        instruction decode()
        {
            set_or( m_instruction_reg );
            return m_instruction_reg.get_code();
        }


        bool execute()
        {
            switch ( m_instruction_reg.get_mode() )
            {
                case mode::instant:
                {
                    set_or( m_instruction_reg.get_arg() );
                    break;
                }
                case mode::direct:
                {
                    set_or( m_mem.get( m_instruction_reg.get_arg() ) );
                    break;
                }
                case mode::indirect:
                {
                    set_or( m_mem.get( m_mem.get( m_instruction_reg.get_arg() ) ) );
                    break;
                }
                case mode::index:
                {
                    // TODO operator+
                    set_or( get_ac().get() + m_instruction_reg.get_arg().get() );
                    break;
                }
            }

            switch ( m_instruction_reg.get_code() )
            {
                case instruction::STOP:
                {
                    // print_registers();
                    // print_memory();
                    return false;

                    break;
                }
                case instruction::LOAD:
                {
                    set_ac( get_or() );
                    break;
                }
                case instruction::STORE:
                {
                    m_mem.set( get_ac(), get_or() );
                    break;
                }
                case instruction::JUMP:
                {
                    m_program_counter = get_or();
                    break;
                }
                case instruction::JNEG:
                {
                    if ( get_ac().get() < 0 )
                    {
                        m_program_counter = get_or();
                    }
                    
                    break;
                }
                case instruction::JZERO:
                {
                    if ( get_ac().get() == 0 )
                    {
                        m_program_counter = get_or();
                    }
                    
                    break;
                }
                case instruction::ADD:
                {
                    set_ac( get_ac().get() + get_or().get() );
                    break;
                }
                case instruction::SUB:
                {
                    set_ac( get_ac().get() - get_or().get() );
                    break;
                }
                case instruction::MULT:
                {
                    set_ac( get_ac().get() * get_or().get() );
                    break;
                }
                case instruction::DIV:
                {
                    set_ac( get_ac().get() / get_or().get() );
                    break;
                }
                case instruction::AND:
                {
                    set_ac( get_ac().get() & get_or().get() );
                    break;
                }
                case instruction::OR:
                {
                    set_ac( get_ac().get() | get_or().get() );
                    break;
                }
                case instruction::NOT:
                {
                    set_ac( !get_or().get() );
                    break;
                }
                case instruction::CMP:
                {
                    if ( get_ac().get() == get_or().get() )
                    {
                        set_ac( word( -1 ) );
                    }
                    else
                    {
                        set_ac( word( 0 ) );
                    }

                    break;
                }
                case instruction::SHZ:
                {
                    if ( get_or().get() < 0 )
                    {
                        set_ac( get_ac().get() >> std::abs( get_or().get() ) );
                    }
                    else if ( get_or().get() > 0 )
                    {
                        set_ac( get_ac().get() << std::abs( get_or().get() ) );
                    }

                    break;
                }
                case instruction::SHC:
                {
                    // TODO
                    break;
                }
            }

            return true;
        }

        void put_to_memory( const word t_word, const word t_register )
        {
            m_mem.set( t_word, t_register );
        }

        void print_registers() const
        {
            std::cout << "-------------------------------" << std::endl;
            std::cout << "IR: " << m_instruction_reg << std::endl;
            std::cout << "INS:" << m_mem.get( m_program_counter ) << std::endl << std::endl;
            std::cout << "PC: " << m_program_counter.get() << std::endl;
            std::cout << "OR: " << get_or().get() << std::endl;
            std::cout << "AC: " << get_ac().get() << std::endl;
            std::cout << "-------------------------------" << std::endl;
        }

        void print_memory() const
        {
            std::cout << "-------------------------------" << std::endl;

            for ( int i = 0; i < get_size(); ++i )
            {
                std::cout << "[ " << std::left << std::setw( 3 )<<  i << " ]: " << m_mem.get( word( i ) ) << std::endl;
            }
                
            std::cout << "-------------------------------" << std::endl;
        }

        int get_size() const
        {
            for ( int i = 511; i >= 0; --i )
            {
                if ( m_mem.get( i ).get() != 0 )
                {
                    return i + 2;
                }
            }
        }

    private:
        memory m_mem;
        word   m_program_counter;
        word   m_instruction_reg;
        word   m_accumulator;
        word   m_operand_reg;
};

}

#endif
