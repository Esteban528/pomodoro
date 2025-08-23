include ../../makefile.conf

OUTPUT=pomtimer

SRC=$(wildcard *.c)
OBJ=$(patsubst %.c, obj/%.o, $(SRC))

$(OUTPUT).a: $(OBJ)
	@ ar rcs ../../obj/$@ $(OBJ)

obj/%.o: %.c
	@ echo "  CC $<"
	@ $(CC) $(CFLAGS) -I. -c $< -o $@
