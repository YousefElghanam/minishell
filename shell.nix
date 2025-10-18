{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  buildInputs = [
	pkgs.gnumake
	pkgs.norminette
	pkgs.clang
    pkgs.readline
    pkgs.ncurses  # often needed by readline
    pkgs.pkg-config  # helps detect libraries
  ];

  # If some tooling inside the shell needs to *find* the readline .so or .so paths at runtime, you can also set:
  shellHook = ''
	export LD_LIBRARY_PATH=${pkgs.readline}/lib:${pkgs.ncurses}/lib
	alias vgt="valgrind --track-fds=all --leak-check=full --trace-children=yes  --suppressions=readline.supp ./test"
  '';
}
