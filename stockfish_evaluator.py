from stockfish import Stockfish
from dotenv import load_dotenv
import os
import time


def get_best_move(sf: Stockfish, fen_position: str):
    start = time.time()
    sf.set_fen_position(fen_position)
    best_move = sf.get_best_move()
    end = time.time()
    return best_move, end - start


if __name__ == "__main__":
    load_dotenv()
    start = time.time()
    sf = Stockfish(path=os.getenv("STOCKFISH_PATH"))
    end = time.time()
    print(f"Time to load stockfish: {end - start} seconds")
    # Load all boards
    count = 0
    with open("sample_games.fen") as f:
        positions = []
        for fen_position in f.readlines():
            positions.append(fen_position)
            count += 1
    # Process each board
    best_moves = []
    times = []
    for fen_position in positions:
        best_move, process_time = get_best_move(sf, fen_position)
        times.append(process_time)
        best_moves.append(best_move)
    # Write moves to file
    with open("best_moves.txt", "w") as f:
        for i in range(count):
            if i < count - 1:
                f.write(f"{best_moves[i]}\n")
            else:
                f.write(best_moves[i])

    print(f"Average time to evaluate position: {sum(times) / len(times)} seconds")
