#ifndef MTG_ENGINE_DECK_H
#define MTG_ENGINE_DECK_H

#include "INI_parser.hpp"
#include "card_factory.hpp"

class deck {
public:
	/**
	* data of the deck
	**/
	IniParser::IniData data;

	/**
	* library of the deck
	**/
	std::vector<std::unique_ptr<card>> library;

	/**
	* deck constructor
	* @param deck_data data of the deck
	**/
	deck(IniParser::IniData deck_data) : data(deck_data) {
		card_factory factory;
		for (auto&& card : deck_data) {
			library.push_back(std::move(factory.create_card(card.second["Type"], card.second)));
		}
	}

	~deck() = default;
};

#endif //MTG_ENGINE_DECK_H