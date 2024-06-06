{
  description = "small test framework for C";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-parts.url = "github:hercules-ci/flake-parts";
  };

  outputs = inputs @ {self, ...}:
    inputs.flake-parts.lib.mkFlake {inherit inputs;} {
      systems = ["x86_64-linux" "aarch64-linux" "aarch64-darwin"];
      perSystem = {
        system,
        pkgs,
        ...
      }: let
        gccVersion = "14";
        gccPkg = pkgs."gcc${gccVersion}";

        llvmVersion = "18";
        llvmPkgs = pkgs."llvmPackages_${llvmVersion}";

        clangStdEnv = pkgs.stdenvAdapters.overrideCC llvmPkgs.stdenv (llvmPkgs.clang.override {
          bintools = llvmPkgs.bintools;
        });
        clangTools = pkgs."clang-tools_${llvmVersion}";

        buildInputs = [pkgs.ninja pkgs.meson pkgs.cmake pkgs.pkg-config gccPkg llvmPkgs.clang clangTools];
        devTools = with pkgs; [ccls just nil statix alejandra];
      in {
        devShells.default = pkgs.mkShell.override {stdenv = clangStdEnv;} {
          buildInputs = buildInputs ++ devTools;
          shellHook = ''
            PATH="${clangTools}/bin:$PATH"
          '';
        };
      };
    };
}
