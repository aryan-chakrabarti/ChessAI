#include "chess_parser.h"
#include "chess_board.h"
#include "chess_color.h"
#include "chess_piece.h"
#include <memory>
#include <utility>
#include <vector>

void Parser::translateFenNotation(Board &board, const string &fenNotation) {
  vector<vector<int>> &boardData(board.data());

  /*
    6 fields:
      1. Board
      2. Turn
      3. Castle rights
      4. En passant
      5. Total number of moves made by both sides
      6. Number of complete turns made (= # of Black moves)
  */

  size_t currRow(0);
  size_t currCol(0);
  size_t currField(0);
  for (size_t i(0); i < fenNotation.size(); i++) {
    // Each piece will take 4 bits of information:
    // 3 for the piece type, 1 for the color
    char c(fenNotation.at(i));
    if (c == ' ') {
      // End of field
      currField++;
      continue;
    }
    switch (currField) {
    case 0: {
      if (c == '/') {
        // End of row
        currRow++;
        currCol = 0;
      } else if (c >= '1' && c <= '8') {
        // Empty space
        currCol += c - '0';
      } else if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
        // Uppercase for white, lowercase for black
        int piece = 0;
        switch (c) {
        case 'K':
          piece = (WHITE << 3) | KING; // 1110
          break;
        case 'k':
          piece = (BLACK << 3) | KING; // 0110
          break;
        case 'Q':
          piece = (WHITE << 3) | QUEEN; // 1101
          break;
        case 'q':
          piece = (BLACK << 3) | QUEEN; // 0101
          break;
        case 'R':
          piece = (WHITE << 3) | ROOK; // 1100
          break;
        case 'r':
          piece = (BLACK << 3) | ROOK; // 0100
          break;
        case 'B':
          piece = (WHITE << 3) | BISHOP; // 1010
          break;
        case 'b':
          piece = (BLACK << 3) | BISHOP; // 0010
          break;
        case 'N':
          piece = (WHITE << 3) | KNIGHT; // 1011
          break;
        case 'n':
          piece = (BLACK << 3) | KNIGHT; // 0011
          break;
        case 'P':
          piece = (WHITE << 3) | PAWN; // 1001
          break;
        case 'p':
          piece = (BLACK << 3) | PAWN; // 0001
          break;
        }
        boardData[currRow][currCol] = piece;
        currCol++;
      }
    } break;
    case 1: {
      if (c == 'w') {
        board.turn() = WHITE;
      } else if (c == 'b') {
        board.turn() = BLACK;
      }
    } break;
    case 2: {
      switch (c) {
      case '-':
        board.castleRights() = CastleRights::NONE;
        break;
      case 'Q':
        board.castleRights() |= CastleRights::WHITE_QUEEN;
        break;
      case 'K':
        board.castleRights() |= CastleRights::WHITE_KING;
        break;
      case 'q':
        board.castleRights() |= CastleRights::BLACK_QUEEN;
        break;
      case 'k':
        board.castleRights() |= CastleRights::BLACK_KING;
        break;
      }
    } break;
    case 3: {
      if (c != '-') {
        // En passant exists, get next character as well
        char nextChar(fenNotation.at(i + 1));
        // c is the column, nextChar is the row
        board.enPassant().col = c - 'a';
        board.enPassant().row = nextChar - '1';
        i += 1;
      }
    } break;
    }
  }
  return;
}

void Parser::translateFromNum(char &pieceStr, int piece) {
  if (piece == 0) {
    // Empty space
    pieceStr = '-';
    return;
  }
  int colorInt((0b1000 & piece) >> 3); // Gets leftmost bit
  int pieceTypeInt(0b111 & piece);     // Gets 3 rightmost bits
  Piece pieceType(static_cast<Piece>(pieceTypeInt));
  switch (pieceType) {
  case PAWN:
    pieceStr = 'p';
    break;
  case KNIGHT:
    pieceStr = 'n';
    break;
  case BISHOP:
    pieceStr = 'b';
    break;
  case ROOK:
    pieceStr = 'r';
    break;
  case QUEEN:
    pieceStr = 'q';
    break;
  case KING:
    pieceStr = 'k';
    break;
  }

  if (static_cast<Color>(colorInt) == WHITE) {
    pieceStr = toupper(pieceStr);
  }

  return;
}

int Parser::convertMoveIntoInt(const chess::Move &move,
                               const chess::Board &board) {
  // 8 parts of integer:
  // 1. Color (1 bit)
  // 2. Piece (3 bits)
  // 3. From square (6 bits)
  // 4. To square (6 bits)
  // 5. Is promotion (1 bit)
  // 6. Promotion piece (3 bits)
  // 7. Castle side (2 bits, 10 is queen, 01 is king, 00 is none)
  // 8. Is En Passant (1 bit)
  // Total: 22 bits
  int moveInt(0);

  // Color
  const chess::Piece &piece(board.at(move.from()));
  if (piece.color() == chess::Color::WHITE) {
    moveInt |= 1;
  }

  // Piece
  int pieceInt(convertPieceTypeIntoInt(piece.type()));
  moveInt << 3;
  moveInt |= pieceInt;

  // From square
  int fromSquareInt(convertSquareIntoInt(move.from()));
  moveInt << 6;
  moveInt |= fromSquareInt;

  // To square
  int toSquareInt(convertSquareIntoInt(move.to()));
  moveInt << 6;
  moveInt |= toSquareInt;

  // Is promotion
  bool isPromotion(move.typeOf() == move.PROMOTION);
  moveInt << 1;
  moveInt |= isPromotion;

  // Promotion piece
  int promotionPieceInt(0);
  if (isPromotion) {
    promotionPieceInt = convertPieceTypeIntoInt(move.promotionType());
  }
  moveInt << 3;
  moveInt |= promotionPieceInt;

  // Castle side
  int castleSideInt(0);
  if (move.typeOf() == move.CASTLING) {
    if (move.to().file() == chess::File::FILE_A) {
      // Queenside castle
      castleSideInt = 2;
    } else {
      // Kingside castle
      castleSideInt = 1;
    }
  }
  moveInt << 2;
  moveInt |= castleSideInt;

  // En passant
  bool isEnPassant(move.typeOf() == move.ENPASSANT);
  moveInt << 1;
  moveInt |= isEnPassant;

  return moveInt;
}

