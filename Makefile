COMPILER=g++
FLAGS=-fopenmp
OPTIMIZATION=-O3
STANDARD=c++14
SRC=main.cpp
NAME=tracer

all:
	$(COMPILER) $(FLAGS) $(OPTIMIZATION) -std=$(STANDARD) $(SRC) -o $(NAME)

clean:
	rm $(NAME).exe