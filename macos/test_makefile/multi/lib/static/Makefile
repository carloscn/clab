.PHONY: clean

SOURCE = $(wildcard *.c)
TARGET = libmod.a
OBJ = mod.o

$(TARGET): $(OBJ)
	$(AR) -r $@ -o $^

mod.o:
	$(CC) -c -o $@ $(SOURCE)

clean:
	rm -rf $(OBJ) $(TARGET)