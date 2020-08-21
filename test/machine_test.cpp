#include "catch.hpp"
#include "machine.hpp"

TEST_CASE( "example codes executed successfully", "[machine]" )
{
    using namespace vnm;

    auto machine { vnm::machine {} };

    SECTION( "array sum example" )
    {
        const std::vector<word> code = {
            { "LOAD", "$", 0 },  { "STORE", "$", 19 }, { "LOAD", "$", 21 },  { "STORE", "$", 18 },
            { "LOAD", "$", 21 }, { "ADD", "@", 20 },   { "SUB", "@", 18 },   { "JZERO", "$", 15 },
            { "LOAD", "@", 19 }, { "ADD", "&", 18 },   { "STORE", "$", 19 }, { "LOAD", "@", 18 },
            { "ADD", "$", 1 },   { "STORE", "$", 18 }, { "JUMP", "$", 4 },
        };

        auto i { word::type { 0 } };
        for ( const auto& instruction : code )
        {
            machine.put_to_memory( instruction, i++ );
        }

        machine.put_to_memory( stop, i );
        machine.put_to_memory( 5, 20 );
        machine.put_to_memory( 10, 21 );
        machine.put_to_memory( 20, 22 );
        machine.put_to_memory( 30, 23 );
        machine.put_to_memory( 40, 24 );
        machine.put_to_memory( 50, 25 );

        while ( machine.execute() )
        {
            machine.tick();
        }

        const auto memory_state { machine.ram };
        REQUIRE( *memory_state[ 18 ] == 26 );
        REQUIRE( *memory_state[ 19 ] == 150 );
    }

    SECTION( "double sum example" )
    {
        const std::vector<word> code = {
            { "LOAD", "@", 5 },
            { "ADD", "@", 6 },
            { "MULT", "$", 2 },
            { "STORE", "$", 7 },
        };

        auto i { word::type { 0 } };
        for ( const auto& instruction : code )
        {
            machine.put_to_memory( instruction, i++ );
        }

        machine.put_to_memory( stop, i );
        machine.put_to_memory( 21, 5 );
        machine.put_to_memory( 34, 6 );
        machine.put_to_memory( 0, 7 );

        while ( machine.execute() )
        {
            machine.tick();
        }

        const auto memory_state { machine.ram };
        REQUIRE( *memory_state[ 7 ] == 110 );
    }
}

TEST_CASE( "STOP tests", "[machine]" )
{
    using namespace vnm;

    auto machine { vnm::machine {} };

    SECTION( "single stop" )
    {
        machine.put_to_memory( stop, 0 );
        machine.tick();
        REQUIRE( machine.instruction_reg == machine.ram[ 0 ] );
        REQUIRE( machine.ram[ 0 ].is_instruction() );
        REQUIRE( machine.ram[ 0 ].get_mode() == mode::instant );
        REQUIRE( machine.ram[ 0 ].get_code() == instruction::STOP );
        REQUIRE_FALSE( machine.execute() );
    }

    SECTION( "single stop on position other than 0 " )
    {
        machine.put_to_memory( stop, 12 );

        while ( machine.execute() )
        {
            machine.tick();
        }

        REQUIRE_FALSE( machine.execute() );
    }

    SECTION( "stop with some values" )
    {
        machine.put_to_memory( 1, 5 );
        machine.put_to_memory( 1, 7 );
        machine.put_to_memory( 1, 8 );
        machine.put_to_memory( 1, 10 );
        machine.put_to_memory( stop, 12 );

        while ( machine.execute() )
        {
            machine.tick();
        }

        REQUIRE_FALSE( machine.execute() );
    }
}

