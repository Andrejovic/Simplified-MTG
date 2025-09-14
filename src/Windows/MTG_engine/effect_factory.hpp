#ifndef MTG_ENGINE_EFFECT_FACTORY_H
#define MTG_ENGINE_EFFECT_FACTORY_H

#include <memory>
#include "effect.hpp"

class effect_factory {
public:
	/**
	* creates an effect from a string representation
	* 
	* @param effect_string the string representation of the effect
	* 
	* @returns the effect
	**/
	static std::unique_ptr<effect> create_effect(const std::string& effect_string);
};


#endif // !MTG_ENGINE_EFFECT_FACTORY_H
