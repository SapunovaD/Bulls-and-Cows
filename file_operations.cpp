#include "bulls_and_cows.h"
#include <iostream>
#include <fstream>
#include <array>

using namespace std;

bool save_high_score(int length, int score) {
    array<int, 4> highscores = {-1, -1, -1, -1};
    
    ifstream inFile("highscore.txt");
    if (inFile) {
        int len, scr;
        while (inFile >> len >> scr) {
            if (len >= 3 && len <= 6) {
                int index = len - 3;
                highscores[index] = scr;
            }
        }
        inFile.close();
    }
    
    int current_index = length - 3;
    bool is_new_record = false;
    
    if (highscores[current_index] == -1 || score < highscores[current_index]) {
        highscores[current_index] = score;
        is_new_record = true;
    } else {
        return false;
    }

    ofstream outFile("highscore.txt");
    if (!outFile) {  
        cout << "Ошибка: не удалось сохранить рекорд!\n";
        return false;
    }
    
    bool write_ok = true;
    for (int len = 3; len <= 6; len++) {
        int index = len - 3;
        if (highscores[index] != -1) {
            outFile << len << " " << highscores[index] << "\n";
            
            if (!outFile) {
                write_ok = false;
                break;
            }
        }
    }
    
    outFile.close();
    
    if (!write_ok) {
        cout << "Ошибка при записи в файл рекордов!\n";
        return false;
    }
    
    return is_new_record;
}

int load_high_score(int length) {
    ifstream file("highscore.txt");
    if (!file) {
        return -1; 
    }
    
    int len, score;
    int best_score = -1;

    while (file >> len >> score) {
        if (file) {
            if (len == length && score > 0) {
                if (best_score == -1 || score < best_score) {
                    best_score = score;
                }
            }
        }
    }
    
    file.close();
    return best_score;
}
