#include "effect_factory.hpp"

std::unique_ptr<effect> effect_factory::create_effect(const std::string& effect_string) {
	if (effect_string.find("damage_target") != std::string::npos) {
		size_t start = effect_string.find('(') + 1;
		size_t end = effect_string.find(')', start);
		int amount = std::stoi(effect_string.substr(start, end - start));
		return std::make_unique<deal_damage>(amount);
	}
	else if (effect_string.find("discard_opponent") != std::string::npos) {
		size_t start = effect_string.find('(') + 1;
		size_t end = effect_string.find(')', start);
		int amount = std::stoi(effect_string.substr(start, end - start));
		return std::make_unique<discard>(amount);
	}
	else if (effect_string.find("draw") != std::string::npos) {
		size_t start = effect_string.find('(') + 1;
		size_t end = effect_string.find(')', start);
		int amount = std::stoi(effect_string.substr(start, end - start));
		return std::make_unique<draw_card>(amount);
	}
	else if (effect_string.find("damage_opponent") != std::string::npos) {
		size_t start = effect_string.find('(') + 1;
		size_t end = effect_string.find(')', start);
		int amount = std::stoi(effect_string.substr(start, end - start));
		return std::make_unique<deal_damage>(amount);
	}
	else {
		return std::make_unique<nothing>();
	}
}