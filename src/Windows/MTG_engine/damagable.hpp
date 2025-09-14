#ifndef MTG_ENGINE_DAMAGABLE_CPP
#define MTG_ENGINE_DAMAGABLE_CPP

#include <string>

class damagable {
public:
	virtual ~damagable() = default;

	/**
	* deal damage to this
	* @param amount
	**/
	virtual void deal_damage(int amount) = 0;

	/**
	* get the name of this
	* @returns name
	**/
	virtual std::string get_damagable_name() = 0;
};

#endif // !MTG_ENGINE_DAMAGABLE_CPP
