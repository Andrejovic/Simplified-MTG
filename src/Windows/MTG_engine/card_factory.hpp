#ifndef MTG_ENGINE_CARD_FACTORY_H
#define MTG_ENGINE_CARD_FACTORY_H

#include <functional>
#include <memory>
#include <unordered_map>
#include "card.hpp"

const std::string PLACEHOLDER = "";

class card_factory {
	using fun_map = const std::map<std::string, std::string>&;
	using creator_func = std::function<std::unique_ptr<card>(const std::map<std::string, std::string>&)>;

	/**
	* Registry of card creators for create_card
	**/
	std::unordered_map<std::string, creator_func> registry;

public:
	/**
	* Default constructor
	**/
	card_factory() {
		registry["creature"] = [](const std::map<std::string, std::string>& args) {
			auto ab_it = args.find("Ability");
			auto ef_it = args.find("Effect");

			if (ab_it == args.end() && ef_it == args.end()) {
				return std::make_unique<creature>(args.at("Name"), args.at("Type"), args.at("Subtype"), args.at("ManaCost"), std::stoi(args.at("Power")), std::stoi(args.at("Toughness")), PLACEHOLDER, PLACEHOLDER);
			} else if (ef_it == args.end()) {
				return std::make_unique<creature>(args.at("Name"), args.at("Type"), args.at("Subtype"), args.at("ManaCost"), std::stoi(args.at("Power")), std::stoi(args.at("Toughness")), args.at("Ability"), PLACEHOLDER);
			} else if (ab_it == args.end()) {
				return std::make_unique<creature>(args.at("Name"), args.at("Type"), args.at("Subtype"), args.at("ManaCost"), std::stoi(args.at("Power")), std::stoi(args.at("Toughness")), PLACEHOLDER, args.at("Effect"));
			} else {
				return std::make_unique<creature>(args.at("Name"), args.at("Type"), args.at("Subtype"), args.at("ManaCost"), std::stoi(args.at("Power")), std::stoi(args.at("Toughness")), args.at("Ability"), args.at("Effect"));
			}
		};
		registry["instant"] = [](const std::map<std::string, std::string>& args) {
			return std::make_unique<instant>(args.at("Name"), args.at("Type"), args.at("ManaCost"), args.at("Effect"));
		};
		registry["sorcery"] = [](fun_map args) {
			return std::make_unique<sorcery>(args.at("Name"), args.at("Type"), args.at("ManaCost"), args.at("Effect"));
		};
		registry["land"] = [](fun_map args) {
			return std::make_unique<land>(args.at("Name"), args.at("Type"), args.at("Subtype"), args.at("Colors"));
		};
	}

	/**
	* Creates a card of the specified type
	* @param card_type
	* @param args
	* 
	* @returns a unique pointer to the created card
	**/
	std::unique_ptr<card> create_card(const std::string& card_type, fun_map args) {
		auto it = registry.find(card_type);
		if (it != registry.end()) {
			return it->second(args);
		}
		return nullptr;
	}
};

#endif //MTG_ENGINE_CARD_FACTORY_H