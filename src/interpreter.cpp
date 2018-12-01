#include "interpreter.hpp"

namespace vnm
{
    interpreter::interpreter( const std::string& t_file_name ) : m_file( t_file_name.c_str() )
    {
        m_file.exceptions( std::ifstream::failbit | std::ifstream::badbit );
    }

    interpreter::~interpreter()
    {
        m_file.close();
    }

    void interpreter::interpret( machine& t_machine )
    {
        int i = 0;

        m_file.exceptions( std::ifstream::goodbit );

        while ( !m_file.eof() )
        {
            std::string line;
            std::getline( m_file, line );
            std::stringstream ss( line );
            std::getline( ss, line, ';' );
            ss.str( line );
            ss.clear();

            std::string tmp;
            ss >> tmp;


            if ( ss.good() || ( ss.eof() && tmp.length() > 0 ) )
            {

                if ( std::isdigit( tmp.at( 0 ) ) )
                {
                    t_machine.put_to_memory( std::stoi( tmp ), i );
                }
                else if ( instructions_from_str.find( tmp ) != instructions_from_str.end() )
                {
                    std::string code( tmp );
                    ss >> tmp;

                    if ( tmp == "$" || tmp == "@" || tmp == "&" || tmp == "+" )
                    {
                        std::string mode( tmp );
                        std::string args;
                        ss >> args;

                        if ( ss.good() || ( ss.eof() && args.length() > 0 ) )
                        {
                            word tmp( code, mode, std::stoi( args ) );
                            tmp.to_instruction();
                            t_machine.put_to_memory( tmp, i );
                        }
                        else
                        {
                            std::stringstream error_stream;
                            error_stream << "Line " << i + 1 << " : Expected numeral value";

                            throw std::runtime_error( error_stream.str() );
                        }
                    }
                    else
                    {
                        if ( tmp == "STOP" )
                        {
                            word tmp( 0 );
                            tmp.to_instruction();
                            t_machine.put_to_memory( tmp, i );
                        }
                        else
                        {
                            std::stringstream error_stream;
                            error_stream << "Line " << i + 1 << " : Invalid instruction";

                            throw std::runtime_error( error_stream.str() );
                        }
                    }
                }
                else
                {
                    std::stringstream error_stream;
                    error_stream << "Line " << i + 1 << " : Invalid instruction";

                    throw std::runtime_error( error_stream.str() );

                }
            }

            ++i;
        }
    }
}
