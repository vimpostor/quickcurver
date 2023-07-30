{
	description = "Modern Qt/C++ implementation of Achtung die Kurve with online multiplayer";
	inputs = {
		nixpkgs.url = "github:NixOS/nixpkgs/nixos-23.05";
		flake-utils.url = "github:numtide/flake-utils";
	};

	outputs = { self, nixpkgs, flake-utils }: flake-utils.lib.eachDefaultSystem (system:
		let
			pkgs = nixpkgs.legacyPackages.${system};
		in {
			packages = rec {
				default = quickcurver;

				quickcurver = pkgs.stdenv.mkDerivation {
					pname = "quickcurver";
					version = "0.1";

					src = ./.;

					nativeBuildInputs = with pkgs; [
						cmake
						pkgconfig
						qt6.wrapQtAppsHook
						imagemagick
					];

					buildInputs = with pkgs; [
						qt6.qtbase
						qt6.qtdeclarative
						qt6.qtsvg
					];
					postBuild = "make linux-desktop-integration";
				 };
			};
		}
	);
}
