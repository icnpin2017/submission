{ nixpkgs ? import <nixpkgs> {} }:
nixpkgs.stdenv.mkDerivation {
  name = "merge-opt";
  buildInputs = with nixpkgs; [
    python35Packages.python python35Packages.click python35Packages.ipython
    python35Packages.numpy
    cmake ];
}

