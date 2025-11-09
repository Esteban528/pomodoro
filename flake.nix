{
  description = "Minimal flake for developing and packaging in C";

  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";

  outputs = { self, nixpkgs }:
    let
      supportedSystems = [
        "x86_64-linux"
        "aarch64-linux"
        "x86_64-darwin"
        "aarch64-darwin"
      ];

      forEachSupportedSystem = f:
        nixpkgs.lib.genAttrs supportedSystems
          (system: f {
            pkgs = import nixpkgs { inherit system; };
            inherit system;
          });
    in
    {
      # Development environment (nix develop)
      devShells = forEachSupportedSystem ({ pkgs, system }: {
        default = pkgs.mkShell {
          packages = with pkgs; [
            tinycc
            pkg-config
            libnotify
            gnumake
          ] ++ (if system == "aarch64-darwin" then [] else [ gdb ]);
        };
      });

      # Buildable package (nix build)
      packages = forEachSupportedSystem ({ pkgs, system }: {
        default = pkgs.stdenv.mkDerivation {
          pname = "pomodoro-timer";
          version = "1.0";
          src = ./.;

          nativeBuildInputs = [ pkgs.tinycc pkgs.pkg-config pkgs.gnumake ];
          buildInputs = [ pkgs.libnotify ];

          buildPhase = ''
            mkdir -p obj
            for d in lib/*; do
              if [ -d "$d" ]; then
                mkdir -p "$d/obj"
              fi
            done
            make debug 
          '';

          installPhase = ''
            mkdir -p $out/bin
            cp pomodoro-timer $out/bin/
          '';
        };
      });
    };
}
