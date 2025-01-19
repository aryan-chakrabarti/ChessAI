#include "chess_board.h"
#include "chess_moves.h"
#include "chess_parser.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

void generateBoardAndMoveData(vector<chess::Board> &boards) {
  const string STARTING_FEN_NOTATION =
      "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
  // Generate board for each FEN notation in file
  ifstream file("data/sample_games.fen");
  string line;
  size_t count = 1;
  if (file.is_open()) {
    while (getline(file, line)) {
      chess::Board board(line);
      boards.emplace_back(board);
      Board dataBoard;
      Parser::translateFenNotation(dataBoard, line);
      chess::Movelist moveList;
      Moves::getLegalMoves(moveList, board, board.sideToMove());
      vector<int> moveIntList;
      for (size_t i(0); i < moveList.size(); i++) {
        const chess::Move &move(moveList.at(i));
        int moveInt(Parser::convertMoveIntoInt(move, board));
        moveIntList.emplace_back(moveInt);
      }
      ostringstream outputFileNameStream;
      outputFileNameStream << "data/chessboard" << count << ".data";
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
      } else {
        cerr << "Error opening board and move output file." << endl;
      }
      count++;
    }
    file.close();
  } else {
    cerr << "Error opening board and move input file." << endl;
  }
}

void generateStockFishData(const vector<chess::Board> &boards) {
  // Read in Stockfish best moves and translate them
  // Into from square with piece and to square
  ifstream file("data/best_moves.txt");
  string line;
  size_t count = 0; // Count will sync boards with best moves
  vector<int> moveInts;
  vector<pair<int, int>> fromIntPairs;
  vector<int> toSquareInts;
  if (file.is_open()) {
    while (getline(file, line)) {
      const chess::Board &board(boards.at(count));
      // Convert string into move
      chess::Move move(Parser::convertStringIntoMove(line, board));
      // Split move into from/to tuples
      pair<int, int> fromInfoInt(Parser::getFromInfoInt(move, board));
      int toSquareInt(Parser::convertSquareIntoInt(move.to()));
      int moveInt(Parser::convertMoveIntoInt(move, board));
      moveInts.emplace_back(moveInt);
      fromIntPairs.emplace_back(fromInfoInt);
      toSquareInts.emplace_back(toSquareInt);
      count++;
    }
    file.close();
  } else {
    cerr << "Error opening stockfish input file." << endl;
  }

  // Write moveInts to file
  ofstream outputFile("data/best_moves.moves");
  if (outputFile.is_open()) {
    ostringstream outputMoveStream;
    for (size_t i(0); i < moveInts.size(); i++) {
      outputMoveStream << moveInts.at(i);
      if (i < moveInts.size() - 1) {
        outputMoveStream << "\n";
      }
    }
    const string &outputMoveStr(outputMoveStream.str());
    outputFile.write(outputMoveStr.c_str(), outputMoveStr.size());
    outputFile.close();
  } else {
    cerr << "Error opening stockfish moveInts output file." << endl;
  }

  // Write fromIntPairs to file
  outputFile = ofstream("data/best_moves.from");
  if (outputFile.is_open()) {
    ostringstream outputMoveStream;
    for (size_t i(0); i < fromIntPairs.size(); i++) {
      const pair<int, int> &fromPair(fromIntPairs.at(i));
      outputMoveStream << fromPair.first << " " << fromPair.second;
      if (i < fromIntPairs.size() - 1) {
        outputMoveStream << "\n";
      }
    }
    const string &outputMoveStr(outputMoveStream.str());
    outputFile.write(outputMoveStr.c_str(), outputMoveStr.size());
    outputFile.close();
  } else {
    cerr << "Error opening stockfish fromIntPairs output file." << endl;
  }

  // Write toSquareInts to file
  outputFile = ofstream("data/best_moves.to");
  if (outputFile.is_open()) {
    ostringstream outputMoveStream;
    for (size_t i(0); i < toSquareInts.size(); i++) {
      outputMoveStream << toSquareInts.at(i);
      if (i < toSquareInts.size() - 1) {
        outputMoveStream << "\n";
      }
    }
    const string &outputMoveStr(outputMoveStream.str());
    outputFile.write(outputMoveStr.c_str(), outputMoveStr.size());
    outputFile.close();
  } else {
    cerr << "Error opening stockfish toSquareInts output file." << endl;
  }
}

int main(int argc, char **argv) {
  vector<chess::Board> boards;
  generateBoardAndMoveData(boards);
  generateStockFishData(boards);
  return 0;
}
