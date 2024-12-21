#include "GameKallaha.h"

int main(int argc, char* argv[]) {

    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_file>\n";
        return 1;
    }

    std::ifstream input_file(argv[1]);
    if (!input_file) {
        std::cerr << "Failed to open file: " << argv[1] << "\n";
        return 1;
    }

    std::ofstream output_file(argv[2]);
    if (!output_file) {
        std::cerr << "Failed to open file: " << argv[2] << "\n";
        return 1;
    }

    int pits, stones;
    input_file >> pits >> stones;

    if (input_file.fail() || pits <= 0 || stones <= 0) {
        std::cerr << "Error: The input file must contain two positive integers (pits and stones).\n";
        return 1;
    }

    KallahaGame game(pits, stones, output_file);
    game.play();

    return 0;
}
