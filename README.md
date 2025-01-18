Steps to train chess AI:

1. Translate chess notation into 2d array with pieces
    a. Translate piece/color combo into an enum NN can process
2. Calculate all legal moves from current board position
3. Translate all legal moves into numbers for NN to process
4. Translate stockfish move into numbers for NN to process
5. Neural Net Design:
    Input 1: List of all legal moves
    Input 2: 2D board with pieces
    Output: Selection from list of legal moves

                    Input 1 -> Linear -> Attention --> Linear -----
                                                                   |
                             ------- Attention -------             |
                            |                         |            |
        Input 2 -> Linear -> Conv2D ----> Linear -----> Linear ------ Linear ---> Output
                                                                            |
                                                                After linear select move
                                                                with the highest output
6. Output data is the move chosen by stockfish