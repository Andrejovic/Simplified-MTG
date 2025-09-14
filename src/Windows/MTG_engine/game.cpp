#include "game.hpp"
#include <random>

constexpr size_t STARTING_HAND_SIZE = 7;

/**
* roll dice to see who goes first
* @param name1 name of player 1
* @param name2 name of player 2
* 
* @returns true if player 1 wins
**/
bool roll_for_high(const std::string& name1, const std::string& name2)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 6);

    const int p1_roll1 = dis(gen);
    const int p1_roll2 = dis(gen);
    std::cout << name1 << " roll: " << p1_roll1 << "+" << p1_roll2 << "=" << (p1_roll1 + p1_roll2) << "\n";
    const int p2_roll1 = dis(gen);
    const int p2_roll2 = dis(gen);
    std::cout << name2 << " roll: " << p2_roll1 << "+" << p2_roll2 << "=" << (p2_roll1 + p2_roll2) << "\n\n";
    return (p1_roll1 + p1_roll2) > (p2_roll1 + p2_roll2);
}

/**
* resolve blocks
* @param attackers attackers in the form of vector
* @param blocks blocks in the form of map<attacker, vector<blockers>>
* 
* @returns damage
**/
int game::resolve_blocks(const std::vector<size_t>& attackers, const std::map<size_t, std::vector<size_t>>& blocks) {
    int damage = 0; //int because of healing in the future
    std::vector<size_t> order_of_blocks;
    for (auto&& [attacker, blockers] : blocks) {
        order_of_blocks.clear();
        if (blockers.size() == 0) {
		    damage += static_cast<creature*>(game::get_active_player()->get_battlefield()[attacker].get())->get_power();
        } else if (blockers.size() >= 1) {
            int toughness = static_cast<creature*>(game::get_active_player()->get_battlefield()[attacker].get())->get_toughness();
            int power = static_cast<creature*>(game::get_active_player()->get_battlefield()[attacker].get())->get_power();
            int block_toughness = 0;
            int block_power = 0;
            for (auto&& blocker : blockers) {
                block_toughness += static_cast<creature*>(game::get_non_active_player()->get_battlefield()[blocker].get())->get_toughness();
                block_power += static_cast<creature*>(game::get_non_active_player()->get_battlefield()[blocker].get())->get_power();
            }

            if (power >= block_toughness) {
				for (auto&& blocker : blockers) {
                    static_cast<creature*>(game::get_non_active_player()->get_battlefield()[blocker].get())->deal_damage(power);
				}
			} else if ((power < block_toughness) && (blockers.size() > 1)) {
                order_of_blocks = game::get_active_player()->select_order_of_blockers(attacker, blockers);
                for (auto&& blocker : order_of_blocks) {
                    if (power == 0) {
                        break;
                    }
                    int single_toughness = static_cast<creature*>(game::get_non_active_player()->get_battlefield()[blocker].get())->get_toughness();
                    if (power >= single_toughness) {
                        static_cast<creature*>(game::get_non_active_player()->get_battlefield()[blocker].get())->deal_damage(single_toughness);
                        power -= single_toughness;
                    }
                }
			} // else, blockers survive

            if (toughness <= block_power) {
                static_cast<creature*>(game::get_active_player()->get_battlefield()[attacker].get())->deal_damage(block_power);
			} // else, attacker survives
        }
    }
    return 0;
}

/**
* check for deaths of creatures on the battlefield
**/
void game::check_deaths() {

    for (size_t i = 0; i < active_player->get_battlefield().size(); i++) {
        auto& card = active_player->get_battlefield()[i];
        if (card->get_type() == "creature" && static_cast<creature*>(card.get())->get_dead()) {
            active_player->send_to_graveyard(card.get());
            i--;
        }
    }

    for (size_t i = 0; i < non_active_player->get_battlefield().size(); i++) {
        auto& card = non_active_player->get_battlefield()[i];
        if (card->get_type() == "creature" && static_cast<creature*>(card.get())->get_dead()) {
            non_active_player->send_to_graveyard(card.get());
            i--;
        }
    }
}

