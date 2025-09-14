#ifndef MTG_ENGINE_CARD_H
#define MTG_ENGINE_CARD_H

#include <string>
#include <utility>
#include <vector>
#include <map>
#include "effect.hpp"
#include "ability.hpp"
#include "damagable.hpp"
#include "effect_factory.hpp"

class card
{
public:
	card(std::string name, std::string type) : name(std::move(name)), type(std::move(type)) {}

	/**
	* taps the card
	**/
	void tap() {
		tapped = true;
	}

	/**
	* untaps the card
	**/
	void untap() {
		tapped = false;
	}

	/**
	* checks if two cards are the same
	* @param other card to check
	* 
	* @return true if the cards are the same
	**/
	bool operator==(const card& other) const {
		return (name == other.name
			&& type == other.type);
	}

	virtual ~card() = default;

	/**
	* get the name of the card
	* 
	* @returns name
	**/
	std::string get_name() {
		return name;
	}

	/**
	* get the type of the card
	* 
	* @returns type
	**/
	std::string get_type() {
		return type;
	}

	/**
	* checks if the card is tapped
	* 
	* @returns true if the card is tapped
	**/
	bool is_tapped() const {
		return tapped;
	}

	/**
	* set the tapped state of the card
	* 
	* @param set_to
	**/
	void set_tapped(bool set_to) {
		tapped = set_to;
	}

	/**
	* get the owner of the card
	* 
	* @returns owner
	**/
	player* get_owner() {
		return owner;
	}

	/**
	* set the owner of the card
	* @param this_owner
	**/
	void set_owner(player* this_owner) {
		owner = this_owner;
	}

private:
	std::string name;
	std::string type;
	bool tapped = false;
	player* owner;
};

class land : public card
{
public:
	land(const std::string& name, const std::string& type, const std::string& subtype, std::string color_string) : card(name, type) {
		taps_for = string_to_color_map(color_string);
	}
	
	/**
	* get the subtype of the land
	* 
	* @returns subtype
	**/
	std::string get_subtype() {
		return subtype;
	}

	/**
	* get the color of the land
	* 
	* @returns color
	**/
	color get_taps_for() const {
		return taps_for;
	}

	std::map<std::string, std::function<void(effect*)>> my_effect;

private:
	std::string subtype;
	color taps_for;
};

class spell : public card
{
public:

	//// solved some problems
	// Explicitly define move constructor
	spell(spell&& other) noexcept = default;
	// Explicitly define move assignment operator
	spell& operator=(spell&& other) noexcept = default;
	// Explicitly delete copy constructor and copy assignment
	spell(const spell&) = delete;
	spell& operator=(const spell&) = delete;

	spell(const std::string&  name, const std::string&  type, std::string cost) : card(name, type), cost(std::move(cost)){}

	/**
	* get the cost of the spell
	* 
	* @returns cost
	**/
	std::string get_cost() {
		return cost;
	}

	/**
	* get the effects of the spell
	* 
	* @returns effects
	**/
	std::vector<std::unique_ptr<effect>>& get_effects() {
		return my_effects;
	}

protected:
	std::vector<std::unique_ptr<effect>> my_effects;
	

private:
	std::string cost;
};

class instant : public spell
{
public:
	instant(const std::string& name, const std::string& type, std::string cost, std::string effects) : spell(name, type, cost) {
		my_effects.push_back(std::move(effect_factory::create_effect(effects)));
	}

	~instant() = default;
};

class sorcery : public spell
{
public:
	sorcery(const std::string& name, const std::string& type, std::string cost, std::string effects) : spell(name, type, cost) {
		my_effects.push_back(std::move(effect_factory::create_effect(effects)));
	}

	~sorcery() = default;
};

class permanent : public spell
{
public:
    permanent(const std::string& type, const std::string&  name, const std::string&  cost) : spell(name,type,cost) {}
};

class creature : public permanent, public damagable
{
public:
    creature(const std::string &name, const std::string &type,
		std::string subtype, const std::string &cost, 
		const int power, const int toughness, std::string abilities, std::string effects
		) : permanent(type, name, cost), power(power), toughness(toughness), subtype(std::move(subtype)){
        health = toughness;
		my_effects.push_back(std::move(effect_factory::create_effect(effects)));
    }

	/**
	* get the name of the creature
	* 
	* @returns name
	**/
	std::string get_damagable_name() override {
		return get_name();
	}

	/**
	* deals damage to the creature
	* @param amount
	**/
	void deal_damage(int amount) override {
		health -= amount;
		if (health <= 0) {
			dead = true;
		}
	}

	
    std::map<std::string, std::function<void(ability*)>> my_ability;

	/**
	* get the subtype of the creature
	* 
	* @returns subtype
	**/
	std::string get_subtype() {
		return subtype;
	}

	/**
	* get the power of the creature
	* 
	* @returns power
	**/
	int get_power() const {
		return power;
	}

	/**
	* get the toughness of the creature
	* 
	* @returns toughness
	**/
	int get_toughness() const {
		return toughness;
	}

	/**
	* get the health (different from toughness in that toughness is static and the starting value to which it gets healed) of the creature
	* 
	* @returns health
	**/
	int get_health() const {
		return health;
	}

	/**
	* get the summoning sickness of the creature
	* 
	* @returns true if the creature has summoning sickness
	**/
	bool get_summoning_sickness() const {
		return summoning_sickness;
	}

	/**
	* set the summoning sickness of the creature
	* 
	* @param set_to
	**/
	void set_summoning_sickness(bool set_to) {
		summoning_sickness = set_to;
	}

	/**
	* set the health of the creature
	* 
	* @param set_to
	**/
	void set_health(int set_to) {
		health = set_to;
	}

	/**
	* get the dead state of the creature
	* 
	* @returns true if the creature is dead
	**/
	bool get_dead() const {
		return dead;
	}

private:
	bool summoning_sickness = true;
	int power;
	int toughness;
	int health;
	std::string subtype;
	bool dead = false;
};

class enchantment : public permanent
{
public:
	//effect my_effect;
};

#endif