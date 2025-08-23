# in case anything here does not work as intended,
# check the output of commands via "make VERBOSE=1 <command>"
include makefile.conf

ifdef VERBOSE
	ifeq ($(VERBOSE), 1)
		SILENT=
	endif
else
	SILENT=@
endif

PROGRAM=pomodoro-timer

CORE=$(wildcard core/*.c)
LIBS=$(wildcard lib/*)

CORE_OBJ=$(patsubst %.c, obj/%.o, $(notdir $(CORE)))
LIBS_OBJ=$(patsubst %,   obj/%.a, $(notdir $(LIBS)))

.PHONY: help release debug test clean run

help:
#@ display this message
	$(SILENT) ./scripts/help-message

CFLAGS=$(CDEPS) $(CRELEASEFLAGS)
release: $(LIBS_OBJ) $(CORE_OBJ) 
#@ build program with optimization in mind
	$(SILENT) echo "LD $(PROGRAM)"
	$(SILENT) $(CC) $(CFLAGS) $(CORE_OBJ) $(LIBS_OBJ) -o $(PROGRAM)

CFLAGS=$(CDEPS) $(CDEBUGFLAGS)
debug: $(LIBS_OBJ) $(CORE_OBJ)
#@ build program with debug in mind
	$(SILENT) echo "LD $(PROGRAM)"
	$(SILENT) $(CC) $(CFLAGS) $(CORE_OBJ) $(LIBS_OBJ) -o $(PROGRAM)

install: $(LIBS_OBJ) $(CORE_OBJ)
#@ build program and install it 
	$(SILENT) echo "LD $(PROGRAM)"
	$(SILENT) $(CC) $(CFLAGS) $(CORE_OBJ) $(LIBS_OBJ) -o $(PROGRAM)
	$(SILENT) cp pomodoro-timer /usr/local/bin
	$(SILENT) echo "pomodoro-timer installed. Use: 'pomodoro-timer'"

test:
#@ build test program
	$(SILENT) echo "TODO: test library and stuff pending"

clean:
#@ removes all object files
	$(SILENT) rm -rf obj/* lib/*/obj/*

run:
#@ executes the program once generated (will freak out if you don't compile anything)
	$(SILENT) ./$(PROGRAM)

obj/%.o: core/%.c
	$(SILENT) echo "CC $<"
	$(SILENT) $(CC) $(CFLAGS) -I./lib/ -o $@ -c $<

obj/%.a: lib/%
	$(SILENT) echo "MODULE $<"
	$(SILENT) $(MAKE) -s -C $< CFLAGS="$(CFLAGS)"
