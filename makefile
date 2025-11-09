include makefile.conf

PROGRAM=pomodoro-timer
CORE=$(wildcard src/*.c)

CORE_OBJ=$(patsubst %.c, obj/%.o, $(notdir $(CORE)))

.PHONY: debug run obj

CFLAGS=$(CDEPS) $(CDEBUGFLAGS)
debug: $(CORE_OBJ)
#@ build program with debug in mind
	$(SILENT) echo "LD $(PROGRAM)"
	$(SILENT) $(CC) $(CFLAGS) $(CORE_OBJ) -o $(PROGRAM)

run:
#@ executes the program once generated (will freak out if you don't compile anything)
	$(SILENT) ./$(PROGRAM)

obj:
	@ mkdir -p obj

obj/%.o: src/%.c | obj
	$(SILENT) echo "CC $<"
	$(SILENT) $(CC) $(CFLAGS) -o $@ -c $<
