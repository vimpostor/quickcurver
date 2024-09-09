{
	description = "Modern Qt/C++ implementation of Achtung die Kurve with online multiplayer";
	inputs = {
		quartz.url = "github:vimpostor/quartz";
	};

	outputs = { self, quartz }: quartz.lib.eachSystem (system:
		let
			pkgs = quartz.inputs.nixpkgs.legacyPackages.${system};
		in
		{
			packages = {
				default = pkgs.stdenv.mkDerivation {
					pname = "quickcurver";
					version = quartz.lib.cmakeProjectVersion ./CMakeLists.txt;

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
					cmakeFlags = quartz.lib.cmakeWrapper { inherit pkgs; cmakeFile = ./CMakeLists.txt; };
					postBuild = "make linux-desktop-integration";
				 };
			};
		}
	);
}
