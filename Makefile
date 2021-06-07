CC     = gcc
INCLUDE= include
SOURCE = $(wildcard src/*.c)
OBJS   = $(SOURCE:src/%.c=build/%.o)
C_FLAGS= -c -Wall -Werror -g -I$(INCLUDE) -DDEBUG
L_FLAGS=
TARGET = casic


.DEFAULT: all

.PHONY: all
all: $(TARGET)


$(TARGET): $(OBJS)
	$(CC) $(L_FLAGS) -o $@ $^


build:
	@mkdir -p build


.PHONY: test
test: $(TARGET)
	./casic test/test.casic


build/%.o: src/%.c | build
	$(CC) $(C_FLAGS) $< -o $@



.PHONY: clean
clean:
	$(RM) $(TARGET)
	$(RM) $(OBJS)
