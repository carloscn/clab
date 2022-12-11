
A = 123
B = default
C := $(B)

ifeq ($(A), 123)
	B := yes
else
	B := no
endif

ifneq ($(A), 123)
	C := yes
else
	C := no
endif

show:
	echo $(A)
	echo $(B)
	echo $(C)
ifdef A
	@echo "ifdef A : the A defined"
endif

ifndef A
	@echo "ifndef A: THE A not defined"
endif

TARGET=a b c d

loop:
	mkdir $(foreach v, $(TARGET), $v.txt)

clean:
	rm -rf $(foreach v, $(TARGET), $v.txt)


define func_1
	return 123
endef

define func_2
	return $(1)
endef

call:
	echo $(call func_1)
	echo $(call func_2, 9999)