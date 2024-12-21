#include "GameKallaha.h"

KallahaGame::KallahaGame(int pits, int stones, std::ofstream& output) : output_file(output) {
    initialize(pits, stones);
}

void KallahaGame::initialize(int pits, int stones) {
    player1.assign(pits, stones);
    player2.assign(pits, stones);
    store1 = store2 = 0;
    player1_turn = true;
}

void KallahaGame::display() const {
    int pits = player1.size();
    int spacing = pits * 4;
    const int field_width = 4;

    std::cout << "Game Board:\n";

    std::cout << "  ";
    for (int i = player2.size() - 1; i >= 0; --i) {
        std::cout << std::string(field_width - std::to_string(player2[i]).length(), ' ')
            << player2[i] << " ";
    }
    std::cout << "\n";

    std::cout << std::string(field_width - std::to_string(store2).length(), ' ') << store2;
    for (int i = 0; i < spacing; ++i) {
        std::cout << " ";
    }
    std::cout << std::string(field_width - std::to_string(store1).length(), ' ') << store1 << "\n";

    std::cout << "  ";
    for (int stones : player1) {
        std::cout << std::string(field_width - std::to_string(stones).length(), ' ')
            << stones << " ";
    }
    std::cout << "\n";
}

bool KallahaGame::is_valid_move(int pit, bool is_player1) const {
    const std::vector<int>& side = is_player1 ? player1 : player2;
    return pit >= 0 && pit < side.size() && side[pit] > 0;
}


int KallahaGame::computer_move() {
    std::vector<int> valid_moves;
    for (int i = 0; i < player2.size(); ++i) {
        if (is_valid_move(i, false)) {
            valid_moves.push_back(i);
        }
    }

    if (valid_moves.empty()) {
        return -1;
    }

    std::srand(std::time(nullptr));
    int random_index = std::rand() % valid_moves.size();
    return valid_moves[random_index];
}


bool KallahaGame::make_move(int pit) {
    std::vector<int>& active_side = player1_turn ? player1 : player2;
    std::vector<int>& opponent_side = player1_turn ? player2 : player1;
    int& store = player1_turn ? store1 : store2;

    if (!is_valid_move(pit, player1_turn)) {
        throw std::invalid_argument("Invalid move!");
    }

    int stones = active_side[pit];
    active_side[pit] = 0; 
    int current_index = pit;

    while (stones > 0) {
        current_index++;

        if (current_index < active_side.size()) {
            active_side[current_index]++;
            stones--;

            if (stones == 0 && active_side[current_index] == 1) {

                int opposite_index = active_side.size() - 1 - current_index;

                store += active_side[current_index];
                active_side[current_index] = 0;

                if (opponent_side[opposite_index] > 0) {
                    store += opponent_side[opposite_index];
                    opponent_side[opposite_index] = 0;
                }
            }
        }

        else if (current_index == active_side.size()) {
            store++;
            stones--;

            if (stones == 0) {
                return true;
            }
            else {

                for (int i = 0; i < opponent_side.size() && stones > 0; ++i) {
                    opponent_side[i]++;
                    stones--;
                }
                if (stones == 0) break;
                current_index = -1;
            }
        }

        else if (current_index - active_side.size() - 1 < opponent_side.size()) {
            opponent_side[current_index - active_side.size() - 1]++;
            stones--;
        }

        else if (current_index - active_side.size() - 1 == opponent_side.size()) {
            stones--;
            if (stones > 0) {

                for (int i = 0; i < active_side.size() && stones > 0; ++i) {
                    active_side[i]++;
                    stones--;
                }
                if (stones == 0) break;
                current_index = -1;
            }
        }
    }

    player1_turn = !player1_turn;
    return false;
}

void KallahaGame::log_game_state(std::ostream& out) const {
    int pits = player1.size();
    const int field_width = 4;

    out << "Current Game State:\n";

    out << "  ";
    for (int i = player2.size() - 1; i >= 0; --i) {
        out << std::string(field_width - std::to_string(player2[i]).length(), ' ')
            << player2[i] << " ";
    }
    out << "\n";

    out << std::string(field_width - std::to_string(store2).length(), ' ') << store2;
    for (int i = 0; i < pits * 4; ++i) {
        out << " ";
    }
    out << std::string(field_width - std::to_string(store1).length(), ' ') << store1 << "\n";

    out << "  ";
    for (int stones : player1) {
        out << std::string(field_width - std::to_string(stones).length(), ' ')
            << stones << " ";
    }
    out << "\n";
}

bool KallahaGame::is_game_over() const {
    return all_zero(player1) || all_zero(player2);
}

bool KallahaGame::all_zero(const std::vector<int>& vec) const {
    for (int stones : vec) {
        if (stones != 0) return false;
    }
    return true;
}

int KallahaGame::sum_vector(const std::vector<int>& vec) const {
    int sum = 0;
    for (int stones : vec) {
        sum += stones;
    }
    return sum;
}


void KallahaGame::finalize_game() {
    store1 += sum_vector(player1);
    store2 += sum_vector(player2);
    player1.assign(player1.size(), 0);
    player2.assign(player2.size(), 0);
}



void KallahaGame::play() {
    bool game_ending = false;

    while (!is_game_over()) {
        display();

        if (player1_turn) {
            int pit;
            std::cout << "Your turn. Choose a pit (0-" << player1.size() - 1 << "): ";
            std::cin >> pit;

            if (std::cin.fail() || !is_valid_move(pit, true)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input. Try again.\n";
                continue;
            }

            try {
                if (make_move(pit)) {
                    std::cout << "You landed in your store! Take another turn.\n";
                }
            }
            catch (const std::invalid_argument& e) {
                std::cout << e.what() << " Try again.\n";
            }
        }
        else {
            int pit = computer_move();
            std::cout << "Computer's turn: pit " << pit << "\n";
            if (make_move(pit)) {
                std::cout << "The computer landed in its store! It takes another turn.\n";
            }
        }

        if (is_game_over()) {
            game_ending = true;
            output_file << "\nFinal Move Before Game End:\n";
            log_game_state(output_file);
        }
    }

    finalize_game();
    display();

    output_file << "\nGame Over!\n";
    log_game_state(output_file);
    output_file << "Your score:     " << store1 << "\n";
    output_file << "Computer score: " << store2 << "\n";

    if (store1 > store2) {
        output_file << "You have won!\n";
    }
    else if (store2 > store1) {
        output_file << "The computer won!\n";
    }
    else {
        output_file << "It's a draw!\n";
    }
    finalize_game();
    display();
}
