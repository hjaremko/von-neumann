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
        }

        instruction decode()
        {
            set_or( m_instruction_reg );
            return m_instruction_reg.get_code();
        }


        bool execute()
        {
            if ( m_instruction_reg.m_instruction )
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
                        // if ( get_ac().get() < 0 )
                        if ( get_ac().is_arg_negative() )
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
                        // set_ac( get_ac().get() / get_or().get() );
                        set_ac( get_ac().get_complete_arg() / get_or().get_complete_arg() );
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
                        if ( get_or().is_arg_negative() )
                        {
                            set_ac( get_ac().get() << std::abs ( get_or().get_complete_arg() ) | std::abs( get_or().get_complete_arg() ) >> ( 16 - std::abs( get_or().get_complete_arg() ) ) );
                        }
                        else
                        {
                            set_ac( get_ac().get() >> get_or().get_complete_arg() | get_or().get_complete_arg() << ( 16 - get_or().get_complete_arg() ) );
                        }

                        break;
                    }
                }
            }

            return true;
        }

        void put_to_memory( const word t_word, const word t_register )
        {
            m_mem.set( t_word, t_register );
        }

        void print_registers_table( std::ostream& t_ostream ) const
        {
            t_ostream << "--------------------------------------------------" << std::endl;
            t_ostream << "|         IR          |    |    |    |           |" << std::endl;
            t_ostream << "----------------------| PC | OR | AC |   next    |" << std::endl;
            t_ostream << "| code  | mode | arg  |    |    |    |           |" << std::endl;
            t_ostream << "--------------------------------------------------" << std::endl;
        }

        void print_registers( std::ostream& t_ostream ) const
        {
            t_ostream << "| "  << std::setw( 6 )  << instructions_to_str.at( m_instruction_reg.get_code() );
            t_ostream << "|  " << std::setw( 4 )  << mode_to_str.at( m_instruction_reg.get_mode() );
            t_ostream << "| "  << std::setw( 5 )  << m_instruction_reg.get_complete_arg();
            t_ostream << "| "  << std::setw( 3 )  << m_program_counter.get();
            t_ostream << "| "  << std::setw( 3 )  << m_operand_reg.get();
            t_ostream << "| "  << std::setw( 3 )  << m_accumulator.get();
            t_ostream << "| "  << std::setw( 10 ) << m_mem.get( m_program_counter ) << "|" << std::endl;
            t_ostream << "--------------------------------------------------" << std::endl;
        }

        void print_memory( std::ostream& t_ostream ) const
        {
            t_ostream << "--------------------------------------------------" << std::endl;

            for ( int i = 0; i < get_size(); ++i )
            {
                t_ostream << "[ " << std::left << std::setw( 3 )<<  i << " ]: " << m_mem.get( word( i ) ) << std::endl;
            }
                
            t_ostream << "--------------------------------------------------" << std::endl;
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
