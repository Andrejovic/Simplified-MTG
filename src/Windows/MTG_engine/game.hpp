#ifndef MTG_ENGINE_GAME_H
#define MTG_ENGINE_GAME_H

#include <string>
#include <map>
#include "player.hpp"
#include "deck.hpp"

constexpr int STARTING_LIFE = 20;

/**
* roll dice to see who goes first
* @param name1 name of player 1
* @param name2 name of player 2
*
* @returns true if player 1 wins
**/
bool roll_for_high(const std::string& name1, const std::string& name2);

class game
{
public:
	/**
	* game constructor
	* 
	* @param name1 name of player 1
	* @param name2 name of player 2
	* @param deck1 deck of player 1
	* @param deck2 deck of player 2
	**/
	game(const std::string& name1, const std::string& name2, deck&& deck1, deck&& deck2) : p1(name1, std::move(deck1), STARTING_LIFE), p2(name2, std::move(deck2), STARTING_LIFE) {
		if (roll_for_high(name1, name2)) { 
			active_player = &p1;
			non_active_player = &p2;
		} else {
			active_player = &p2;
			non_active_player = &p1;
		} 
	}

	/**
	* get active player
	* 
	* @returns active player
	**/
	player* get_active_player() { return active_player; }

	/**
	* get non-active player
	* 
	* @returns non-active player
	**/
	player* get_non_active_player() { return non_active_player; }

	/**
	* game starter - shuffles decks, gives cards and opportunity to mulligan
	**/
	void start_game();
	/**
	* main game loop
	**/
	void turn();

	/**
	* check if the game is ended
	* 
	* @returns true if the game is ended
	**/
	bool is_ended() const { return ended; }

	
	//int round = 0;
	

private:
	player p1;
	player p2;

	bool ended = false;

	player* active_player;
	player* non_active_player;

    void untap();
    void upkeep();
    void draw_step();
    void main_phase();
    void combat();
    void end_phase();

	/**
	* resolve blocks
	* @param attackers attackers in the form of vector
	* @param blocks blocks in the form of map<attacker, vector<blockers>>
	*
	* @returns damage
	**/
	int resolve_blocks(const std::vector<size_t>& attackers, const std::map<size_t, std::vector<size_t>>& blockers);
	/**
	* check for deaths of creatures on the battlefield
	**/
	void check_deaths();
};

#endif