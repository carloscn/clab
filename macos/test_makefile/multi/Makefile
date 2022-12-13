
.PHONY: clean algo print run

TARGET=all
SOURCE = ./src/$(wildcard *.c)
OBJ = $(patsubst %.c, %.o, $(SOURCE))
INC = ./inc

run: all
	./main.elf

all: main algo print lib_cacu lib_mod $(OBJ)
	echo $(OBJ)
	gcc ./build/*.o -o main.elf -I $(INC) -L./build/ -lmod -lcacu

main: init
	make -C ./src
	cp -r ./src/*.o ./build

algo: init
	make -C ./src/algo
	cp -r ./src/algo/*.o ./build

print: init
	make -C ./src/print
	cp -r ./src/print/*.o ./build

lib_cacu:
	make -C ./lib/dym
	cp -r ./lib/dym/libcacu.so ./build
	cp -r ./build/libcacu.so .

lib_mod:
	make -C ./lib/static
	cp -r ./lib/static/libmod.a ./build

init:
	mkdir -p ./build

clean:
	make -C ./src/algo clean
	make -C ./src/print clean
	make -C ./lib/static clean
	make -C ./lib/dym clean
	make -C ./src clean
	rm -rf *.elf
	rm -rf ./build