export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./lib/
g++ src/main.cpp -I./include/ -L./lib/ -lraylib -lm 
./a.out
