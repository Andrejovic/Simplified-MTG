#ifndef MTG_ENGINE_PLAYER_H
#define MTG_ENGINE_PLAYER_H

#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <memory>
#include <random>
#include <ranges>
#include <sstream>
#include <algorithm>

#include "deck.hpp"
#include "phase.hpp"
#include "damagable.hpp"


class player : public damagable
{
public:
    /**
    * player constructor
    * @param name name of the player
    * @param my_deck deck of the player
    * @returns player
    **/
	player(const std::string& name,deck&& my_deck, int starting_life) : name(name), library(std::move(my_deck.library)), life(starting_life) {
        for (auto& card : library) {
			card->set_owner(this);
		}
    }

    // TODO: deletion causes leaks for some reason
    ~player() {
        for (auto& card : library) {
            card.reset();
        }
        for (auto& card : graveyard) {
			card.reset();
		}
		for (auto& card : hand) {
			card.reset();
		}
		for (auto& card : battlefield) {
			card.reset();
		}
    }

    /**
    * get the name of the player
    * @returns name
    **/
    std::string get_damagable_name() override {
        return name;
    }

    /**
    * deal damage to the player
    * @param amount amount of damage
    **/
    void deal_damage(int amount) override {
        life -= amount;
    }

    /**
    * get the life of the player
    * @returns life
    **/
    int get_life() const {
        return life;
    }

	/**
    * add life to the player
    * @param add amount of life
    **/
    void add_life(int add) {
        life += add;
    }

    /**
    * get the library of the player
    * @returns library
    **/
    std::vector<std::unique_ptr<card>>& get_library() { 
        return library; 
    }

	/**
    * get the graveyard of the player
    * @returns graveyard
    **/
    std::vector<std::unique_ptr<card>>& get_graveyard() { 
        return graveyard; 
    }

	/**
    * get the hand of the player
    * @returns hand
    **/
    std::vector<std::unique_ptr<card>>& get_hand() { 
        return hand; 
    }

	/**
    * get the battlefield of the player
    * @returns battlefield
    **/
    std::vector<std::unique_ptr<card>>& get_battlefield() { 
        return battlefield; 
    }

    /**
    * get the mana pool of the player into a string ready to be printed 
    * @returns mana pool
    **/
    std::string print_mana_pool() const {
        std::string mana_pool_string = "";
		for (auto& [color, count] : mana_pool) {
			mana_pool_string += std::to_string(count) + " " + color_to_string(color) + " ";
		}
        return mana_pool_string;
    }

    /**
    * get the battlefield of the player into a string ready to be printed 
    * @returns battlefield
    **/
    void print_battlefield() {
        std::cout << "Battlefield of " << this->get_damagable_name() << ":\n";
        for (auto&& smt : battlefield) {
            std::cout << smt->get_name() << " - " << smt->get_type() << " - " << (smt->is_tapped() ? "tapped" : "untapped");
            if (smt->get_type() == "creature") {
				std::cout << "(" << (static_cast<creature*>(smt.get())->get_summoning_sickness() ? "ss" : "nss" ) << ") - " << static_cast<creature*>(smt.get())->get_power() << " - " << static_cast<creature*>(smt.get())->get_health();
			}
			std::cout << "\n";
        }
		std::cout << "\n";
    }

    /**
    * get the graveyard of the player into a string ready to be printed
    * @returns graveyard
    **/
    void print_graveyard() {
        std::cout << "Graveyard of " << this->get_damagable_name() << ":\n";
        for (auto&& smt : graveyard) {
            std::cout << smt->get_name() << " - " << smt->get_type() << "\n";
        }
		std::cout << "\n";
    }

    /**
    * add mana to the mana pool
    * @param color_of_mana color of the mana
    **/
    void add_to_mana_pool(color color_of_mana) { mana_pool[color_of_mana]++; }

    /**
    * shuffle the library of the player, uses internal library
    **/
    void shuffle() {
        std::shuffle(library.begin(), library.end(), std::mt19937(std::random_device()()));
    }               

