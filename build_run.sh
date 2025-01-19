bash clean_data.sh
mkdir -p cmake
cd cmake
make
cd ..
cmake/chessboardconverter
python3 stockfish_evaluator.py
cmake/chessstockfishmoveconverter