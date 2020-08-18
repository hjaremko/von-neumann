#include "catch.hpp"
#include "machine.hpp"

TEST_CASE( "example codes executed successfully", "[machine]" )
{
    using namespace vnm;

    auto machine { vnm::machine {} };

    SECTION( "array sum example" )
    {
        const std::vector<word> code = {
            word { "LOAD", "$", 0 },   word { "STORE", "$", 19 },
            word { "LOAD", "$", 21 },  word { "STORE", "$", 18 },
            word { "LOAD", "$", 21 },  word { "ADD", "@", 20 },
            word { "SUB", "@", 18 },   word { "JZERO", "$", 15 },
            word { "LOAD", "@", 19 },  word { "ADD", "&", 18 },
            word { "STORE", "$", 19 }, word { "LOAD", "@", 18 },
            word { "ADD", "$", 1 },    word { "STORE", "$", 18 },
            word { "JUMP", "$", 4 },
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

TEST_CASE( "stop tests", "[machine]" )
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
