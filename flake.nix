{
	description = "Modern Qt/C++ implementation of Achtung die Kurve with online multiplayer";
	inputs = {
		nixpkgs.url = "github:NixOS/nixpkgs/nixos-23.05";
		flake-utils.url = "github:numtide/flake-utils";
	};

	outputs = { self, nixpkgs, flake-utils }: flake-utils.lib.eachDefaultSystem (system:
		let
			pkgs = nixpkgs.legacyPackages.${system};
			quartz = pkgs.fetchFromGitHub {
				owner = "vimpostor";
				repo = "quartz";
				rev = builtins.head (builtins.match ".*FetchContent_Declare\\(.*GIT_TAG ([[:alnum:]\\.]+).*" (builtins.readFile ./CMakeLists.txt));
				hash = "sha256-cbU+VTsXetn31L1znMa/f1oqoQ8Zpond/I+Lk5jVxR0=";
			};
		in {
			packages = rec {
				default = quickcurver;

				quickcurver = pkgs.stdenv.mkDerivation {
					pname = "quickcurver";
					version = "0.1";

					src = ./.;

					nativeBuildInputs = with pkgs; [
						cmake
						pkg-config
						qt6.wrapQtAppsHook
						imagemagick
					];
					buildInputs = with pkgs; [
						qt6.qtbase
						qt6.qtdeclarative
						qt6.qtsvg
					];
					cmakeFlags = [("-DFETCHCONTENT_SOURCE_DIR_QUARTZ=" + quartz)];
					postBuild = "make linux-desktop-integration";
				 };
			};
		}
	);
}
