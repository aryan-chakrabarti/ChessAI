bash clean_data.sh
mkdir -p cmake
cd cmake
make
cd ..
python3 stockfish_evaluator.py
cmake/chessdataconverter