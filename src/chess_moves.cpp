#include "chess_moves.h"

void Moves::getLegalMoves(chess::Movelist &moveList,
                          const string &fen_notation) {
  chess::Board board(fen_notation);
  chess::movegen::legalmoves(moveList, board);
  return;
}