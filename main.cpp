#include "bulls_and_cows.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    GameState game_state; 
    
    bool running = true;
    
    while (running) {
        main_menu(game_state);
        
        int choice = check_the_number(1, 6, "Выберите пункт меню: ");
        
        switch (choice) {
            case 1:
                play_game(game_state);
                break;
                
            case 2:
                play_two_players_game(game_state);
                break;
                
            case 3:
                settings_menu(game_state);
                break;
                
            case 4:
                scores_menu(game_state);
                break;
                
            case 5:
                show_rules(game_state);
                break;
                
            case 6:
                cout << "\nСпасибо за игру!\n";
                cout << "(__)\n";
                cout << "(..)_____\n";
                cout << "(oo)    /|\\\n";
                cout << "  | |--/ | *\n";
                cout << "  w w w  w\n";
                running = false;
                break;
        }
    }
    
    return 0;
}