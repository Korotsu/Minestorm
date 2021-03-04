CC 		= gcc
CFLAGS 		= -Wall -Werror -O0 -g -MMD
SRC_DIR 	= src
INC_DIR		= includes
BIN_DIR		= bin
SRC 		= $(wildcard $(SRC_DIR)/*.c)
OBJ 		= $(patsubst %.c, %.o, $(SRC))
LDLIBS 		= -lSDL2 -lSDL2_image -lSDL2_ttf -lm -lSDL2_gfx
OUTPUT 		= $(BIN_DIR)/space_invaders

.PHONY: all clean re run

all: $(OBJ)
	$(CC) $(CFLAGS) $^ $(LDLIBS) -o $(OUTPUT)

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $^ -g -c $(LDLIBS) -I$(INC_DIR) -o $@

clean:
	rm -rf $(OBJ) $(OUTPUT)

re: clean all

run: all
	./$(OUTPUT)
