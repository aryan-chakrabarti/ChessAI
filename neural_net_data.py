import numpy as np
from torch import Tensor


def pad_legal_moves(legal_moves, max_num_moves):
    for i, moves in enumerate(legal_moves):
        if len(moves) < max_num_moves:
            legal_moves[i].extend([0] * (max_num_moves - len(moves)))


def convert_square_to_vector(square):
    vect = [0] * 64
    vect[square] = 1
    return vect


def read_data():
    boards = []
    all_legal_moves = []
    start_squares = []
    end_squares = []
    with open("data/best_moves.from") as f:
        for line in f.readlines():
            start_square = int(line.split(" ")[0])
            start_squares.append(convert_square_to_vector(start_square))
    with open("data/best_moves.to") as f:
        for line in f.readlines():
            end_square = int(line)
            end_squares.append(convert_square_to_vector(end_square))
    chess_board_name_base = "chessboard"
    max_len_legal_moves = 0
    for i in range(1, len(start_squares) + 1):
        with open(f"data/{chess_board_name_base}{i}.data") as f:
            # First 8 lines are board, 9th line is legal moves
            board = []
            legal_moves = []
            for j, line in enumerate(f.readlines()):
                if j < 8:
                    # Chess board
                    row_list = line.split()
                    row = []
                    for piece in row_list:
                        row.append(int(piece))
                    board.append(row)
                elif j == 8:
                    # Legal moves
                    legal_move_str = line.split()
                    for move in legal_move_str:
                        legal_moves.append(int(move))
            boards.append(board)
            all_legal_moves.append(legal_moves)
            max_len_legal_moves = max(max_len_legal_moves, len(legal_moves))
    pad_legal_moves(all_legal_moves, max_len_legal_moves)
    boards = np.array(boards)
    all_legal_moves = np.array(all_legal_moves)
    all_legal_moves = all_legal_moves.reshape(len(all_legal_moves), 1, -1)
    start_squares = np.array(start_squares)
    start_squares = start_squares.reshape(-1, 1, 64)
    end_squares = np.array(end_squares)
    end_squares = end_squares.reshape(-1, 1, 64)
    return (
        Tensor(boards),
        Tensor(all_legal_moves),
        Tensor(start_squares),
        Tensor(end_squares),
    )


if __name__ == "__main__":
    boards, legal_moves, start_squares, end_squares = read_data()
    print(boards.shape, legal_moves.shape, start_squares.shape, end_squares.shape)
