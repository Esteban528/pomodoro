* Project Modules

Modules are located at the `lib/` directory.

Each and every module has their own `makefile`, `interface.h` and `implementation.c`. It has it's own `obj/` directory, where all files get before makign the output <module-name>.a at the projects root `obj/`.

The `create-module` script can create modules for you with all template files copied to where they belong. Although you should check them for the following tweaks:

- `makefile` should indicate the name of the module and should be equal to the directory's name.
- `interface.h` nor `implementation.c` have anything at the time of being created, and you should populate them with code.