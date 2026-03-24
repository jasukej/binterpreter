CC = gcc
CFLAGS = -Wall -Wextra -Isrc -g -fsanitize=address
SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c, out/%.o, $(SRC))
TARGET = out/binterpreter
CPPCHECK ?= cppcheck
CPPCHECK_FLAGS ?= --enable=warning,style,performance,portability --error-exitcode=1 --inline-suppr

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

out/%.o: src/%.c | out
	$(CC) $(CFLAGS) -c $< -o $@

out:
	mkdir -p out

lint: lint-c

lint-c: 
	$(CPPCHECK) $(CPPCHECK_FLAGS) src

clean:
	rm -rf out
