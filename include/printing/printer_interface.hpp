#ifndef PRINTER_INTERFACE_HPP
#define PRINTER_INTERFACE_HPP

// TODO: print all in binary

namespace vnm
{

class printer_interface
{
public:
    virtual void print_registers_table() const = 0;
    virtual void print_registers() const = 0;
    virtual void print_memory() const = 0;

    printer_interface() = default;
    printer_interface( const printer_interface& ) = default;
    printer_interface( printer_interface&& ) = default;
    auto operator=( const printer_interface& ) -> printer_interface& = default;
    auto operator=( printer_interface&& ) -> printer_interface& = default;
    virtual ~printer_interface() = default;
};

} // namespace vnm

#endif
