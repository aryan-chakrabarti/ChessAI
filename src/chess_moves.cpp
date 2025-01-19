#include "chess_moves.h"
#include "chess.hpp"
#include <algorithm>

void Moves::getLegalMoves(chess::Movelist &moveList, const chess::Board &board,
                          const chess::Color &color) {
  chess::Movelist tempMoveList;
  chess::movegen::legalmoves(tempMoveList, board);
  if (color != chess::Color::NONE) {
    for (size_t i(0); i < tempMoveList.size(); i++) {
      const chess::Move &move(tempMoveList.at(i));
      if (board.at(move.from()).color() == color) {
        moveList.add(move);
      }
    }
  } else {
    moveList = move(tempMoveList);
  }
  return;
}