TEST_CASE( "JNEG tests", "[machine]" )
{
    using namespace vnm;

    auto machine { vnm::machine {} };

    SECTION( "jump on negative AC" )
    {
        auto w { static_cast<word::type>( -5 ) };
        machine.put_to_memory( { "LOAD", "$", w }, 1 );
        machine.put_to_memory( { "JNEG", "$", 5 }, 2 );
        machine.put_to_memory( { "STORE", "$", 4 }, 3 );
        machine.put_to_memory( stop, 5 );

        while ( machine.execute() )
        {
            machine.tick();
        }

        REQUIRE( *machine.ram[ 4 ] != 507 );
    }

    SECTION( "do not jump on positive AC" )
    {
        auto w { static_cast<word::type>( 5 ) };
        machine.put_to_memory( { "LOAD", "$", w }, 1 );
        machine.put_to_memory( { "JNEG", "$", 5 }, 2 );
        machine.put_to_memory( { "STORE", "$", 4 }, 3 );
        machine.put_to_memory( stop, 5 );

        while ( machine.execute() )
        {
            machine.tick();
        }

        REQUIRE( *machine.ram[ 4 ] == 5 );
    }
}

TEST_CASE( "SHZ tests", "[machine]" )
{
    using namespace vnm;

    auto machine { vnm::machine {} };

    SECTION( "shift left" )
    {
        auto w { word::type { 1 } };
        machine.put_to_memory( { "LOAD", "$", w }, 0 );
        machine.put_to_memory( { "SHZ", "$", 1 }, 1 );
        machine.put_to_memory( stop, 5 );

        while ( machine.execute() )
        {
            machine.tick();
        }

        REQUIRE( *machine.accumulator == 1u << 1u );
    }

    SECTION( "shift right" )
    {
        auto w { word::type { 5 } };
        machine.put_to_memory( { "LOAD", "$", w }, 0 );
        machine.put_to_memory( { "SHZ", "$", static_cast<word::type>( -1 ) }, 1 );
        machine.put_to_memory( stop, 5 );

        while ( machine.execute() )
        {
            machine.tick();
        }

        REQUIRE( *machine.accumulator == 5u >> 1u );
    }
}

TEST_CASE( "SHC tests", "[machine]" )
{
    using namespace vnm;

    auto machine { vnm::machine {} };

    SECTION( "shift left" )
    {
        auto w { word::type { 0b1100'0000'0000'0000 } };
        machine.put_to_memory( w, 0 );
        machine.put_to_memory( { "LOAD", "@", 0 }, 1 );
        machine.put_to_memory( { "SHC", "$", 1 }, 2 );
        machine.put_to_memory( stop, 5 );

        while ( machine.execute() )
        {
            machine.tick();
        }

        REQUIRE( *machine.accumulator == 0b1000'0000'0000'0001 );
    }

    SECTION( "shift right" )
    {
        auto w { word::type { 0b1100'0000'0000'0010 } };
        machine.put_to_memory( word { w }, 0 );
        machine.put_to_memory( word { "LOAD", "@", 0 }, 1 );
        machine.put_to_memory( word { "SHC", "$", static_cast<word::type>( -2 ) }, 2 );
        machine.put_to_memory( stop, 5 );

        while ( machine.execute() )
        {
            machine.tick();
        }

        REQUIRE( *machine.accumulator == 0b1011'0000'0000'0000 );
    }
}

TEST_CASE( "machine method tests", "[machine]" )
{
    using namespace vnm;

    auto machine { vnm::machine {} };

    SECTION( "set memory" )
    {
        machine::mem_t mem;
        mem[ 1 ] = word { "STORE", "@", 0 };
        mem[ 0 ] = stop;
        machine.ram = mem;

        REQUIRE( machine.ram[ 0 ] == mem[ 0 ] );
        REQUIRE( machine.ram[ 1 ] == mem[ 1 ] );
    }

    SECTION( "program counter" )
    {
        REQUIRE( machine.program_counter == 0 );
        machine.program_counter = 20;
        REQUIRE( machine.program_counter == 20 );
    }

    SECTION( "tick" )
    {
        auto w { word { "STORE", "@", 0 } };
        machine.put_to_memory( w, 0 );

        REQUIRE( machine.program_counter == 0 );
        REQUIRE( machine.instruction_reg == 0 );
        machine.tick();
        REQUIRE( machine.program_counter == 1 );
        REQUIRE( machine.instruction_reg == w );
    }
}
