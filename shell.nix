{ pkgs ? import <nixpkgs> {} }:
  pkgs.mkShell {
    # nativeBuildInputs is usually what you want -- tools you need to run
    nativeBuildInputs = with pkgs.buildPackages; [ 
        pkgs.libnotify
        pkgs.pkg-config
        # pkgs.automake116x
        # pkgs.autoconf271
        # pkgs.check
        # pkgs.libxslt
        # pkgs.gperf
        #
        #
        # pkgs.xorg.libXau
        # pkgs.xorg.libxcb
        # pkgs.xcb-proto
    ];
}
