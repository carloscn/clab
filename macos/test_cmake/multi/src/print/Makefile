
.PHONY: clean

TARGET=print
SOURCE = $(wildcard *.c)
OBJ = $(patsubst %.c, %.o, $(wildcard *.c))
INC = ../../inc

$(TARGET): $(OBJ)
	@echo "build finish $(TARGET)"

%.o : %.c
	gcc -c $^ -o $@ -I $(INC)

clean:
	rm -rf *.o