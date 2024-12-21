#ifndef KALLAHAGAME_H
#define KALLAHAGAME_H

#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <ctime>
#include <string>

class KallahaGame {
private:
    std::vector<int> player1; // Human player's pits
    std::vector<int> player2; // Computer's pits
    int store1;               // Human's store
    int store2;               // Computer's store
    bool player1_turn;        // Whose turn (true - human, false - computer)
    std::ofstream& output_file;

    void initialize(int pits, int stones);
    void display() const;
    bool is_valid_move(int pit, bool is_player1) const;
    int computer_move();
    bool make_move(int pit);
    void log_game_state(std::ostream& out) const;
    bool is_game_over() const;
    bool all_zero(const std::vector<int>& vec) const;
    int sum_vector(const std::vector<int>& vec) const;
    void finalize_game();

public:
    KallahaGame(int pits, int stones, std::ofstream& output);
    void play();
};

#endif // KALLAHAGAME_H
