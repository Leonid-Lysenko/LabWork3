G = g++
A = Lab4
FLAGS = -std=c++17 -Werror -Wpedantic -Wall -g
GTEST_FLAGS = -lgtest -lgtest_main -lpthread
SRC = $(wildcard *.cpp)
OBJS = $(SRC:.cpp=.o)

all: $(A)

$(A): $(OBJS)
	$(G) $(FLAGS) $(OBJS) -o $@ $(GTEST_FLAGS)

%.o: %.cpp
	$(G) $(FLAGS) -c $< -o $@

docs:
	doxygen Doxyfile

test: $(A)
	./$(A)

clean:
	rm -f $(OBJS) $(A)

.PHONY: all clean docs test
