SRC := $(shell find . -name '*.c')
OBJ = $(SRC:.c=.o)
EXE = test
VER = -std=c99
OPT = -O0 
ERR = -Wall -Wuninitialized -Werror=implicit-function-declaration -Wextra -Wno-unused-parameter -Wno-incompatible-pointer-types -Werror=int-conversion -Wduplicated-cond -Wlogical-op -Wrestrict -Wnull-dereference -Wjump-misses-init -Wdouble-promotion -Wshadow -Wformat=2 -Wno-unused-function -Wno-discarded-qualifiers
LFLAGS = -o
CFLAGS = $(ERR) $(VER) $(OPT) -c -g -fpic

make: $(OBJ)
	gcc -shared -o libezd.so $(OBJ)
	sudo cp *.h /usr/local/include/ezd/
	mv libezd.so /usr/local/lib/

%.o: %.c
	gcc $(CFLAGS) $<