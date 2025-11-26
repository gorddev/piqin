emcc src/main.cpp \
	-O2 \
	-s USE_SDL=2 \
	--shell-file ~/game/game_shell.html \
	-o index.html

if [ "$#" -ge 1 ] && [ "$1" == "1" ]; then
	echo "\033[1;32mRunning HTML\033[0m"
	emrun index.html
fi