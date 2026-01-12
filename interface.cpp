#include "bulls_and_cows.h"
#include <iostream>
#include <fstream>  

using namespace std;

string get_player_name(int player_number) {
    string name;
    cout << "Введите имя " << player_number << " игрока: ";
    cin >> name;
    cin.ignore(1000, '\n');
    return name;
}

int check_the_number(int min, int max, const string& text) {
    int number;
    
    while (true) {
        cout << text;
        cin >> number;
        
        if (cin.fail() || number < min || number > max) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Ошибка! Введите число от " << min << " до " << max << ".\n";
        } else {
            cin.ignore(1000, '\n');
            break;
        }
    }
    
    return number;
}

void clean_screen() {
    for (int i = 0; i < 30; i++) {
        cout << "\n";
    }
}

void pause() {
    cout << "\nНажмите Enter для продолжения...";
    cin.clear();
    cin.get();
}

void show_rules(const GameState& state) {
    clean_screen();
    cout << "=================================\n";
    cout << "        ПРАВИЛА ИГРЫ\n";
    cout << "=================================\n";
    cout << " Компьютер загадывает " << state.current_length << "-значное число\n";
    cout << " (цифры не повторяются, число не начинается с 0)\n";
    cout << " Игрок делает ходы, чтобы узнать эти цифры и их порядок.\n";
    cout << " После каждой попытки появляется подсказка:\n";
    cout << "   * Быки: цифра есть и стоит на своем месте\n";
    cout << "   * Коровы: цифра есть, но стоит не на своем месте\n";
    cout << " Игра продолжается до полного угадывания числа\n";
    cout << "=================================\n";
    cout << "\nРЕЖИМ ДВУХ ИГРОКОВ:\n";
    cout << " Первый игрок загадывает число\n";
    cout << " Второй игрок пытается его угадать\n";
    cout << "=================================\n";
    pause();
}

void current_score(const GameState& state) {
    clean_screen();
    cout << "=================================\n";
    cout << "РЕКОРД ДЛЯ " << state.current_length << "-ЗНАЧНЫХ ЧИСЕЛ\n";
    cout << "=================================\n";
    
    int score = load_high_score(state.current_length);
    
    if (score < 0) {
        cout << "Рекорд еще не установлен.\n";
        cout << "(файл рекордов отсутствует или поврежден)\n";
    } else {
        cout << "Наименьшее кол-во попыток: " << score << endl;
    }
    
    cout << "=================================\n";
    pause();
}

void all_scores() {
    clean_screen();
    cout << "=================================\n";
    cout << "         ВСЕ РЕКОРДЫ\n";
    cout << "=================================\n";
    
    ifstream file("highscore.txt");
    if (!file) {
        cout << "Ошибка: файл рекордов недоступен!\n";
        cout << "=================================\n";
        pause();
        return;
    }
    
    bool found_any = false;
    int len, score;

    while (file >> len >> score) {
        if (file) {  
            if (len >= 3 && len <= 6 && score > 0) {
                cout << "Длина числа " << len << ", кол-во попыток: " << score << endl;
                found_any = true;
            }
        }
    }
    
    if (!found_any) {
        cout << "Рекордов еще нет.\n";
    }
    
    file.close();
    cout << "=================================\n";
    pause();
}

void scores_menu(GameState& state) {
    bool inside = true;
    
    while (inside) {
        clean_screen();
        cout << "=================================\n";
        cout << "         ПРОСМОТР РЕКОРДОВ\n";
        cout << "=================================\n";
        cout << "1. Текущий рекорд (длина числа " << state.current_length << ")\n";
        cout << "2. Все рекорды\n";
        cout << "3. Вернуться в главное меню\n";
        cout << "=================================\n";
        
        int choice = check_the_number(1, 3, "Выберите пункт: ");
        
        switch (choice) {
            case 1:
                current_score(state);
                break;
            case 2:
                all_scores();
                break;
            case 3:
                inside = false;
                break;
        }
    }
}

