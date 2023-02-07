CC=cc
CFLAGS=-I include -I lib/include -g
NAME=pipex
SRC_DIR=src
SRC=pipex.c parser.c is_char.c
OBJ=$(addprefix _bin/, $(SRC:.c=.o))
HEADERS=include/pipex.h

ifndef LENIENT
	CFLAGS += -Wall -Wextra -Werror
endif

ifdef DEBUG
	CFLAGS += -g
endif

all: $(NAME)
	# ./$< here_doc eof "grep c" outfile
	# ./$< infile "grep c" outfile
	# ./$<

_bin:
	mkdir $@

_bin/%.o: $(SRC_DIR)/%.c $(HEADERS) | _bin
	$(CC) -c $(CFLAGS) -o $@ $<

lib/lib.a:
	if [! -d "./lib"]; then git clone https://github.com/gero-boehm/lib.git; fi
	(cd lib && make && make clean)

$(NAME): lib/lib.a $(OBJ)
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