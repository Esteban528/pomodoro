# Pomodoro timer

## Dependencies:
- gcc
- pkg-config
- make
- libnotify

## Compilation:
`make clean debug`

## Nix usage:
`nix run github:Esteban528/pomodoro`

## NixOS Install:

flake.nix
```nix 
# add the input
pomodoro.url = "github:Esteban528/pomodoro";
```

```nix 
# declare the package
inputs.pomodoro.packages.${system}.default
```

## Other Linux distros 

- Install all dependencies
- clone the repo
- `make clean install`

---
# license
Public domain
