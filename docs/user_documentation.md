# User Documentation for MTG Engine

### Requirements

Any PC capable of running a text_editor and a C++ compiler.

Required software:
CMake 3.12
C++ 20
C++ compiler

### Installation

go to ~/src/Windows/

run `cmake CMakeLists.txt`

go to build

run MTG_engine.exe

### Opening the program

run MTG_engine.exe in the build folder from the `Installation` step

You should see a (new) command line window prompting you to enter the player's name

Checkout [this](screenshots/mtg_engine_image_0.png) for how the program looks at first

### Start

The game is played in console, and the user can interact with the game by typing commands in the console.

The program starts with input for 2 names of players.

After this, the game starts.

The players roll 2 dice to see who goes first and the game askes both players if they want to mulligan. (Mulliganing is discarding your hand, shuffling your deck and drawing one less card). A player can mulligan as many times as they want (if they have at least 1 card left in their hand).

[this](screenshots/mtg_engine_image_1.png) shows how the start of the game looks

### Game

A player's turn is composed of several phases which are mostly invisible to the player.
The phases are:

- Untap
- Draw
- Main phase
- Combat
- End step

During untap step, the player untaps all his cards on the battlefield.

During draw step, the player draws a card from the top of their deck.

During main phase, the player can play one land and/or cast multiple spells with the mana they got from tapping their lands. This is also the phase where effects are triggered (when you play/cast some cards). The effects need a target and the user is prompted to provide it.

[this](screenshots/mtg_engine_image_2.png) shows how to play/cast/tap cards and pass

During combat, the player selects which creatures they want to attack with, and the non-active player selects blocks for these creatures. Then the defending player gets damage if any creatures still attacked them through blocks. Any creatures that were dealt damage equal to or greater than their toughness are destroyed.

[this](screenshots/mtg_engine_image_3.png) shows how to select creatures for combat

During end step, all creatures get healed back to their toughness, if the player has more than 7 cards in their hand, the player discards cards equal to the difference from their hand.

### Commands

During main phase, the player can make these types of commands:

- play/cast 'name of the land' - example: play Mountain
- play/cast 'name of the spell' - example: cast Lightning Bolt
- tap 'name of the land' - example: tap Mountain - gain one red mana
- pass - continue to combat phase
- hand - print all cards in your hand
- battlefield - print all cards on the battlefield
- graveyard - print all cards in your graveyard
- concede - concede the game, the other player becomes the winner

While resolving effects, you can target anything damagable, meaning players and creatures (it is on the user to not try to draw cards for a creature)

- me - targets the active_player
- opp - targets the non_active_player
- me 'name of a creature' - target creature by that name on the active_player battlefield
- opp 'name of a creature' - targets creature by that name on the non_active_player battlefield

During combat phase, the player will be asked for:

- Selecting attackers, example: 1 3 (first and third card on the battlefield). The player can select only creatures, if they select non-creature, they will be asked again
- The other player selects blockers, example: Select blockers for 'Name of the creature': 1 3 (first and third card on the other player's battlefield)
Select blockers for 'Name of the creature': 2 (second card on the other player's battlefield)
- After blockers are selected, the attacking player gets to choose for each of his attacking creatures in which order the attacking creature fights the defending creatures assigned to it
- the player can skip combat by pressing Enter with empty line when choosing attackers

### End

The game ends when:

- one player has no more cards in their library
- one player has less than 0 life
- one player concedes

The other player becomes the winner.