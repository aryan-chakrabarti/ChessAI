#ifndef INCLUDE_CHESS_SQUARE_UTILS
#define INCLUDE_CHESS_SQUARE_UTILS

#include "chess.hpp"
#include "chess_parser.h"

struct SquareUtils {
  static inline int getSquaredDistance(const chess::Square &from,
                                       const chess::Square &to) {
    int fromInt(Parser::convertSquareIntoInt(from));
    int toInt(Parser::convertSquareIntoInt(to));
    int mask(0b111);
    // Row is first 3 bits, col is last 3 bits
    int fromRow((fromInt & (mask << 3)) >> 3);
    int fromCol(fromInt & mask);
    int toRow((toInt & (mask << 3)) >> 3);
    int toCol(toInt & mask);

    int squared_sum = (toRow - fromRow) * (toRow - fromRow) +
                      (toCol - fromCol) * (toCol - fromCol);
    return squared_sum;
  }
};

#endif