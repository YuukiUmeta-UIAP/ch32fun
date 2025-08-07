{
  inputs = {
    utils.url = "github:numtide/flake-utils";
  };
  outputs = { self, nixpkgs, utils }:
  (utils.lib.eachDefaultSystem (system:
  let
    pkgs = nixpkgs.legacyPackages.${system}.extend self.overlay;
  in {
    packages.default = pkgs.minichlink;
  })) // {
    overlay = self: super: {
      minichlink = self.callPackage ./. {};
    };
  };
}
