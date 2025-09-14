// MTG_engine.cpp : Defines the entry point for the application.
//

#include <iostream>
#include "game.hpp"
using namespace std;

int main()
{
	string player1_name, player2_name;
	cout << "Enter player names: ";

    std::getline(std::cin, player1_name);
	std::getline(std::cin, player2_name);

    IniParser parser;
    IniParser::IniData deck1 = parser.parseIniFile("..\\..\\..\\..\\decks\\red.ini");
    IniParser::IniData deck2 = parser.parseIniFile("..\\..\\..\\..\\decks\\red.ini");

    deck deck_1(deck1);
    deck deck_2(deck2);

	game game(player1_name, player2_name, std::move(deck_1), std::move(deck_2));
    game.start_game();
    while (!game.is_ended()) {
        game.turn();
    }
    string winner = (game.get_active_player()->get_life()) > 0 ? game.get_active_player()->get_damagable_name() : game.get_non_active_player()->get_damagable_name();
    std::cout << winner << " won the game!";

    std::cin.get();
    std::exit(0);
}
