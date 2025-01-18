#include "chess_board.h"
#include "chess_moves.h"
#include "chess_parser.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

int main(int argc, char **argv) {
  const string STARTING_FEN_NOTATION =
      "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
  // Generate board for each FEN notation in file
  ifstream file("sample_games.fen");
  string line;
  size_t count = 1;
  if (file.is_open()) {
    while (getline(file, line)) {
      Board result;
      Parser::translateFenNotation(result, line);
      cout << "Chess board #" << count << ":" << result << endl;
      chess::Movelist moveList;
      getLegalMoves(moveList, line);
      cout << "Legal moves:\n";
      for (size_t i(0); i < moveList.size(); i++) {
        cout << moveList.at(i) << "\n";
      }
      cout << endl;
      count++;
    }
    file.close();
  } else {
    cerr << "Error opening file." << endl;
  }
  return 0;
}
