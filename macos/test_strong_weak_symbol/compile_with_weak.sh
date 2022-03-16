# !/bin/zsh
echo  "`rm -rf *.o *.elf`" &&
echo  "gcc -g main.c -o main.elf -Wl,-U,_extern_func" &&
gcc -g main.c -o main.elf -Wl,-U,_extern_func &&
chmod +x main.elf &&
echo ""
echo "run main.elf..."
echo ""
./main.elf
