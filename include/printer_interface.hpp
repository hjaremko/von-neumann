#ifndef PRINTER_INTERFACE_HPP
#define PRINTER_INTERFACE_HPP

#include <bitset>

// TODO: print all in binary

namespace vnm
{
class printer_interface
{
public:
    virtual void print_registers_table() const = 0;
    virtual void print_registers() const = 0;
    virtual void print_memory() const = 0;
};

class word_printer_interface
{
public:
    virtual void print_word( std::ostream&, const vnm::word& w ) const = 0;
};

class word_default_printer : public word_printer_interface
{
public:
    void print_word( std::ostream& os_, const word& rhs ) const override
    {
        std::stringstream ss;

        if ( rhs.is_instruction() )
        {
            if ( rhs.get_code() == vnm::instruction::STOP )
            {
                ss << "STOP";
            }
            else
            {
                ss << std::left << std::setw( 6 ) << instructions_to_str.at( rhs.get_code() )
                   << mode_to_str.at( rhs.get_mode() ) << ' ' << *rhs.get_arg();
            }
        }
        else if ( *rhs != 0 )
        {
            ss << *rhs;
        }

        os_ << ss.str();
    }
};

class default_printer : public printer_interface, public word_default_printer
{
public:
    default_printer( std::ostream& os, const machine& machine ) : os_( os ), machine_( machine )
    {
    }

    void print_registers_table() const override
    {
        os_ << "--------------------------------------------------" << std::endl;
        os_ << "|         IR          |    |    |    |           |" << std::endl;
        os_ << "----------------------| PC | OR | AC |   next    |" << std::endl;
        os_ << "| code  | mode | arg  |    |    |    |           |" << std::endl;
        os_ << "--------------------------------------------------" << std::endl;
    }

    void print_registers() const override
    {
        os_ << std::left;
        os_ << "| " << std::setw( 6 );

        const auto& ir { machine_.get_ir() };
        const auto& ir_code { ir.get_code() };
        const auto& ir_mode { ir.get_mode() };
        const auto& ir_arg { ir.get_arg() };
        const auto& pc { machine_.get_pc() };
        const auto& oreg { machine_.get_or() };
        const auto& ac { machine_.get_ac() };
        const auto& next { machine_.get_memory().at( pc ) };

        if ( ir.is_instruction() )
            os_ << instructions_to_str.at( ir_code );
        else
            os_ << ' ';

        os_ << "|  " << std::setw( 4 ) << mode_to_str.at( ir_mode );
        os_ << "| " << std::setw( 5 );
        print_word( os_, ir_arg );
        os_ << "| " << std::setw( 3 ) << *pc;
        os_ << "| " << std::setw( 3 ) << *oreg;
        os_ << "| " << std::setw( 3 ) << *ac;
        os_ << "| " << std::setw( 10 );
        print_word( os_, next );
        os_ << "|" << std::endl;
        os_ << "--------------------------------------------------" << std::endl;
    }

    void print_memory() const override
    {
        os_ << "--------------------------------------------------" << std::endl;

        word::type i = 0;
        //        for ( ; machine_.get_memory().get( word { i } ) != vnm::stop; ++i )
        for ( ; i < get_size(); ++i )
        {
            print_memory_cell( i );
        }

        print_memory_cell( i );

        os_ << "--------------------------------------------------" << std::endl;
    }

private:
    void print_memory_cell( word::type i ) const
    {
        os_ << "[ " << std::left << std::setw( 3 ) << i << " ]: ";
        print_word( os_, machine_.get_memory().at( word { static_cast<word::type>( i ) } ) );
        os_ << std::endl;
    }

    [[nodiscard]] word::type get_size() const
    {
        for ( word::type i = 511; i >= 0; --i )
        {
            if ( *machine_.get_memory().at( word { i } ) != 0 )
            {
                return i + 2;
            }
        }

        return 511;
    }

    std::ostream& os_;
    const machine& machine_;
};

class binary_printer : public default_printer
{
public:
    binary_printer( std::ostream& os, const machine& machine ) : default_printer( os, machine )
    {
    }

    void print_word( std::ostream& o, const word& rhs ) const override
    {
        std::stringstream ss;

        if ( rhs.is_instruction() )
        {
            if ( rhs.get_code() == vnm::instruction::STOP )
            {
                ss << "STOP";
            }
            else
            {
                ss << std::left << std::setw( 6 ) << instructions_to_str.at( rhs.get_code() )
                   << mode_to_str.at( rhs.get_mode() ) << ' ';
                ss << std::bitset<9>( *rhs.get_arg() );
            }
        }
        else if ( *rhs != 0 )
        {
            ss << std::bitset<16>( *rhs );
        }

        o << ss.str();
    }
};

class signed_printer : public default_printer
{
public:
    signed_printer( std::ostream& os, const machine& machine ) : default_printer( os, machine )
    {
    }

    struct s
    {
        int val : 9;
    };

    void print_word( std::ostream& o, const word& rhs ) const override
    {
        std::stringstream ss;

        if ( rhs.is_instruction() )
        {
            if ( rhs.get_code() == vnm::instruction::STOP )
            {
                ss << "STOP";
            }
            else
            {
                ss << std::left << std::setw( 6 ) << instructions_to_str.at( rhs.get_code() )
                   << mode_to_str.at( rhs.get_mode() ) << ' ';
                const auto t { s { *rhs.get_arg() } };
                ss << t.val;
            }
        }
        else if ( *rhs != 0 )
        {
            ss << static_cast<int16_t>( *rhs );
        }

        o << ss.str();
    }
};

std::unique_ptr<printer_interface>
make_printer( const cxxopts::ParseResult& parse_result, std::ostream& os, const machine& m )
{
    if ( parse_result.count( "binary" ) )
        return std::make_unique<binary_printer>( os, m );

    if ( parse_result.count( "signed" ) )
        return std::make_unique<signed_printer>( os, m );

    return std::make_unique<default_printer>( os, m );
}
} // namespace vnm

#endif
