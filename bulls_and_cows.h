#include <vector>
#include <string>
#include <array>

struct GameAttempt {
    std::string guess;
    int bulls;
    int cows;
};

struct GameState {
    std::string secret_number;
    std::vector<GameAttempt> history;
    int number_length = 0;      
    int current_length = 4;      
    int attempts_count = 0;
    bool game_finished = false;
    bool two_players_mode = false;
    std::string player1_name = "Игрок 1";
    std::string player2_name = "Игрок 2";
};


// Основные функции игры
void start_new_game(GameState& state, int length);
void start_two_players_game(GameState& state, int length);
void main_menu(const GameState& state);
void play_game(GameState& state);
void play_two_players_game(GameState& state);
void settings_menu(GameState& state);
void show_rules(const GameState& state);
void scores_menu(GameState& state);
void current_score(const GameState& state);
void all_scores();

// Функции для ввода и проверки
std::string player_guess(const GameState& state, const std::string& player_name = "");
std::string get_player_name(int player_number);
std::string get_secret_number_from_player(const std::string& player_name, int length);
int check_the_number(int min, int max, const std::string& text);

// Функции проверки чисел
bool Digits_Only(const std::string& str);
bool Repeat_in_num(const std::string& str);
void bulls_cows(const GameState& state, const std::string& guess, int& bulls, int& cows);

// Функции работы с файлами
bool save_high_score(int length, int score);
int load_high_score(int length);

// Вспомогательные функции
void clean_screen();
void pause();
std::string generate_secret_number(int length);
