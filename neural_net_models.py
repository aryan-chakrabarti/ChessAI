import torch.nn as nn
from torch import Tensor
from torch.nn.functional import sigmoid, softmax, relu

"""
2 submodels in NN model:
1. Start square predictor model
    a) Inputs: Board, legal moves
    b) Output: Start square of piece to move
2. End square predictor model
    a) Inputs: Board, start square, legal moves
    b) Output: End square of piece to move

                       ______________
Board ----------------|              |
               |      | Start square |--- Start square
Legal moves ----------|     model    |   |
               |  |    --------------    |        ____________
               |  |                       -------|            |
               |   ------------------------------| End square | -- End square
                ---------------------------------|    model   |
                                                  ------------

Start square model:
                            - Conv2D - Linear -
    Board ------- Linear - |                  +| -- Linear -----
                            ---- Attention ----                 |
                                                               *| - Linear - Linear - Softmax - Start square
                            ----------------------              |
    Legal Moves - Linear - |                     +| -- Linear --
                            ------ Attention -----


End square model:
                            - Conv2D - Linear -
    Board ------- Linear - |                  +| -- Linear -----
                            ---- Attention ----                  |
                                                                 |
                            ----------------------               |
    Legal Moves - Linear - |                     +| -- Linear -- | * - Linear - Linear - Softmax - End square
                            ------ Attention -----               |
                                                                 |
                             ----------------------              |
    Start Square - Linear - |                     +| -- Linear --
                             ------ Attention -----
"""


class StartSquareModel(nn.Module):
    def __init__(self):
        super().__init__()
        self.board_fc1 = nn.Linear(8, 8)
        self.board_fc2 = nn.Linear(8, 8)
        self.board_fc3 = nn.Linear(8, 8)
        self.board_conv = nn.Conv2d(
            in_channels=1, out_channels=1, kernel_size=3, padding=1
        )
        self.legal_moves_fc1 = nn.Linear(53, 21)
        self.legal_moves_fc2 = nn.Linear(21, 8)
        self.board_attention = nn.MultiheadAttention(embed_dim=8, num_heads=4)
        self.moves_attention = nn.MultiheadAttention(embed_dim=21, num_heads=3)
        self.main_fc1 = nn.Linear(8, 8)
        self.main_fc2 = nn.Linear(8, 8)
        self.main_flatten = nn.Flatten()
        self.main_fc3 = nn.Linear(64, 64)

    def forward(self, x):
        # x[0] is board, x[1] is legal moves
        y = x[0]
        z = x[1]

        y = self.board_fc1(y)
        y = relu(y)

        reshaped_y = y.reshape(-1, 1, 8, 8)
        conv_part = self.board_conv(reshaped_y)
        conv_part = conv_part.reshape(-1, 8, 8)
        y = relu(y)
        conv_part = self.board_fc2(conv_part)
        y = relu(y)
        board_attention, board_weights = self.board_attention(y, y, y)
        y = self.board_fc3(relu(board_attention + conv_part))
        y = relu(y)

        z = self.legal_moves_fc1(z)
        z = relu(z)
        moves_attention, moves_weights = self.moves_attention(z, z, z)
        z = self.legal_moves_fc2(relu(moves_attention + z))
        z = relu(z)

        x = relu(y * z)
        x = self.main_fc1(x)
        x = relu(x)

        x = self.main_fc2(x)
        x = relu(x)
        x = self.main_flatten(x)
        x = self.main_fc3(x)
        x = softmax(x, dim=1)

        return x


class EndSquareModel(nn.Module):
    def __init__(self):
        super().__init__()

    def forward(self, x):
        pass


class ChessAIModel(nn.Module):
    def __init__(self):
        super().__init__()
        self.start_square_model = StartSquareModel()
        self.end_square_model = EndSquareModel()

    def forward(self, x):
        start_square = self.start_square_model(x)
        end_square_input = Tensor([x, start_square])
        x = self.end_square_model(end_square_input)
        return x
