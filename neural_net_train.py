from torch.nn import Conv2d, Linear, MultiheadAttention
from torch.nn.functional import softmax, sigmoid


def read_data():
    pass


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
               |  |    --------------    |
               |  |                      |        ____________
               |  |                       -------|            |
               |   ------------------------------| End square | -- End square
                ---------------------------------|    model   |
                                                  ------------

Start square model:



End square model:

"""
