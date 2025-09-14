# MTG_engine Documentation

## Overview
This project is a game simulation for a card game inspired by Magic: The Gathering. It manages players, cards, and game mechanics.
The main concerns were to be expandable and flexible. The game can be extended to include more game modes, card types, and more complex game mechanics.


## Classes

1. **Player**  
   - **Description**: Represents a player in the game.  
   - **Methods**:  
     - `std::string get_damagable_name()`
     - `void deal_damage(int amount)`
     - `int get_life()`
     - `void add_life(int add)`
     - `std::vector<std::unique_ptr<card>>& get_library()`
     - `std::vector<std::unique_ptr<card>>& get_graveyard()`
     - `std::vector<std::unique_ptr<card>>& get_hand()`
     - `std::vector<std::unique_ptr<card>>& get_battlefield()`
     - `std::string print_mana_pool()`
     - `void print_battlefield()`
     - `void print_graveyard()`
     - `void add_to_mana_pool(color color_of_mana)`
     - `void shuffle()`  
     - `void mulligan(int n_of_cards)`  
     - `void draw(int n_of_cards)`  
     - `void discard(int n_of_cards)`
     - `void display_hand()`
     - `std::vector<size_t> select_attackers()`
     - `std::map<size_t, std::vector<size_t>> select_blockers(std::vector<size_t> attackers, player* opponent)`
     - `std::vector<size_t> select_order_of_blockers(size_t attacker, std::vector<size_t> blockers)`
     - `std::vector<size_t> selector()` : selector of indices from a list to create a vector
     - `void empty_mana_pool()` : Mana pool (tapped mana) doesn't stay in between rounds.
     - `void heal_creatures()` 
     - `void send_to_graveyard(const permanent& target)`
     - `void send_to_graveyard(card* target)`
     - `bool play(player& opponent)` : contains all the commands for main phase
     - `void reset_played_land()`

2. **Game**  
   - **Description**: Manages the game state and player turns
   - **Methods**:
     - `player* get_active_player()`
     - `player* get_non_active_player()`
     - `void start_game()`
     - `void turn()`
     - `bool is_ended()`

3. **Card**  (BASE CLASS)
   - **Description**: Base class for all cards.  
   - **Extendability**: To create more card types, you can create a new descendant anywhere in the descendant tree (such as `spell` or `permanent`)
   - **Methods**:
     - `void tap()`
     - `void untap()`
     - `std::string get_name()`
     - `std::string get_type()`
     - `bool is_tapped()`
     - `void set_tapped(bool set_to)`
     - `player* get_owner()`
     - `void set_owner(player* this_owner)`

4. **Land**
   - **Methods**:
     - `std::string get_subtype()`
     - `color get_taps_for()`

5. **Spell** (inherits from `Card`)  
   - **Methods**:  
     - `std::string get_cost()`: Cost to cast the spell.  
     - `std::vector<std::unique_ptr<effect>>& get_effects()`

6. **Creature** (inherits from `Permanent` which inherits from `Spell`)  
   - **Methods**:
     - `std::string get_damagable_name()`
     - `void deal_damage(int amount)`
     - `std::string get_subtype()`
     - `int get_power()`
     - `int get_toughness()`
     - `int get_health()`
     - `bool get_summoning_sickness()`
     - `void set_summoning_sickness(bool set_to)`
     - `void set_health(int set_to)`
     - `bool get_dead()`

7. **Effect**  (ABSTRACT)
   - **Description**: Defines game effects such as dealing damage, healing, etc.  
   - **Extendability**: To extend the functionality of effects, create a new descendant of this class
   - **Methods**:  
     - `void execute(damagable& target)`
     - `std::string get_name()`

9. **IniParser**
   - **Description**: Header for INI parsing.
   - **Methods**:
     - `IniData parseIniFile(const std::string& filename)` : Reads INI file and returns a map of sections

10. **Color**
   - **Description**: Enum Class for card colors.
   - **Methods** :
     - `color string_to_color_map(const std::string& color_string)`
     - `color char_to_color_map(const char& color_char)`
     - `std::string color_to_string(color color_of_mana)`

11. **Damagable** (ABSTRACT)
   - **Description**: Parent class for classes that can take damage
   - **Extendability**: Inheritance from this class and implementing it's methods allows for creating a new targetable and damagable class
   - **Methods**:
     - `std::string get_damagable_name()`
     - `void deal_damage(int amount)`

## Main Function
- The entry point of the application where the game is initialized and started.
- Sets up players and their decks.
- Calls the game loop to manage turns and game flow.


## Extendability 

To extend the capability of `effects`, it is needed to modify `effect_factory.cpp` for another effect creation, `effect.hpp` and `effect.cpp` to create a new class from abstract class effect (a new descendant). 
The `effect` class declares `void execute(damagable& target)` and `string get_name()`, if the descended class declares these methods, the `effect` will work.

To create more card types, there are multiple semi-stages of abstract class `card` declared. If you need something with an "instant" effect, you derive from `spell`, else most likely it will be `permanent`.
If it's something more exotic, there will be a need to create a new branch in `cast_card` in `player.hpp`

To create functional `abilities` there would be a need to create events at all phase transitions (e.g. from main phase to combat phase to check for haste). This would also need a bit of work in to create `ability_factory` first and then modify the code in `game` and `ability` classes (most likely an execute and get_name methods just like effects).

For example, let's say I want to create an effect that deals damage to opponent equal to the number of Goblins on the battlefield. I would create an effect and register in effect_factory deal_damage_x(goblin) that in it's execute method, counts the amount of goblins on the battlefield and deals that much damage.

The program is limited by the command line interface and would need two interconnected ones for actual play (with different outputs)

## Used Technology

I used pure c++ with no libraries. The program should be able to run with lower Cpp standard but I selected C++20 for ease.

I used CMake for multi-platform capabilities.

For creating effects and cards I used factory patterns.

## Programming Problems

The programming of the application was relatively straight-forward, did have couple problems with self-deleting unique pointers but nothing out of the ordinary for cpp.
The only hard part of the program was figuring out what to do next as the project was relatively branching.

## Differences compared to specification

In the specification I had the abilities, but realised it would take more time to implement them so I decided to do effects which are mostly the same, just a bit easier.

There are more card types in the specification, but they are tied to the ability system and would be useless without it so I decided to not include it.

In the game turn I decided to have only one main phase as even with one the command line gets a bit crowded.