#ifndef INCLUDE_CHESS_MOVES
#define INCLUDE_CHESS_MOVES

#include "chess.hpp"
#include "chess_board.h"
#include <string>

class Moves {
public:
  static void getLegalMoves(chess::Movelist &moveList,
                            const chess::Board &board,
                            const chess::Color &color = chess::Color::NONE);
};

#endif