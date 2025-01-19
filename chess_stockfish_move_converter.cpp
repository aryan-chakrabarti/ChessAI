#include "chess_parser.h"
#include <fstream>
int main(int argc, char **argv) {
  ifstream file("best_moves.txt");
  string line;
  size_t count = 1;
  if (file.is_open()) {
    while (getline(file, line)) {
      // Convert string into move
      cout << line << endl;
    }
    file.close();
  } else {
    cerr << "Error opening file." << endl;
  }
}