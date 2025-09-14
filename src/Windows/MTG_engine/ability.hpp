#ifndef MTG_ENGINE_ABILITY_CPP
#define MTG_ENGINE_ABILITY_CPP

#include <string>
#include <functional>
#include "color.hpp"
#include "effect.hpp"

class player;
class creature;
class permanent;

class ability
{
public:
	virtual ~ability() = default;
	virtual void execute() {};
	virtual void execute(player&) {};

	/*
	// in main phase
	static void haste(creature& attacker);

	// in combat phase
	static void lifelink(player& controller, int damage);
	static void trample(player& target, const creature& attacker, std::vector<creature>& defenders);
	static void vigilance(creature& attacker);
	static void deathtouch();

	// in block phase
	static void flying();
	static void reach();

	// maybe if i have time
	static void tap(permanent& target, effect& effect);
	*/
};

// TODO: work on abilities

class haste : public ability {
	creature& me;
public:
	haste(creature& me) : me(me) {}
	void execute() override;
};

class lifelink : public ability {
private:
	int amount;
public:
	lifelink(int amount) : amount(amount) {};
	void execute(player& controller) override;
};

class trample : public ability {
public:
	trample() {};
	void execute() override;
};

class vigilance : public ability {
public:
	vigilance() {};
	void execute() override;
};

class deathtouch : public ability {
public:
	deathtouch() {};
	void execute() override;
};

class flying : public ability {
public:
	flying() {};
	void execute() override;
};

class reach : public ability {
public:
	reach() {};
	void execute() override;
}; 
#endif