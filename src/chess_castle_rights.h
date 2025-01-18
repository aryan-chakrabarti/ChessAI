#ifndef INCLUDE_CHESS_CASTLE_RIGHTS
#define INCLUDE_CHESS_CASTLE_RIGHTS

// Castle Rights can be summarized in 4 bits
enum class CastleRights {
  NONE,
  BLACK_KING = 1,
  BLACK_QUEEN = 2,
  WHITE_KING = 4,
  WHITE_QUEEN = 8
};

inline CastleRights &operator|=(CastleRights &a, CastleRights b) {
  unsigned ai = static_cast<unsigned>(a);
  unsigned bi = static_cast<unsigned>(b);
  return a = static_cast<CastleRights>(ai | bi);
}

#endif