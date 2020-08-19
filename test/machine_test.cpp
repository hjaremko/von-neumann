#include "catch.hpp"
#include "machine.hpp"

TEST_CASE( "example codes executed successfully", "[machine]" )
{
    using namespace vnm;

    auto machine { vnm::machine {} };

    SECTION( "array sum example" )
    {
        const std::vector<word> code = {
            word { "LOAD", "$", 0 },   word { "STORE", "$", 19 }, word { "LOAD", "$", 21 },
            word { "STORE", "$", 18 }, word { "LOAD", "$", 21 },  word { "ADD", "@", 20 },
            word { "SUB", "@", 18 },   word { "JZERO", "$", 15 }, word { "LOAD", "@", 19 },
            word { "ADD", "&", 18 },   word { "STORE", "$", 19 }, word { "LOAD", "@", 18 },
            word { "ADD", "$", 1 },    word { "STORE", "$", 18 }, word { "JUMP", "$", 4 },
        };

        auto i { word::type { 0 } };
        for ( const auto& instruction : code )
        {
            machine.put_to_memory( instruction, word { i++ } );
        }

        machine.put_to_memory( stop, word { i } );
        machine.put_to_memory( word { 5 }, word { 20 } );
        machine.put_to_memory( word { 10 }, word { 21 } );
        machine.put_to_memory( word { 20 }, word { 22 } );
        machine.put_to_memory( word { 30 }, word { 23 } );
        machine.put_to_memory( word { 40 }, word { 24 } );
        machine.put_to_memory( word { 50 }, word { 25 } );

        while ( machine.execute() )
        {
            machine.tick();
        }

        const auto memory_state { machine.get_memory() };
        REQUIRE( *memory_state.at( word { 18 } ) == 26 );
        REQUIRE( *memory_state.at( word { 19 } ) == 150 );
    }

    SECTION( "double sum example" )
    {
        const std::vector<word> code = {
            word { "LOAD", "@", 5 },
            word { "ADD", "@", 6 },
            word { "MULT", "$", 2 },
            word { "STORE", "$", 7 },
        };

        auto i { word::type { 0 } };
        for ( const auto& instruction : code )
        {
            machine.put_to_memory( instruction, word { i++ } );
        }

        machine.put_to_memory( stop, word { i } );
        machine.put_to_memory( word { 21 }, word { 5 } );
        machine.put_to_memory( word { 34 }, word { 6 } );
        machine.put_to_memory( word { 0 }, word { 7 } );

        while ( machine.execute() )
        {
            machine.tick();
        }

        const auto memory_state { machine.get_memory() };
        REQUIRE( *memory_state.at( word { 7 } ) == 110 );
    }
}

TEST_CASE( "STOP tests", "[machine]" )
{
    using namespace vnm;

    auto machine { vnm::machine {} };

    SECTION( "single stop" )
    {
        machine.put_to_memory( stop, word { 0 } );
        machine.tick();
        REQUIRE( machine.get_ir() == machine.get_memory().at( word { 0 } ) );
        REQUIRE( machine.get_memory().at( word { 0 } ).is_instruction() );
        REQUIRE( machine.get_memory().at( word { 0 } ).get_mode() == mode::instant );
        REQUIRE( machine.get_memory().at( word { 0 } ).get_code() == instruction::STOP );
        REQUIRE_FALSE( machine.execute() );
    }

    SECTION( "single stop on position other than 0 " )
    {
        machine.put_to_memory( stop, word { 12 } );

        while ( machine.execute() )
        {
            machine.tick();
        }

        REQUIRE_FALSE( machine.execute() );
    }

    SECTION( "stop with some values" )
    {
        machine.put_to_memory( word { 1 }, word { 5 } );
        machine.put_to_memory( word { 1 }, word { 7 } );
        machine.put_to_memory( word { 1 }, word { 8 } );
        machine.put_to_memory( word { 1 }, word { 10 } );
        machine.put_to_memory( stop, word { 12 } );

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
        machine.put_to_memory( word { "LOAD", "$", w }, word { 1 } );
        machine.put_to_memory( word { "JNEG", "$", 5 }, word { 2 } );
        machine.put_to_memory( word { "STORE", "$", 4 }, word { 3 } );
        machine.put_to_memory( stop, word { 5 } );

        while ( machine.execute() )
        {
            machine.tick();
        }

        REQUIRE( *machine.get_memory().at( word { 4 } ) != 507 );
    }

    SECTION( "do not jump on positive AC" )
    {
        auto w { static_cast<word::type>( 5 ) };
        machine.put_to_memory( word { "LOAD", "$", w }, word { 1 } );
        machine.put_to_memory( word { "JNEG", "$", 5 }, word { 2 } );
        machine.put_to_memory( word { "STORE", "$", 4 }, word { 3 } );
        machine.put_to_memory( stop, word { 5 } );

        while ( machine.execute() )
        {
            machine.tick();
        }

        REQUIRE( *machine.get_memory().at( word { 4 } ) == 5 );
    }
}

