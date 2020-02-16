
FLAGS = -Werror -Wextra -Wall

all:
	clang++ $(FLAGS) -std=c++11 *.cpp -o avm
clean:

fclean: clean
	rm -rf avm

re: fclean all


