.PHONY: clean

SOURCE = $(wildcard *.c)
TARGET = libcacu.so
LD = -fPIC -shared

$(TARGET):
	$(CC) $(LD) -o $@ $(SOURCE)

clean:
	rm -rf *.so