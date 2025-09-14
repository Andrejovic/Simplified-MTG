# CMakeList.txt : CMake project for MTG_engine, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (MTG_engine "MTG_engine.cpp"  "card.hpp"  "effect.hpp" "effect.cpp" "game.hpp" "INI_parser.hpp"
						"INI_parser.cpp" "color.hpp" "types.hpp" "game.cpp" "deck.hpp" "card_factory.hpp" 
						"effect_factory.hpp" "ability.hpp" "ability.cpp" "damagable.hpp" "effect_factory.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET MTG_engine PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
