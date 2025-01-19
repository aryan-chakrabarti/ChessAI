from stockfish import Stockfish
from dotenv import load_dotenv
import os
import time
import subprocess
import threading


def get_best_move(sf: Stockfish, fen_position: str):
    start = time.time()
    sf.set_fen_position(fen_position)
    best_move = sf.get_best_move()
    end = time.time()
    return best_move, end - start


def analyze_position(sf, fen, thread_best_moves, curr_thread):
    best_move, time_taken = get_best_move(sf, fen)
    thread_best_moves[curr_thread] = (best_move, time_taken)


def spawn_stockfish(sfs, curr_thread):
    sfs[curr_thread] = Stockfish(path=os.getenv("STOCKFISH_PATH"))


if __name__ == "__main__":
    load_dotenv()

    subprocess.run(["mkdir", "-p", "data"])

    # Load all boards
    count = 0
    positions = []
    with open("data/sample_games.fen") as f:
        best_moves = []
        times = []
        for fen_position in f.readlines():
            positions.append(fen_position)
            count += 1

    # Process each board
    FACTOR = 20
    NUM_THREADS = min(len(positions) // FACTOR, 10)
    posCount = 0
    results = []
    start = time.time()
    # Multithread spawning stockfish objects
    sfs = [None] * NUM_THREADS
    threads: list[threading.Thread] = []
    for i in range(NUM_THREADS):
        thread = threading.Thread(target=spawn_stockfish, args=[sfs, i])
        threads.append(thread)
    for thread in threads:
        thread.start()
    for thread in threads:
        thread.join()
    end = time.time()
    print(
        f"Time taken to create {NUM_THREADS} stockfish instances: {end - start} seconds"
    )
    while posCount < len(positions):
        threads = []
        curr_thread = 0
        thread_best_moves = [("", 0)] * NUM_THREADS
        while posCount < len(positions) and curr_thread < NUM_THREADS:
            thread = threading.Thread(
                target=analyze_position,
                args=[
                    sfs[curr_thread],
                    positions[posCount],
                    thread_best_moves,
                    curr_thread,
                ],
            )
            threads.append(thread)
            posCount += 1
            curr_thread += 1
        for thread in threads:
            thread.start()
        for thread in threads:
            thread.join()
        results.extend(thread_best_moves)
    times = [result[1] for result in results]
    best_moves = [result[0] for result in results]

    # Write moves to file
    with open("data/best_moves.txt", "w") as f:
        for i in range(count):
            if i < count - 1:
                f.write(f"{best_moves[i]}\n")
            else:
                f.write(best_moves[i])

    print(f"Average time to evaluate position: {sum(times) / len(times)} seconds")
