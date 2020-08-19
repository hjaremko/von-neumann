#include "catch.hpp"
#include "word.hpp"

#include <sstream>

TEST_CASE( "word constructor", "[word]" )
{
    using namespace vnm;

    SECTION( "default" )
    {
        auto w { word {} };
        REQUIRE( w.get() == 0 );
        REQUIRE_FALSE( w.is_instruction() );
    }

    SECTION( "STOP from string" )
    {
        auto w { word { "STOP", "$", 0 } };
        REQUIRE( w.get_code() == instruction::STOP );
        REQUIRE( w.get_mode() == mode::instant );
        REQUIRE( *w.get_arg() == 0 );
        REQUIRE( w.is_instruction() );
    }

    SECTION( "CMP from string" )
    {
        auto w { word { "CMP", "&", 511 } };
        REQUIRE( w.get_code() == instruction::CMP );
        REQUIRE( w.get_mode() == mode::indirect );
        REQUIRE( *w.get_arg() == 511 );
        REQUIRE( w.is_instruction() );
    }
}

TEST_CASE( "word methods", "[word]" )
{
    using namespace vnm;
    auto w { word {} };

    SECTION( "set and get" )
    {
        w.set( 256 );
        REQUIRE( w.get() == 256 );
        REQUIRE( *w == 256 );
    }

    SECTION( "const get" )
    {
        const auto ww { word { 10000 } };
        REQUIRE( *ww == 10000 );
    }

    SECTION( "to instruction" )
    {
        REQUIRE_FALSE( w.is_instruction() );
        w.to_instruction();
        REQUIRE( w.is_instruction() );
        w.to_instruction();
        REQUIRE( w.is_instruction() );
    }

    SECTION( "get arg" )
    {
        w.set( 0b1'1011'11'101010101u );
        REQUIRE( *w.get_arg() == 0b0'0000'00'101010101u );
    }

    SECTION( "get instant mode" )
    {
        w.set( 0b1'1011'00'101010101u );
        REQUIRE( w.get_mode() == mode::instant );
    }

    SECTION( "get direct mode" )
    {
        w.set( 0b1'1011'01'101010101u );
        REQUIRE( w.get_mode() == mode::direct );
    }

    SECTION( "get indirect mode" )
    {
        w.set( 0b1'1011'10'101010101u );
        REQUIRE( w.get_mode() == mode::indirect );
    }

    SECTION( "get index mode" )
    {
        w.set( 0b1'1011'11'101010101u );
        REQUIRE( w.get_mode() == mode::index );
    }

    SECTION( "get STOP code" )
    {
        w.set( 0b1'0000'11'101010101u );
        REQUIRE( w.get_code() == instruction::STOP );
    }

    SECTION( "get OR code" )
    {
        w.set( 0b1'1011'11'101010101u );
        REQUIRE( w.get_code() == instruction::OR );
    }

    SECTION( "is arg negative" )
    {
        auto val { int16_t { 1453 } };
        w.set( val );
        w.to_instruction();
        REQUIRE( w.is_arg_negative() );
        REQUIRE( w.get() > 0 );
    }

    SECTION( "is arg non negative" )
    {
        auto val { int16_t { -31571 } };
        w.set( val );
        w.to_instruction();
        REQUIRE_FALSE( w.is_arg_negative() );
        REQUIRE( static_cast<int16_t>( w.get() ) < 0 );
    }

    //    SECTION( "is non instruction arg negative" )
    //    {
    //        auto val { int16_t { -31571 } };
    //        w.set( val );
    //        REQUIRE( w.is_arg_negative() );
    //        REQUIRE( static_cast<int16_t>( w.get() ) < 0 );
    //    }
    //
    //    SECTION( "is non instruction arg non negative" )
    //    {
    //        auto val { int16_t { 1453 } };
    //        w.set( val );
    //        REQUIRE_FALSE( w.is_arg_negative() );
    //        REQUIRE( w.get() > 0 );
    //    }
}

TEST_CASE( "word operators test", "[word]" )
{
    using namespace vnm;

    auto lhs { word::type { 250 } };
    auto rhs { word::type { 5 } };

    SECTION( "plus" )
    {
        auto actual = word { lhs } + word { rhs };
        REQUIRE( *actual == lhs + rhs );
    }

    SECTION( "minus" )
    {
        auto actual = word { lhs } - word { rhs };
        REQUIRE( *actual == lhs - rhs );
    }

    SECTION( "multiply" )
    {
        auto actual = word { lhs } * word { rhs };
        REQUIRE( *actual == lhs * rhs );
    }

    SECTION( "divide" )
    {
        auto actual = word { lhs } / word { rhs };
        REQUIRE( *actual == lhs / rhs );
    }

    SECTION( "bit and" )
    {
        lhs = 0b0100'0100'0010'1110u;
        rhs = 0b0100'0010'0010'0100u;
        auto actual = word { lhs } & word { rhs };
        REQUIRE( *actual == ( lhs & rhs ) );
    }

    SECTION( "bit or" )
    {
        lhs = 0b0100'0100'0010'1110u;
        rhs = 0b0100'0010'0010'0100u;
        auto actual = word { lhs } | word { rhs };
        REQUIRE( *actual == ( lhs | rhs ) );
    }

    SECTION( "negation" )
    {
        auto w { word::type { 0b0100'0100'0010'1110u } };
        REQUIRE( *( !word { w } ) == !w );
    }

    SECTION( "increment" )
    {
        auto w { word {} };
        auto w2 = w++;
        REQUIRE( w2 == word {} );
        REQUIRE( w == word { 1 } );
        REQUIRE( ++w == word { 2 } );
    }

    SECTION( "decrement" )
    {
        auto w { word {} };
        auto w2 = w--;
        REQUIRE( w2 == word {} );
        REQUIRE( w == word { static_cast<word::type>( -1 ) } );
        REQUIRE( --w == word { static_cast<word::type>( -2 ) } );
    }

    SECTION( "input" )
    {
        auto w { word {} };
        auto ss { std::stringstream { "SHZ @ 3; with comment" } };
        ss >> w;
        REQUIRE( w.get_code() == instruction::SHZ );
        REQUIRE( w.get_mode() == mode::direct );
        REQUIRE( *w.get_arg() == 3 );
        REQUIRE( w.is_instruction() );
    }
}
