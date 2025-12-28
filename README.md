# Qt Blackjack

A simple Blackjack game implementation using Qt framework.

## Features
*   **Core Gameplay:** Standard Blackjack rules (Hit, Stand, Dealer logic).
*   **Betting System:** Place bets using chips (10, 50, 100).
*   **Animations:** Cards fly from the deck to the hand; chips move when betting.
*   **Skins:** Change card visuals (fronts and backs) dynamically during the game.
*   **Audio:** Background music and sound effects for actions.
*   **Multi-hand Support:** Option to bet on and play two hands.

## Prerequisites
*   CMake (Version 3.16 or higher)
*   Qt 6 (preferred) or Qt 5
    *   Modules: `Widgets`, `Multimedia`

## How to Build

1.  Clone or download the repository.
2.  Open a terminal in the project folder.
3.  Run the following commands:

```bash
mkdir build
cd build
cmake ..
cmake --build .
