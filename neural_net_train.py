import torch.optim as optim
import torch.nn as nn
from neural_net_models import StartSquareModel
from neural_net_data import read_data


def train_model(model, inputs, targets):
    # Define loss function and optimizer
    criterion = nn.CrossEntropyLoss()
    optimizer = optim.RMSprop(model.parameters(), lr=0.001)
    boards = inputs[0]
    all_legal_moves = inputs[1]
    # Training loop (simplified)
    for epoch in range(20):
        running_loss = 0
        running_accuracy = 0
        i = 0
        for board, legal_moves in zip(boards, all_legal_moves):
            optimizer.zero_grad()

            # Forward pass
            outputs = model([board, legal_moves])
            loss = criterion(outputs, targets[i])

            # Backward pass and optimization
            loss.backward()
            optimizer.step()

            # print statistics
            running_loss += loss.item()
            running_accuracy += (outputs == targets[i]).sum().item() / outputs.size(0)
            if i % 100 == 99:  # print every 100 mini-batches
                print(
                    f"[{epoch + 1}, {i + 1:5d}] accuracy: {running_accuracy / 100:.3f} loss: {running_loss / 100:.3f}"
                )
                running_loss = 0.0
                running_accuracy = 0
            i += 1


if __name__ == "__main__":
    model = StartSquareModel()
    boards, legal_moves, start_squares, end_squares = read_data()
    train_model(model, [boards, legal_moves], start_squares)
