CC     = gcc
INCLUDE= include
SOURCE = $(wildcard src/*.c)
OBJS   = $(SOURCE:src/%.c=build/%.o)
C_FLAGS= -c -Wall -Werror -Wunused -g -I$(INCLUDE)
#C_FLAGS= -c -I$(INCLUDE)
TARGET = casic


.DEFAULT: all

.PHONY: all
all: $(TARGET)


$(TARGET): $(OBJS)
	$(CC) -o $@ $^


build:
	@mkdir -p build


build/%.o: src/%.c | build
	$(CC) $(C_FLAGS) $< -o $@


.PHONY: clean
clean:
	$(RM) $(TARGET)
	$(RM) $(OBJS)
