CC=cc
CFLAGS=-I include -I lib/include -g
NAME=pipex
SRC_DIR=src
SRC=parser.c is_char.c command.c run.c file.c
OBJ=$(addprefix _bin/, $(SRC:.c=.o))
HEADERS=include/pipex.h

ifndef LENIENT
	CFLAGS += -Wall -Wextra -Werror
endif

ifdef DEBUG
	CFLAGS += -g
endif

all: $(NAME)

_bin:
	mkdir $@

_bin/%.o: $(SRC_DIR)/%.c $(HEADERS) Makefile | _bin
	$(CC) -c $(CFLAGS) -o $@ $<

lib/lib.a:
	if [! -d "./lib"]; then git clone https://github.com/gero-boehm/lib.git; fi
	(cd lib && make && make clean)

$(NAME): lib/lib.a $(OBJ) _bin/pipex.o
	$(CC) $(CFLAGS) -o $(NAME) $^

bonus: lib/lib.a $(OBJ) _bin/pipex_bonus.o
	$(CC) $(CFLAGS) -o $(NAME) $^

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

lclean: fclean
	rm -rf lib

re: fclean all

norm:
	norminette $(addprefix $(SRC_DIR)/, $(SRC)) $(HEADERS)

.PHONY:
	all bonus clean fclean lclean re norm