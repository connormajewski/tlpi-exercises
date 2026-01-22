LIB_DIR  = ../../lib

ERROR_FUNCTIONS = $(LIB_DIR)/error_functions.c
GET_NUM = $(LIB_DIR)/get_num.c
INCLUDE = -I$(LIB_DIR)

CC = gcc

all:
	$(CC) $(FILE).c $(ERROR_FUNCTIONS) $(GET_NUM) -o $(FILE) $(INCLUDE)

clean:
	find . -maxdepth 1 -type f -executable -exec rm {} \;
