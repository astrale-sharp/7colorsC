
local dev: `gcc -Wall -Wextra -g  *.c -o main && ./main`

web dev: `emcc *.c -s ASYNCIFY --js-library=xterm-pty/emscripten-pty.js -o ./output/output.mjs`

Running test: in the folder: `gcc *.c -Dtest=main -o test.o && ./test.o`