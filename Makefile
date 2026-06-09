# ============================================================
#  Makefile – Menu-Driven 2D Graphics Editor in C
#  Targets: make | make run | make clean
# ============================================================

CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11 -pedantic
TARGET  = graphics_editor
SRC     = main.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) -lm

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET) $(TARGET).exe canvas.txt

.PHONY: all run clean
