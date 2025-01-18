#ifndef INCLUDE_CHESS_MOVES
#define INCLUDE_CHESS_MOVES

#include "chess.hpp"
#include "chess_board.h"
#include <string>

class Moves {
  static void getLegalMoves(chess::Movelist &moveList,
                            const string &fen_notation);
};

#endif