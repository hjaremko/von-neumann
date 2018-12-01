#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include <stdexcept>
#include <sstream>
#include <string>
#include <fstream>

#include "machine.hpp"

namespace vnm
{

class interpreter
{
    public:
        explicit interpreter( const std::string& );
        ~interpreter();

        void interpret( machine& );

    private:
        std::fstream m_file;
};

}

#endif
