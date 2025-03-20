# Define variables
# CXX = /opt/llvm/19.1.4/bin/clang++
CXX = clang++
CXX_FLAGS = -Wall -Wextra -O2 -std=c++23
LD = ld
LD_FLAGS = --fatal-warnings -g

# Define targets
all: funclist

funclist:
	$(CXX) $(CXX_FLAGS) *.cpp

test: clean funclist
	./a.out

clean:
	rm -fv *.o

.PHONY: all clean
