#ifndef MTG_ENGINE_EFFECT_CPP
#define MTG_ENGINE_EFFECT_CPP

#include <string>
#include <functional>
#include <iostream>
#include "color.hpp"

/* static void destroy_permanent(player& owner, permanent& target);
	static void deal_damage(creature& target, int damage);
	static void deal_damage(player& target, int damage);
	static void heal(player& target, int heal);
	static void draw(player& target, int n_of_cards);
	static void discard(player& target, int n_of_cards);
	static void add_mana_into_pool(player& target,int n_of_mana, color color_of_mana); */
	//static void add_counters(permanent& target ,int n_of_counters);

class player;
class creature;
class permanent;
class damagable;

// effects are things that happen when a card is played
class effect
{
public:
	virtual ~effect() = default;
	/**
	* executes the effect
	* 
	* @param target the target of the effect
	**/
	virtual void execute(damagable& target) = 0;

	/**
	* returns the name of the effect
	*
	* @returns name
	**/
	std::string get_name() {return name;}
protected:
	/**
	* name of the effect
	**/
	std::string name = " ";
};

class destroy_permanent : public effect {
public:
	~destroy_permanent() = default;
	destroy_permanent() {name = "destroy permanent";};

	void execute(damagable& target) override;
};

class deal_damage : public effect {
private:
	int amount;
public:
	~deal_damage() = default;
	deal_damage(int amount) : amount(amount) {name = "deal damage";};

	void execute(damagable& target) override;
};

class heal : public effect {
private:
	int amount;
public:
	~heal() = default;
	heal(int amount) : amount(amount) {name = "heal";};

	void execute(damagable& target) override;
};

class draw_card : public effect {
private:
	int amount;
public:
	~draw_card() = default;
	draw_card(int amount) : amount(amount) {name = "draw card";};

	void execute(damagable& target) override;
};

class discard : public effect {
private:
	int amount;
public:
	~discard() = default;
	discard(int amount) : amount(amount) {name = "discard card";};

	void execute(damagable& target) override;
};

class nothing : public effect {
public:
	~nothing() = default;
	nothing() {name = "nothing";};

	void execute(damagable& target) override {};
};

/*
class add_mana_into_pool : public effect {
private:
	int amount;
	color color_of_mana;
public:
	~add_mana_into_pool() = default;
	add_mana_into_pool() {}

	void execute(damagable& target) override;
};*/

#endif

