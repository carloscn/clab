# !/bin/zsh
rm -rf *.o *.elf*
echo "gcc -g -c main.c -o main.o" &&
gcc -g -c main.c -o main.o &&
echo "gcc -g -c extern.c -o extern.o"  &&
gcc -g -c extern.c -o extern.o  &&
echo "gcc -g main.o extern.o -o main.elf" &&
gcc main.o extern.o -o main.elf &&
chmod +x main.elf &&
./main.elf
