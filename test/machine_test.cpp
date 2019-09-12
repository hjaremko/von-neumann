#include "catch.hpp"

#include "machine.hpp"

TEST_CASE( "example codes executed successfully", "[example]" )
{
    vnm::machine machine;
    auto stop = vnm::word{0};
    stop.to_instruction();
    int i = 0;

    SECTION( "array sum example" )
    {
        const std::vector<vnm::word> code = {
            vnm::word{"LOAD", "$", 0},
            vnm::word{"STORE", "$", 19},
            vnm::word{"LOAD", "$", 21},
            vnm::word{"STORE", "$", 18},
            vnm::word{"LOAD", "$", 21},
            vnm::word{"ADD", "@", 20},
            vnm::word{"SUB", "@", 18},
            vnm::word{"JZERO", "$", 15},
            vnm::word{"LOAD", "@", 19},
            vnm::word{"ADD", "&", 18},
            vnm::word{"STORE", "$", 19},
            vnm::word{"LOAD", "@", 18},
            vnm::word{"ADD", "$", 1},
            vnm::word{"STORE", "$", 18},
            vnm::word{"JUMP", "$", 4},
        };

        for (const auto& instruction : code)
        {
            machine.put_to_memory(instruction, vnm::word{static_cast<vnm::word::type>(i++)});
        }

        machine.put_to_memory(stop, vnm::word{static_cast<vnm::word::type>(i)});

        machine.put_to_memory(vnm::word{5}, vnm::word{20});
        machine.put_to_memory(vnm::word{10}, vnm::word{21});
        machine.put_to_memory(vnm::word{20}, vnm::word{22});
        machine.put_to_memory(vnm::word{30}, vnm::word{23});
        machine.put_to_memory(vnm::word{40}, vnm::word{24});
        machine.put_to_memory(vnm::word{50}, vnm::word{25});

        while (machine.execute())
        {
            machine.tick();
        }

        const auto memory_state = machine.get_memory();
        REQUIRE( memory_state.get(vnm::word{18}).get_complete_arg() == 26 );
        REQUIRE( memory_state.get(vnm::word{19}).get_complete_arg() == 150 );
    }

    SECTION( "double sum example" )
    {
        const std::vector<vnm::word> code = {
            vnm::word{"LOAD", "@", 5},
            vnm::word{"ADD", "@", 6},
            vnm::word{"MULT", "$", 2},
            vnm::word{"STORE", "$", 7},
        };

        for (const auto& instruction : code)
        {
            machine.put_to_memory(instruction, vnm::word{static_cast<vnm::word::type>(i++)});
        }

        machine.put_to_memory(stop, vnm::word{static_cast<vnm::word::type>(i)});

        machine.put_to_memory(vnm::word{21}, vnm::word{5});
        machine.put_to_memory(vnm::word{34}, vnm::word{6});
        machine.put_to_memory(vnm::word{0}, vnm::word{7});

        while (machine.execute())
        {
            machine.tick();
        }

        const auto memory_state = machine.get_memory();
        REQUIRE( memory_state.get(vnm::word{7}).get_complete_arg() == 110 );
    }
}
