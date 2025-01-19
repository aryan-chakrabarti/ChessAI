#ifndef INCLUDE_CHESS_PARSER
#define INCLUDE_CHESS_PARSER

#include "chess.hpp"
#include "chess_board.h"
#include <memory>
#include <string>
#include <vector>

namespace {
using namespace std;
}

class Parser {
public:
  static void translateFenNotation(Board &board, const string &fenNotation);
  static void translateFromNum(char &pieceStr, int piece);
  static int convertMoveIntoInt(const chess::Move &move,
                                const chess::Board &board);
  static int convertPieceTypeIntoInt(const chess::PieceType &pieceType);
  static int convertSquareIntoInt(const chess::Square &square);
  static chess::Move convertIntIntoMove(int moveInt);
  static void convertIntIntoPieceType(chess::PieceType &pieceType,
                                      int pieceInt);
  static chess::Square convertIntIntoSquare(int squareInt);
};

#endif