#include "bulls_and_cows.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

string generate_secret_number(int length) {
    string digits = "0123456789";
    string secret = "";
    
    for (int i = 9; i > 0; i--) { // (алгоритм Фишера-Йетса)
        int j = rand() % (i + 1);
        char temp = digits[i];
        digits[i] = digits[j];
        digits[j] = temp;
    }
    //Если первый в строке "0", то берём число со второй цифры
    int start_index = (digits[0] == '0') ? 1 : 0;
    
    for (int i = 0; i < length; i++) {
        secret += digits[start_index + i];
    }
    
    return secret;
}

string get_secret_number_from_player(const string& player_name, int length) {
    string secret;
    bool valid = false;
    
    while (!valid) {
        clean_screen();
        cout << "=================================\n";
        cout << "       РЕЖИМ ДВУХ ИГРОКОВ\n";
        cout << "=================================\n";
        cout << player_name << ", введите " << length << "-значное число (секретное):\n";
        cout << "Цифры не должны повторяться, число не должно начинаться с 0\n";
        cout << "=================================\n";
        cin >> secret;
        
        if (secret.size() != static_cast<size_t>(length)) {
            cout << "Ошибка: число должно быть " << length << "-значное!\n";
        }
        else if (!Digits_Only(secret)) {
            cout << "Ошибка: в числе должны быть только цифры!\n";
        }
        else if (!Repeat_in_num(secret)) {
            cout << "Ошибка: цифры в числе не должны повторяться!\n";
        }
        else if (secret[0] == '0') {
            cout << "Ошибка: число не должно начинаться с нуля!\n";
        }
        else {
            valid = true;
        }
        
        if (!valid) {
            cin.clear();
            cin.ignore(1000, '\n');
            pause();
        }
    }
    
    // "Скрываем" введенное число
    clean_screen();
    cout << "=================================\n";
    cout << "       РЕЖИМ ДВУХ ИГРОКОВ\n";
    cout << "=================================\n";
    cout << player_name << " ввел(а) секретное число.\n";
    cout << "Теперь оно скрыто от второго игрока.\n";
    cout << "=================================\n";
    pause();
    
    return secret;
}

void start_new_game(GameState& state, int length) {
    state.number_length = length;
    state.secret_number = generate_secret_number(length);
    state.history.clear();
    state.attempts_count = 0;
    state.game_finished = false;
    state.two_players_mode = false;
}

void start_two_players_game(GameState& state, int length) {
    state.number_length = length;
    state.history.clear();
    state.attempts_count = 0;
    state.game_finished = false;
    state.two_players_mode = true;

    state.secret_number = get_secret_number_from_player(state.player1_name, length);
}

void bulls_cows(const GameState& state, const string& guess, int& bulls, int& cows) {
    bulls = 0;
    cows = 0;
    
    const string& secret = state.secret_number;
    int length = state.number_length;
    
    for (int i = 0; i < length; i++) {
        if (guess[i] == secret[i]) {
            bulls++;
        } else { 
            for (int j = 0; j < length; j++) {
                if (i != j && guess[i] == secret[j]) {
                    cows++;
                    break;
                }
            }
        }
    }
}

bool Digits_Only(const string& str) {
    for (size_t i = 0; i < str.size(); i++) {
        char c = str[i];
        if (c < '0' || c > '9') {
            return false;
        }
    }
    return true;
}

bool Repeat_in_num(const string& str) {
    for (size_t i = 0; i < str.size(); i++) {
        for (size_t j = i + 1; j < str.size(); j++) {
            if (str[i] == str[j]) {
                return false;
            }
        }
    }
    return true;
}

string player_guess(const GameState& state, const string& player_name) {
    string guess;
    bool value = false;
    string prompt_player = player_name.empty() ? "" : player_name + ", ";
    
    while (!value) {
        cout << "\n" << prompt_player << "введите " << state.number_length << "-значное число (без повторяющихся цифр): ";
        cin >> guess;
        
        if (guess.size() != static_cast<size_t>(state.number_length)) {
            cout << "Ошибка: число должно быть " << state.number_length << "-значное и не отрицательное!\n";
        }
        else if (!Digits_Only(guess)) {
            cout << "Ошибка: в числе должны быть только цифры!\n";
        }
        else if (!Repeat_in_num(guess)) {
            cout << "Ошибка: цифры в числе не должны повторяться!\n";
        }
        else if (guess[0] == '0') {
            cout << "Ошибка: число не должно начинаться с нуля!\n";
        }
        else {
            value = true;
        }
        
        if (!value) {
            cin.clear();
            cin.ignore(1000, '\n');
        }
    }
    
    return guess;
}