TEST_CASE( "SHZ tests", "[machine]" )
{
    using namespace vnm;

    auto machine { vnm::machine {} };

    SECTION( "shift left" )
    {
        auto w { word::type { 1 } };
        machine.put_to_memory( word { "LOAD", "$", w }, word { 0 } );
        machine.put_to_memory( word { "SHZ", "$", 1 }, word { 1 } );
        machine.put_to_memory( stop, word { 5 } );

        while ( machine.execute() )
        {
            machine.tick();
        }

        REQUIRE( *machine.get_ac() == 1u << 1u );
    }

    SECTION( "shift right" )
    {
        auto w { word::type { 5 } };
        machine.put_to_memory( word { "LOAD", "$", w }, word { 0 } );
        machine.put_to_memory( word { "SHZ", "$", static_cast<word::type>( -1 ) }, word { 1 } );
        machine.put_to_memory( stop, word { 5 } );

        while ( machine.execute() )
        {
            machine.tick();
        }

        REQUIRE( *machine.get_ac() == 5u >> 1u );
    }
}

TEST_CASE( "SHC tests", "[machine]" )
{
    using namespace vnm;

    auto machine { vnm::machine {} };

    SECTION( "shift left" )
    {
        auto w { word::type { 0b1100'0000'0000'0000 } };
        machine.put_to_memory( word { w }, word { 0 } );
        machine.put_to_memory( word { "LOAD", "@", 0 }, word { 1 } );
        machine.put_to_memory( word { "SHC", "$", 1 }, word { 2 } );
        machine.put_to_memory( stop, word { 5 } );

        while ( machine.execute() )
        {
            machine.tick();
        }

        REQUIRE( *machine.get_ac() == 0b1000'0000'0000'0001 );
    }

    SECTION( "shift right" )
    {
        auto w { word::type { 0b1100'0000'0000'0010 } };
        machine.put_to_memory( word { w }, word { 0 } );
        machine.put_to_memory( word { "LOAD", "@", 0 }, word { 1 } );
        machine.put_to_memory( word { "SHC", "$", static_cast<word::type>( -2 ) }, word { 2 } );
        machine.put_to_memory( stop, word { 5 } );

        while ( machine.execute() )
        {
            machine.tick();
        }

        REQUIRE( *machine.get_ac() == 0b1011'0000'0000'0000 );
    }
}

TEST_CASE( "machine method tests", "[machine]" )
{
    using namespace vnm;

    auto machine { vnm::machine {} };

    SECTION( "set memory" )
    {
        machine::mem_t mem;
        mem.set( word { "STORE", "@", 0 }, word { 1 } );
        mem.set( stop, word { 0 } );
        machine.set_memory( mem );

        REQUIRE( machine.get_memory().at( word { 0 } ) == mem.at( word { 0 } ) );
        REQUIRE( machine.get_memory().at( word { 1 } ) == mem.at( word { 1 } ) );
    }

    SECTION( "program counter" )
    {
        REQUIRE( machine.get_pc() == word { 0 } );
        machine.set_pc( word { 20 } );
        REQUIRE( machine.get_pc() == word { 20 } );
    }

    SECTION( "tick" )
    {
        auto w { word { "STORE", "@", 0 } };
        machine.put_to_memory( w, word { 0 } );

        REQUIRE( machine.get_pc() == word { 0 } );
        REQUIRE( machine.get_ir() == word { 0 } );
        machine.tick();
        REQUIRE( machine.get_pc() == word { 1 } );
        REQUIRE( machine.get_ir() == w );
    }
}
