{ stdenv, udev, libusb1 }:

stdenv.mkDerivation {
  name = "minichlink";
  src = ./..;
  postUnpack = ''
    export sourceRoot="$sourceRoot/minichlink"
  '';
  buildInputs = [ udev libusb1 ];
  installPhase = ''
    mkdir -pv $out/{bin,lib,lib/udev/rules.d}
    cp -v minichlink $out/bin/
    cp -v minichlink.so $out/lib/
    cp -v 99-minichlink.rules $out/lib/udev/rules.d/
  '';
}