    /**
    * mulligan the player
    * @param n_of_cards number of cards to draw after mulligan
    **/
    void mulligan(const size_t n_of_cards) {
        for (size_t i = 0; i < this->hand.size(); i++) {
            library.push_back(std::move(hand[i]));
        }
        hand.clear();
        shuffle();
        std::string player_mulligan;
        draw_card(n_of_cards);
        display_hand();
        std::cout << this->name << " mulligan? y/N\n";
        std::getline(std::cin, player_mulligan);
        if ((player_mulligan == "Y" || player_mulligan == "y") && (n_of_cards > 1)) mulligan(n_of_cards - 1);
        else if (n_of_cards == 1) std::cout << "You can't mulligan anymore, you have 1 card in your hand\n\n";
    }

    /**
    * draw n_of_cards from the library and add them to the hand
    * kills the player if he tries to draw a card from an empty library
    * @param n_of_cards number of cards to draw
    **/
    void draw_card(const size_t n_of_cards) {
        if (n_of_cards > library.size()) {
            this->add_life(-999999999);
            return;
        }
        for (size_t i = 0; i < n_of_cards; i++) {
            hand.push_back(std::move(library.back()));
            library.pop_back();
        }
    }

    /**
    * discard n_of_cards from the hand
    * @param n_of_cards number of cards to discard
    **/
    void discard(const size_t n_of_cards) {
        if (n_of_cards > hand.size()) {
            for (auto&& card : hand) {
                graveyard.push_back(std::move(card));
            }
            return;
        }
        for (size_t i = 0; i < n_of_cards; i++) {
            std::cout << "Select a card to discard: \n";
            for (size_t j = 0; j < hand.size(); j++) {
                std::cout << j << ": " << hand[j]->get_name() << "\n";
            }
            int choice = 0;
            if (std::cin >> choice) {
                graveyard.push_back(std::move(hand[choice]));
                hand.erase(hand.begin() + choice);
            } else {
                i--;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }
    }

    /**
    * display the hand of the player
    **/
    void display_hand() {
        std::cout << this->get_damagable_name() << "\n";
        for (auto&& card : hand) {
            std::cout << card->get_name() << " - " << card->get_type();
            if (card->get_type() != "land") {
                std::cout << " - " << static_cast<spell*>(card.get())->get_cost();
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }

    /**
    * select creatures you want to attack with
    * @returns selected creatures in the form of a vector of indices
    **/
    std::vector<size_t> select_attackers() {
        if (battlefield.size() == 0) return std::vector<size_t>();
		std::cout << this->get_damagable_name() << " Select attackers: \n";
		for (size_t i = 0; i < battlefield.size(); i++) {
			std::cout << i + 1 << ": " << battlefield[i]->get_name() << "\n";
		}

        return selector();
    }

    /**
    * select creatures you want to block with
    * @param attackers vector of indices of creatures you want to block
    * @param opponent pointer to the opponent
    * 
    * @returns selected creatures in the form of a map of attackers to vector of indices of blockers
    **/
    std::map<size_t, std::vector<size_t>> select_blockers(std::vector<size_t> attackers, player* opponent) {
        std::map<size_t, std::vector<size_t>> ret;
        for (size_t i = 0; i < battlefield.size(); i++) {
            std::cout << i + 1 << ": " << battlefield[i]->get_name() << "\n";
        }
        std::cout << this->get_damagable_name() << " Select blocker/s for each attacker: \n";
        for (auto&& attacker : attackers) {
			std::cout << opponent->get_battlefield()[attacker]->get_name() << "\n";
            ret[attacker] = selector();
        }
        return ret;
    }

    /**
    * select the order you want to battle the blocking creatures as the attacker
    * @param attacker index of the attacker
    * @param blockers vector of indices of creatures that are blocking
    * 
    * @returns selected order in the form of a vector of indices
    **/
    std::vector<size_t> select_order_of_blockers(size_t attacker, std::vector<size_t> blockers) {
        std::cout << this->get_damagable_name() << " Select order of blockers for " << battlefield[attacker]->get_name() << "\n";
        bool correct_input = false;
        std::vector<size_t> ret;
        while (!correct_input) {
            ret = selector();
            if (ret.size() == blockers.size() && std::includes(ret.begin(), ret.end(), blockers.begin(), blockers.end())) { // includes in this case means that all elements of blockers are in ret
                correct_input = true;
            } else {
                std::cout << "Incorrect input\n";
            }
        }
		return ret;
    }

    /**
    * selector of indices from a list to create a vector
    * @returns selected creatures in the form of a vector of indices
    **/
    std::vector<size_t> selector() {
        std::string choice;
        std::getline(std::cin, choice);

        std::istringstream iss(choice);
        std::vector<size_t> ret;

        size_t num;
        while (iss >> num) {
            ret.push_back(num - 1);
        }
        return ret;
    }

    /**
    * empty the mana pool
    **/
    void empty_mana_pool() {
        for (const auto& key : mana_pool | std::views::keys) {
            mana_pool[key] = 0;
        }
    }

    /**
    * heal all creatures
    **/
    void heal_creatures() const
    {
        for (auto&& card : battlefield) {
            auto creature_ = static_cast<creature*>(card.get());
            if (creature_ != nullptr) {
                creature_->set_health(creature_->get_toughness());
            }
        }
    }

    /**
    * send a permanent to the graveyard from the battlefield
    * @param target permanent reference to send
    **/
    void send_to_graveyard(const permanent& target) {
        move_card_from_target_to_target(target, battlefield, graveyard);
    }

    /**
    * send a card to the graveyard from the battlefield
    * @param target card pointer to send
    **/
    void send_to_graveyard(card* target) {
        auto found = std::find_if(battlefield.begin(), battlefield.end(),
			[&target](const std::unique_ptr<card>& cardPtr) {
				return *cardPtr == *target;
			});

		move_from_vector_to_another(found, battlefield, graveyard);
    }

    /**
    * play a turn from the command line with some options
    * @param opponent player for reference
    * 
    * @returns true if the player ended his main phase
    **/
    bool play(player& opponent) {
        std::string action;
        std::getline(std::cin, action);
        if (action == "concede") {
            concede();
            return true;
        } else if (action == "pass") {
            pass_turn();
            return true;
        } else if (action == "hand") {
			display_hand();
        } else if (action == "battlefield") {
            print_battlefield();
        } else if (action == "graveyard") {
            print_graveyard();
        } else {
            return decode_play(action, opponent);
        }
        return false;
    }

    static void check_effects(phase start_of) {
        // TODO: do this (actually might not be necessary)
    }

    /**
    * reset the played_land flag for the next turn
    **/
    void reset_played_land() {
        played_land = false;
    }

private:
	std::string name;
	int life = 0;
    bool played_land = false;

    std::map<color, int> mana_pool;

    std::vector<std::unique_ptr<card>> library;
    std::vector<std::unique_ptr<card>> graveyard;
    std::vector<std::unique_ptr<card>> hand;
    std::vector<std::unique_ptr<card>> battlefield;

    void pass_turn() {
        std::cout << this->get_damagable_name() << " passed their current phase\n";
    }
    void concede() {
        std::cout << this->get_damagable_name() << " conceded\n";
        deal_damage(INT_MAX);
    }

    bool decode_play(std::string& play, player& opponent) {
        size_t offset_to_space = play.find(" ");
        if (offset_to_space != std::string::npos) {
            if (play.substr(0,offset_to_space) == "tap") {
                std::string name = play.substr(offset_to_space+1);
                for (auto&& smt : battlefield) {
                    if (smt->get_name() == name && !smt->is_tapped() && smt->get_type() == "land") {
                        smt->set_tapped(true);
                        mana_pool[static_cast<land*>(smt.get())->get_taps_for()]++;
                        break;
                    }
                }
                std::cout << this->print_mana_pool() << "\n";
            } else if (play.substr(0,offset_to_space) == "play" || play.substr(0,offset_to_space) == "cast") {
                return cast_card(play.substr(offset_to_space + 1), opponent);
            }
        }
        return false;
    }

    bool cast_card(std::string name, player& opponent) {
        std::string cost;
        for (auto&& smt : hand) {
            if (smt->get_name() == name && smt->get_type() != "land") {
                cost = static_cast<spell*>(smt.get())->get_cost();
                if (!pay(cost)) {
                    return false;
                }

                auto found = std::find_if(hand.begin(), hand.end(),
                    [&smt](const std::unique_ptr<card>& cardPtr) {
                        return *cardPtr == *smt;
                    });

                if (smt->get_type() == "creature") {
                    if (found != hand.end()) {
                        do_effects(smt, opponent);
                        battlefield.push_back(std::move(*found));
                        std::iter_swap(found, hand.end() - 1);
                        hand.pop_back();
                    }
                }
                else {
                    if (found != hand.end()) {
                        do_effects(smt, opponent);
                        graveyard.push_back(std::move(*found));
                        std::iter_swap(found, hand.end() - 1);
                        hand.pop_back();
                    }
                }
                break;
            }
            else if (smt->get_name() == name && smt->get_type() == "land") {
                if (played_land) {
                    std::cout << "you can only play one land per turn\n";
                    return false;
                }

                move_card_from_target_to_target(*smt, hand, battlefield);
                played_land = true;
                break;
            }
        }
        print_battlefield();
        return false;
    }

    bool pay(std::string& cost) {
        if (can_pay(mana_pool, cost)) {
            // for now, counting on the player to tap precisely how he needs and not more
            empty_mana_pool();
            return true;
        } else {
            std::cout << "You can't pay for that with your current mana pool\n";
            return false;
        }
    }
	
    void move_card_from_target_to_target(const card& target, std::vector<std::unique_ptr<card>>& source_vector, std::vector<std::unique_ptr<card>>& target_vector) {
        auto found = std::find_if(source_vector.begin(), source_vector.end(),
			[&target](const std::unique_ptr<card>& cardPtr) {
				return *cardPtr == target;
			});

		move_from_vector_to_another(found, source_vector, target_vector);
    }

    void move_from_vector_to_another(std::vector<std::unique_ptr<card>>::iterator found ,std::vector<std::unique_ptr<card>>& source_vector, std::vector<std::unique_ptr<card>>& target_vector) {
        if (found != source_vector.end()) {
            target_vector.push_back(std::move(*found));
            std::iter_swap(found, source_vector.end() - 1);
            source_vector.pop_back();
        }
    }

    void destroy(const card& target) {
        move_card_from_target_to_target(target, battlefield, graveyard);
    }

    bool can_pay(std::map<color, int> mana_pool, std::string& cost) {
        int any = 0;
        if (isdigit(cost[0])) {
            any = cost[0] - '0';
            cost = cost.substr(1);
        }

        std::map<color, int> mana_pool_local = mana_pool;

		for (auto&& ch : cost) {
			if (isalpha(ch)) {
				if (mana_pool_local[char_to_color_map(ch)] < 1) {
					return false;
				}
				mana_pool_local[char_to_color_map(ch)] -= 1;
			}
		}

        int mana = 0;
        for (auto&& [color, count] : mana_pool_local) {
			mana += count;
		}
		if (mana >= any) {
			return true;
		}

        return false;
    }

    void do_effects(std::unique_ptr<card>& smt, player& opponent) {
        auto& effects = static_cast<creature*>(&*smt)->get_effects();
        for (auto&& eff : effects) {
            if (eff->get_name() == "nothing") {
                continue;
            }
            std::cout << "Enter target for effect " << eff->get_name() << " : ";
            std::string target;
            std::getline(std::cin, target);
            auto&& target_ptr = parse_target(target, opponent);
            eff->execute(target_ptr);
        }
    }

    damagable& parse_target(std::string& target, player& opponent) {
        if (target == "me" || target == this->name) {
            return *this;
        } else if (target == "opp" || target == opponent.name) {
            return opponent;
        } else {
            std::string owner = target.substr(0, target.find("-"));
            std::string name = target.substr(target.find("-") + 1);
            if (owner == "me") {
                for (auto&& card : battlefield) {
					if (card->get_name() == name) {
						return static_cast<creature&>(*card);
					}
				}
            } else {
                for (auto&& card : opponent.battlefield) {
                    if (card->get_name() == name) {
                        return static_cast<creature&>(*card);
                    }
                }
            }
        }
    }
};

#endif