/**
* game starter - shuffles decks, gives cards and opportunity to mulligan
**/
void game::start_game() {
    p1.shuffle();
    p2.shuffle();
    std::string p1_mulligan, p2_mulligan;
    p1.draw_card(STARTING_HAND_SIZE);
    p2.draw_card(STARTING_HAND_SIZE);
    p1.display_hand();
    std::cout << p1.get_damagable_name() << " mulligan? y/N\n";
    std::getline(std::cin, p1_mulligan);
    if (p1_mulligan == "Y" || p1_mulligan == "y") p1.mulligan(STARTING_HAND_SIZE-1);
    p2.display_hand();
    std::cout << p2.get_damagable_name() << " mulligan? y/N\n";
    std::getline(std::cin, p2_mulligan);
    if (p2_mulligan == "Y" || p2_mulligan == "y") p2.mulligan(STARTING_HAND_SIZE-1);
}

/**
* main game loop
**/
void game::turn() {
    untap();
    upkeep();
    draw_step();
    main_phase(); // one main phase is enough
    combat();
    //main_phase();
    end_phase();
}

/**
* untap phase - untap all cards on the battlefield and remove summoning sickness from creatures
**/
void game::untap() {
    if (!this->ended) {
        if (!(active_player->get_battlefield().empty())) {
            for (auto&& card : active_player->get_battlefield()) {
                card->set_tapped(false);
                if (card->get_type() == "creature") {
					static_cast<creature*>(card.get())->set_summoning_sickness(false);
				}
            }
        }
    }
    // start of untap effects
}
void game::upkeep() {
    // start of upkeep, could just be end of untap
    // might skip
}

/**
* draw step - active player draws a card
**/
void game::draw_step() {
    if (!this->ended) {
        active_player->draw_card(1);
        if (active_player->get_life() <= 0) {
            this->ended = true;
        }
    }
}

/**
* main phase - active player plays max one land and any amount of non-land cards provided they have enough mana
**/
void game::main_phase() {
    if (!this->ended) {
        non_active_player->print_battlefield();
        active_player->display_hand();
        active_player->print_battlefield();
        bool main_phase_end = false;
        while (!main_phase_end) {
            main_phase_end = active_player->play(*non_active_player);
        }
        std::cout << "\n";

        if (active_player->get_life() <= 0 || non_active_player->get_life() <= 0) {
            this->ended = true;
        }

        check_deaths();
    }
}

/**
* combat step - active player attacks and non-active player blocks
**/
void game::combat() {
    if (!this->ended) {
        // start of combat effects
		bool selector_done = false;
        std::vector<size_t> attackers;
        std::map<size_t, std::vector<size_t>> blockers;

        while (!selector_done) {
            attackers = active_player->select_attackers();
            bool summoning_sickness = false;
            for (auto&& attacker : attackers) {
                if (static_cast<creature*>(active_player->get_battlefield()[attacker].get())->get_summoning_sickness()) {
                    std::cout << "one or more creatures you selected can't attack this turn\n";
                    summoning_sickness = true;
                    break;
                }
            }

			if (!summoning_sickness) selector_done = true;
        }

        for (auto&& attacker : attackers) {
			static_cast<creature*>(active_player->get_battlefield()[attacker].get())->set_tapped(true);
		}
        
        selector_done = false;
        if (!attackers.empty()) {
            while (!selector_done) {
                blockers = non_active_player->select_blockers(attackers, get_active_player());
                bool tapped = false;
                for (auto&& [_, blocker] : blockers) {
                    for (auto&& i : blocker) {
                        if (static_cast<creature*>(non_active_player->get_battlefield()[i].get())->is_tapped()) {
                            std::cout << "one or more creatures you selected can't block this turn\n";
                            tapped = true;
                            break;
                        }
                    }
                    if (tapped) break;
                }

                if (!tapped) selector_done = true;
            }
        }
        int damage = resolve_blocks(attackers, blockers);
        non_active_player->deal_damage(damage);
        std::cout << active_player->get_damagable_name() << " " << active_player->get_life() << "\n";
        std::cout << non_active_player->get_damagable_name() << " " << non_active_player->get_life() << "\n";
        check_deaths();
    }
}

/**
* end step - reset both players mana pools, creatures get healed, discard cards if needed and change who the active player is
**/
void game::end_phase() {
    if (!this->ended) {
        // start of end phase effects active_player->check_effects(ENUM end_phase)
        p1.empty_mana_pool();
        p2.empty_mana_pool();
        p1.heal_creatures();
        p2.heal_creatures();
        active_player->reset_played_land();
        if (active_player->get_hand().size() > STARTING_HAND_SIZE) {
            active_player->discard(active_player->get_hand().size() - STARTING_HAND_SIZE);
        }
        if (&p1 == active_player) { 
            active_player = &p2; 
            non_active_player = &p1;
        } else {
            active_player = &p1;
			non_active_player = &p2;
        }
    }
}
