include ../../makefile.conf

OUTPUT=notify

SRC    = $(wildcard *.c)
OBJ    = $(patsubst %.c, obj/%.o, $(SRC))

$(OUTPUT).a: $(OBJ)
	@ echo "  AR $@"
	@ ar rcs ../../obj/$(OUTPUT).a $(OBJ)

obj/%.o: %.c | obj
	@ echo "  CC $<"
	@ $(CC) $(CFLAGS) -I. -c $< -o $@

obj:
	@ mkdir -p obj

.PHONY: obj
