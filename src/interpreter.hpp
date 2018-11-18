#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include <stdexcept>
#include <sstream>
#include <string>
#include <fstream>
#include <cctype>

namespace vnm
{

class interpreter
{
    public:
        interpreter( std::string t_file_name ) : m_file( t_file_name.c_str() )
        {
            m_file.exceptions( std::ifstream::failbit | std::ifstream::badbit );
        }

        ~interpreter()
        {
            m_file.close();
        }

        bool interpret( machine& t_machine )
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

                        if ( tmp == "$" | tmp == "@" | tmp == "&" | tmp == "+" )
                        {
                            std::string mode( tmp );
                            std::string args;
                            ss >> args;

                            if ( ss.good() || ( ss.eof() && args.length() > 0 && std::isdigit( args.at( 0 ) ) ) )
                            {
                                t_machine.put_to_memory( word( code, mode, std::stoi( args ) ), i );
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
                                t_machine.put_to_memory( 0b1'0000'00'000000000, i );
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

    private:
        std::fstream m_file;
};

}

#endif
