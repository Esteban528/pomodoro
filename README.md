# Pomodoro C Timer

Simple pomodoro-technique timing tool, supports pause, bell rings when pomodoro ends.

## Deps
- dbus.dev
- pipewire *(if you want to hear the bell)*

## Usage:
Just run the binary file and use it.

[Screenshots](https://cloud.estebandev.xyz/public/dir/esteban@estebandev.xyz/PublicGallery/pom)

## Makefile
`make <options>`
`debug` compile to development mode
`release` compile to "ready to use" mode
`run` run the binary

## Nix

- Nix install:
```nix 
nix profile add github:Esteban528/pomodoro
```

- NixOS flake installing
flake.nix
```nix 
# add the input
pomodoro.url = "github:Esteban528/pomodoro";
```

```nix 
# declare the package
inputs.pomodoro.packages.${system}.default
```

## License
This is public domain 
