{
	description = "Modern Qt/C++ implementation of Achtung die Kurve with online multiplayer";
	inputs = {
		nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
	};

	outputs = { self, nixpkgs }:
	let eachSystem = with nixpkgs.lib; f: foldAttrs mergeAttrs {} (map (s: mapAttrs (_: v: { ${s} = v; }) (f s)) systems.flakeExposed);
	in eachSystem (system:
		let
			pkgs = nixpkgs.legacyPackages.${system};
			quartz = pkgs.fetchFromGitHub {
				owner = "vimpostor";
				repo = "quartz";
				rev = builtins.head (builtins.match ".*FetchContent_Declare\\(.*GIT_TAG ([[:alnum:]\\.]+).*" (builtins.readFile ./CMakeLists.txt));
				hash = "sha256-gni+5kQkjU1LhoEK6/yVC0x6K36Ra30xM47w893nGJ4=";
			};
		in
		{
			packages = {
				default = pkgs.stdenv.mkDerivation {
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
