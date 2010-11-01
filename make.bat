g++ src/main.cxx -Wextra -O3 -o cvp 
cd bin/
g++ -Wextra -O2 -c ../src/*.cpp
g++ -Wextra -O2 ../src/main.cxx *.o -o ../cvp
