#include "effect.hpp"
#include "player.hpp"

void destroy_permanent::execute(damagable& target) {
    player* owner = static_cast<creature&>(target).get_owner();
    std::cout << "Destroy permanent: " << target.get_damagable_name() << "\n";
    owner->send_to_graveyard(static_cast<creature&>(target));
}

void deal_damage::execute(damagable& target) {
    std::cout << "Deal " << this->amount << " damage to target " << target.get_damagable_name() << "\n";
    target.deal_damage(this->amount);
}

void heal::execute(damagable& target) {
    std::cout << "Heal " << this->amount << " life to target " << target.get_damagable_name() << "\n";
    target.deal_damage(-(this->amount));
}

void draw_card::execute(damagable& target) {
    std::cout << target.get_damagable_name() << " draws " << this->amount << "cards\n";
    static_cast<player&>(target).draw_card(this->amount);
}

void discard::execute(damagable& target) {
    std::cout << target.get_damagable_name() << " discards " << this->amount << "cards\n";
    static_cast<player&>(target).discard(this->amount);
}

/*
void add_mana_into_pool::execute(damagable& target) {
    std::cout << target.get_damagable_name() << " added " << this->amount << " " << color_to_string(this->color_of_mana) << "\n";
}*/

//void effect::add_counters( permanent& target ,int n_of_counters) {};
