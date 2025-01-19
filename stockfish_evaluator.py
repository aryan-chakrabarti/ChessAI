from stockfish import Stockfish
from dotenv import load_dotenv
import os

if __name__ == "__main__":
    load_dotenv()
    sf = Stockfish(path=os.getenv("STOCKFISH_PATH"))
    best_moves = []
    with open("sample_games.fen") as f:
        count = 1
        best_moves = []
        for fen_position in f.readlines():
            sf.set_fen_position(fen_position)
            best_moves.append(sf.get_best_move())
    with open("best_moves.txt", "w") as f:
        for i, move in enumerate(best_moves):
            if i < len(best_moves) - 1:
                f.write(f"{move}\n")
            else:
                f.write(move)
