#ifndef INCLUDE_CHESS_BOARD
#define INCLUDE_CHESS_BOARD

#include "chess_castle_rights.h"
#include "chess_color.h"
#include "chess_square.h"
#include <iostream>
#include <vector>

namespace {
using namespace std;
}

class Board {
  vector<vector<int>> m_board;
  Color m_turn;
  CastleRights m_castleRights;
  Square m_enPassant;

public:
  Board();
  const vector<vector<int>> &data() const;
  vector<vector<int>> &data();
  Color turn() const;
  Color &turn();
  CastleRights castleRights() const;
  CastleRights &castleRights();
  const Square &enPassant() const;
  Square &enPassant();
  friend ostream &operator<<(std::ostream &out, const Board &board);
  string toString() const;
};

#endif