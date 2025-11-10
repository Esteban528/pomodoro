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
            dbus.dev
            tinycc
            gdb
            pkg-config
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
          buildInputs = [ dbus.dev ];

          buildPhase = ''
            make release
          '';

          installPhase = ''
            mkdir -p $out/bin
            cp pom_timer $out/bin/
          '';
        };
      });
    };
}
