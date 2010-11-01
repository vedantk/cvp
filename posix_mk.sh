flags='-Wextra -O2'

mkdir -p bin/
cd bin/
g++ $flags -c ../src/*.cpp
g++ $flags ../src/main.cxx *.o -o ../cvp