void convertIntIntoMove(chess::Move &move, int moveInt) {
  // 8 parts of integer:
  // 1. Color (1 bit)
  // 2. Piece (3 bits)
  // 3. From square (6 bits)
  // 4. To square (6 bits)
  // 5. Is promotion (1 bit)
  // 6. Promotion piece (3 bits, 4 possible promotion pieces)
  // 7. Castle side (2 bits, 10 is queen, 01 is king, 00 is none)
  // 8. Is En Passant (1 bit)
  // Total: 23 bits
  const unsigned COLOR_BIT = 22;
  const unsigned PIECE_BIT = 19;
  const unsigned FROM_SQUARE_BIT = 13;
  const unsigned TO_SQUARE_BIT = 7;
  const unsigned IS_PROMOTION_BIT = 6;
  const unsigned PROMOTION_PIECE_BIT = 3;
  const unsigned CASTLE_SIDE_BIT = 1;
  const unsigned EN_PASSANT_BIT = 0;

  const unsigned NUM_COLOR_BITS = 1;
  const unsigned NUM_PIECE_BITS = 3;
  const unsigned NUM_SQUARE_BITS = 6;
  const unsigned NUM_IS_PROMOTION_BITS = 1;
  const unsigned NUM_PROMOTION_PIECE_BITS = 3;
  const unsigned NUM_CASTLE_BITS = 2;
  const unsigned NUM_EN_PASSANT_BITS = 1;

  const unsigned BIT_LOCATIONS[8] = {
      COLOR_BIT,        PIECE_BIT,           FROM_SQUARE_BIT, TO_SQUARE_BIT,
      IS_PROMOTION_BIT, PROMOTION_PIECE_BIT, CASTLE_SIDE_BIT, EN_PASSANT_BIT};

  const unsigned BIT_SIZES[8] = {
      NUM_COLOR_BITS,  NUM_PIECE_BITS,        NUM_SQUARE_BITS,
      NUM_SQUARE_BITS, NUM_IS_PROMOTION_BITS, NUM_PROMOTION_PIECE_BITS,
      NUM_CASTLE_BITS, NUM_EN_PASSANT_BITS};

  int colorMask(1 << 22);
  int pieceMask(0b111 << 19);
  int fromSquareMask(0b111111 << 13);
  int toSquareMask(0b111111 << 7);
  int isPromotionMask(1 << 6);
  int promotionPieceMask(0b111 << 3);
  int casleSideMask(0b11 << 1);
  int enPassantMask(1);

  return;
}

int Parser::convertPieceTypeIntoInt(const chess::PieceType &pieceType) {
  int pieceInt(0);
  if (pieceType == chess::PieceType::PAWN) {
    pieceInt = 1;
  } else if (pieceType == chess::PieceType::KNIGHT) {
    pieceInt = 2;
  } else if (pieceType == chess::PieceType::BISHOP) {
    pieceInt = 3;
  } else if (pieceType == chess::PieceType::ROOK) {
    pieceInt = 4;
  } else if (pieceType == chess::PieceType::QUEEN) {
    pieceInt = 5;
  } else if (pieceType == chess::PieceType::KING) {
    pieceInt = 6;
  }
  return pieceInt;
}

int Parser::convertSquareIntoInt(const chess::Square &square) {
  int squareInt(0);
  // Row is rank, Col is file
  const chess::Rank &rank(square.rank());
  int rankInt(-1);
  if (rank == chess::Rank::RANK_1) {
    rankInt = 0;
  } else if (rank == chess::Rank::RANK_2) {
    rankInt = 1;
  } else if (rank == chess::Rank::RANK_3) {
    rankInt = 2;
  } else if (rank == chess::Rank::RANK_4) {
    rankInt = 3;
  } else if (rank == chess::Rank::RANK_5) {
    rankInt = 4;
  } else if (rank == chess::Rank::RANK_6) {
    rankInt = 5;
  } else if (rank == chess::Rank::RANK_7) {
    rankInt = 6;
  } else if (rank == chess::Rank::RANK_8) {
    rankInt = 7;
  }

  if (rankInt == -1) {
    return -1;
  }

  squareInt |= rankInt;
  squareInt << 3;

  const chess::File &file(square.file());
  int fileInt(-1);
  if (file == chess::File::FILE_A) {
    fileInt = 0;
  } else if (file == chess::File::FILE_B) {
    fileInt = 1;
  } else if (file == chess::File::FILE_C) {
    fileInt = 2;
  } else if (file == chess::File::FILE_D) {
    fileInt = 3;
  } else if (file == chess::File::FILE_E) {
    fileInt = 4;
  } else if (file == chess::File::FILE_F) {
    fileInt = 5;
  } else if (file == chess::File::FILE_G) {
    fileInt = 6;
  } else if (file == chess::File::FILE_H) {
    fileInt = 7;
  }

  if (fileInt == -1) {
    return -1;
  }

  squareInt |= fileInt;

  return squareInt;
}