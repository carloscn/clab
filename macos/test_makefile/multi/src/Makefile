
.PHONY: clean algo print

TARGET=all
SOURCE = $(wildcard *.c)
OBJ = $(patsubst %.c, %.o, $(SOURCE))
INC = ../inc

all: algo print $(OBJ)
	echo $(OBJ)
	gcc $(OBJ) ./algo/*.o ./print/*.o -o main.elf -I $(INC)

%.o : %.c
	gcc -c $^ -o $@ -I $(INC)

algo:
	make -C ./algo

print:
	make -C ./print

clean:
	make -C ./algo clean
	make -C ./print clean
	rm -rf $(OBJ) *.elf