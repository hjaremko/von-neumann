#ifndef VON_NEUMANN_FILE_PRINTER_HPP
#define VON_NEUMANN_FILE_PRINTER_HPP

#include "printer.hpp"

#include <filesystem>
#include <fstream>

namespace vnm
{

template <class T>
class file_printer : public printer_interface
{
public:
    file_printer( const std::filesystem::path& f, const vnm::machine& m )
        : f_( f, std::ios::out | std::ios::trunc ),
          p_( std::make_unique<printer<T>>( f_, m ) )
    {
    }

    void print_registers_table() const override
    {
        p_->print_registers_table();
    }

    void print_registers() const override
    {
        p_->print_registers();
    }

    void print_memory() const override
    {
        p_->print_memory();
    }

private:
    std::fstream f_;
    std::unique_ptr<printer_interface> p_;
};

} // namespace vnm

#endif // VON_NEUMANN_FILE_PRINTER_HPP
