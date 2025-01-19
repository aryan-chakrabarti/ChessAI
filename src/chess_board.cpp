#include "chess_board.h"
#include "chess_color.h"
#include "chess_parser.h"

Board::Board()
    : m_board(8, vector<int>(8, 0)), m_turn(WHITE),
      m_castleRights(CastleRights::NONE), m_enPassant() {}

vector<vector<int>> &Board::data() { return m_board; }

ostream &operator<<(std::ostream &out, const Board &board) {
  out << "\n";
  for (int i = 0; i < board.data().size(); i++) {
    const vector<int> &row(board.data().at(i));
    for (int j = 0; j < row.size(); j++) {
      char pieceChar;
      Parser::translateFromNum(pieceChar, row.at(j));
      out << pieceChar;
      if (j < row.size() - 1) {
        out << " ";
      }
    }
    out << "\n";
  }
  out << "Turn: " << (board.turn() == WHITE ? "White" : "Black") << "\n";
  out << "Castle Rights: " << static_cast<int>(board.castleRights()) << "\n";
  out << "En passant: (" << board.enPassant().row << ", "
      << board.enPassant().col << ")"
      << "\n";
  return out;
}

const vector<vector<int>> &Board::data() const { return m_board; }

Color Board::turn() const { return m_turn; }

Color &Board::turn() { return m_turn; }

CastleRights Board::castleRights() const { return m_castleRights; }

CastleRights &Board::castleRights() { return m_castleRights; }

const Square &Board::enPassant() const { return m_enPassant; }

Square &Board::enPassant() { return m_enPassant; }

string Board::toString() const {
  ostringstream outputString;
  for (size_t i(0); i < m_board.size(); i++) {
    const vector<int> &row(m_board.at(i));
    for (size_t j(0); j < row.size(); j++) {
      outputString << row.at(j);
      if (j < row.size() - 1) {
        outputString << "\t";
      }
    }
    if (i < m_board.size() - 1) {
      outputString << "\n";
    }
  }
  return outputString.str();
}