void play_two_players_game(GameState& state) {
    clean_screen();
    cout << "=================================\n";
    cout << "       РЕЖИМ ДВУХ ИГРОКОВ\n";
    cout << "=================================\n";
    
    state.player1_name = get_player_name(1);
    state.player2_name = get_player_name(2);
    
    cout << "\n=================================\n";
    cout << "Игроки: " << state.player1_name << " и " << state.player2_name << "\n";
    cout << "Длина числа: " << state.current_length << "\n";
    cout << "=================================\n";
    pause();
    
    start_two_players_game(state, state.current_length);
    
    while (!state.game_finished) {
        clean_screen();
        cout << "=================================\n";
        cout << "   ДВА ИГРОКА (Длина: " << state.current_length << ")\n";
        cout << "=================================\n";
        cout << "Загадывает: " << state.player1_name << "\n";
        cout << "Отгадывает: " << state.player2_name << "\n";
        cout << "=================================\n";
        
        if (state.attempts_count > 0) {
            cout << "\nИстория попыток:\n";
            cout << "-----------------\n";
            for (int i = 0; i < state.attempts_count; i++) {
                cout << i + 1 << ". " << state.history[i].guess; 
                cout << " - Быки: " << state.history[i].bulls; 
                cout << ", Коровы: " << state.history[i].cows << "\n";
            }
            cout << "-----------------\n";
        }
        
        cout << "Кол-во попыток: " << state.attempts_count << "\n";
        cout << "---------------------------------\n";
        
        cout << "1. " << state.player2_name << ", сделать попытку\n";
        cout << "2. Сдаться и выйти в главное меню\n";
        cout << "---------------------------------\n";
        
        int choice = check_the_number(1, 2, "Выберите действие: ");
        
        if (choice == 1) {
            string guess = player_guess(state, state.player2_name);
            
            int bulls, cows;
            bulls_cows(state, guess, bulls, cows);
            
            GameAttempt attempt;
            attempt.guess = guess;
            attempt.bulls = bulls;
            attempt.cows = cows;
            
            state.history.push_back(attempt);
            state.attempts_count++;
            
            cout << "\nРезультат: " << guess << " - Быки: " << bulls << ", Коровы: " << cows << "\n";
            
            if (bulls == state.number_length) {
                state.game_finished = true;
                cout << "\n═════════════════════════════════\n";
                cout << "  Победа! Число угадано\n";
                cout << "  Победитель: " << state.player2_name << "\n";
                cout << "  Загадал число: " << state.player1_name << "\n";
                cout << "  Количество попыток: " << state.attempts_count << "\n";
                cout << "═════════════════════════════════\n";
                
                pause();
            } else {
                pause();
            }
        } 
        else {
            cout << "\n═════════════════════════════════\n";
            cout << "  Игра завершена\n";
            cout << "  Загаданное число было: " << state.secret_number << "\n";
            cout << "  Кол-во попыток: " << state.attempts_count << "\n";
            cout << "═════════════════════════════════\n";
            
            pause();
            break;
        }
    }
}

void play_game(GameState& state) {
    int high_score = load_high_score(state.current_length);
    
    start_new_game(state, state.current_length);
    
    while (!state.game_finished) {
        clean_screen();
        cout << "=================================\n";
        cout << "      ИГРА (Длина: " << state.current_length << ")\n";
        //cout << "Сразу ответ для тестирования " << state.secret_number << endl;
        cout << "=================================\n";
        
        if (state.attempts_count > 0) {
            cout << "\nИстория попыток:\n";
            cout << "-----------------\n";
            for (int i = 0; i < state.attempts_count; i++) {
                cout << i + 1 << ". " << state.history[i].guess; 
                cout << " - Быки: " << state.history[i].bulls; 
                cout << ", Коровы: " << state.history[i].cows << "\n";
            }
            cout << "-----------------\n";
        }
        
        cout << "Кол-во попыток: " << state.attempts_count << "\n";
        if (high_score >= 0) {
            cout << "Рекорд (минимальное кол-во попыток): " << high_score << endl;
        }
        cout << "---------------------------------\n";
        
        cout << "1. Сделать попытку\n";
        cout << "2. Сдаться и выйти в главное меню\n";
        cout << "---------------------------------\n";
        
        int choice = check_the_number(1, 2, "Выберите действие: ");
        
        if (choice == 1) {
            string guess = player_guess(state, "");
            
            int bulls, cows;
            bulls_cows(state, guess, bulls, cows);
            
            GameAttempt attempt;
            attempt.guess = guess;
            attempt.bulls = bulls;
            attempt.cows = cows;
            
            state.history.push_back(attempt);
            state.attempts_count++;
            
            cout << "\nРезультат: " << guess << " - Быки: " << bulls << ", Коровы: " << cows << "\n";
            
            if (bulls == state.number_length) {
                state.game_finished = true;
                cout << "\n═════════════════════════════════\n";
                cout << "  Победа! Число угадано\n";
                cout << "  Количество попыток: " << state.attempts_count << "\n";
                cout << "═════════════════════════════════\n";
                
                if (high_score < 0 || state.attempts_count < high_score) {
                    cout << "\n !Новый рекорд! \n";
                    if (save_high_score(state.current_length, state.attempts_count)) {
                        cout << " !Рекорд сохранен!\n";
                    } else {
                        cout << " (Рекорд не сохранен - возможна ошибка файла)\n";
                    }
                }
                
                pause();
            } else {
                pause();
            }
        } 
        else {
            cout << "\n═════════════════════════════════\n";
            cout << "  Игра завершена\n";
            cout << "  Загаданное число было: " << state.secret_number << "\n";
            cout << "  Кол-во попыток: " << state.attempts_count << "\n";
            cout << "═════════════════════════════════\n";
            
            pause();
            break;
        }
    }
}

