#include "ability.hpp"
#include "player.hpp"


// TODO: work on abilities
void haste::execute() {
    this->me.set_summoning_sickness(false);
}

void lifelink::execute(player& controller) {
    controller.add_life(this->amount);
}

void trample::execute() {

}

void vigilance::execute() {

}

void deathtouch::execute() {

}

void flying::execute() {

}

void reach::execute() {

}

/*
void ability::haste(creature& attacker) {
    attacker.summoning_sickness = false;
};
void ability::lifelink(player& controller, int damage) {
    controller.add_life(damage);
};
void ability::trample(player& target, const creature& attacker, std::vector<creature>& defenders) {
    int sum_def = 0;
    for (creature& defender : defenders) {
        sum_def += defender.toughness;
    }
    if (attacker.power > sum_def) {
        target.add_life(-(attacker.power - sum_def));
    }
};
void ability::vigilance(creature& attacker) {

};
void ability::deathtouch() {};
void ability::flying() {};
void ability::reach() {};*/