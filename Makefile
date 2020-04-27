
COMPILE = clang++
FLAGS = -std=c++11 -Wall -O

timing : words.hpp words.cpp timing.cpp
	${COMPILE} ${FLAGS} words.cpp timing.cpp -o timing

clean:
		rm -f timing
