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
      chess::Board board(line);
      Board dataBoard;
      Parser::translateFenNotation(dataBoard, line);
      cout << "Chess board #" << count << ":" << dataBoard << endl;
      chess::Movelist moveList;
      Moves::getLegalMoves(moveList, board, board.sideToMove());
      vector<int> moveIntList;
      cout << "Legal moves for " << board.sideToMove() << ":\n";
      for (size_t i(0); i < moveList.size(); i++) {
        const chess::Move &move(moveList.at(i));
        int moveInt(Parser::convertMoveIntoInt(move, board));
        moveIntList.emplace_back(moveInt);
        cout << move << ", int form: " << moveInt << "\n";
      }
      cout << endl;
      ostringstream outputFileNameStream;
      outputFileNameStream << "chessboard" << count << ".data";
      string outputFileName(outputFileNameStream.str());
      ofstream outputFile(outputFileName);
      if (outputFile.is_open()) {
        ostringstream outputFileDataStream;
        ostringstream moveIntListStream;
        for (size_t i(0); i < moveIntList.size(); i++) {
          moveIntListStream << moveIntList.at(i);
          if (i < moveIntList.size() - 1) {
            moveIntListStream << " ";
          }
        }
        string boardOutput(dataBoard.toString());
        outputFileDataStream << boardOutput << "\n" << moveIntListStream.str();
        const string &outputFileDataStr(outputFileDataStream.str());
        outputFile.write(outputFileDataStr.c_str(), outputFileDataStr.size());
        outputFile.close();
      }
      count++;
    }
    file.close();
  } else {
    cerr << "Error opening file." << endl;
  }
  return 0;
}