void settings_menu(GameState& state) {
    bool inside = true;
    
    while (inside) {
        clean_screen();
        cout << "=================================\n";
        cout << "         НАСТРОЙКИ\n";
        cout << "=================================\n";
        cout << "1. Изменить длину числа (3-6 цифр)\n";
        cout << "2. Настройки имен игроков\n";
        cout << "3. Вернуться в главное меню\n";
        cout << "=================================\n";
        
        int choice = check_the_number(1, 3, "Выберите пункт меню: ");
        
        switch (choice) {
            case 1: {
                clean_screen();
                cout << "=================================\n";
                cout << "   ВЫБОР ДЛИНЫ ЧИСЛА\n";
                cout << "=================================\n";
                cout << "Рекомендуется от 3 до 6 цифр\n";
                
                state.current_length = check_the_number(3, 6, "Введите длину числа (3-6): ");
                
                cout << "Длина числа установлена: " << state.current_length << "\n";
                pause();
                break;
            }
            
            case 2: {
                clean_screen();
                cout << "=================================\n";
                cout << "   НАСТРОЙКИ ИМЕН ИГРОКОВ\n";
                cout << "=================================\n";
                cout << "Текущие имена:\n";
                cout << "Игрок 1: " << state.player1_name << "\n";
                cout << "Игрок 2: " << state.player2_name << "\n";
                cout << "=================================\n";
                
                cout << "\nИзменить имя Игрока 1? (1-Да, 0-Нет): ";
                int change1;
                cin >> change1;
                cin.ignore(1000, '\n');
                
                if (change1 == 1) {
                    cout << "Введите новое имя для Игрока 1: ";
                    getline(cin, state.player1_name);
                }
                
                cout << "\nИзменить имя Игрока 2? (1-Да, 0-Нет): ";
                int change2;
                cin >> change2;
                cin.ignore(1000, '\n');
                
                if (change2 == 1) {
                    cout << "Введите новое имя для Игрока 2: ";
                    getline(cin, state.player2_name);
                }
                
                cout << "\nИмена обновлены!\n";
                pause();
                break;
            }
            
            case 3: {
                inside = false;
                break;
            }
        }
    }
}

void main_menu(const GameState& state) {
    clean_screen();
    cout << "=================================\n";
    cout << "      БЫКИ И КОРОВЫ\n";
    cout << "=================================\n";
    cout << "Текущая длина числа: " << state.current_length << "\n";
    cout << "Текущие игроки: " << state.player1_name << " и " << state.player2_name << "\n";
    cout << "---------------------------------\n";
    cout << "1. Начать новую игру (против компьютера)\n";
    cout << "2. Играть вдвоем (против другого игрока)\n";
    cout << "3. Настройки\n";
    cout << "4. Просмотреть рекорды\n";
    cout << "5. Правила игры\n";
    cout << "6. Выход\n";
    cout << "=================================\n";
}