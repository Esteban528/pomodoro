include makefile.conf

PROGRAM=pom_timer
CORE=$(wildcard src/*.c)

CORE_OBJ=$(patsubst %.c, obj/%.o, $(notdir $(CORE)))

.PHONY: debug run obj clean

CFLAGS=$(CINC) $(CDEBUGFLAGS)
debug: $(CORE_OBJ)
	# build program with debug in mind
	$(SILENT) echo "LD $(PROGRAM)"
	$(SILENT) $(CC) $(CORE_OBJ) $(CLIBS) -o $(PROGRAM)

CFLAGS=$(CINC) $(CRELEASEFLAGS)
release: $(CORE_OBJ)
	# build program with as release
	$(SILENT) echo "LD $(PROGRAM)"
	$(SILENT) $(CC) $(CORE_OBJ) $(CLIBS) -o $(PROGRAM)

run:
	# executes the program once generated
	$(SILENT) ./$(PROGRAM)

obj:
	@ mkdir -p obj

obj/%.o: src/%.c | obj
	$(SILENT) echo "CC $<"
	$(SILENT) $(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf obj $(PROGRAM)
