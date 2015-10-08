emulador:
	gcc -o build/emulador.exe src/emulator.c src/main.c src/instr.c -std=c11 -Wall -Wextra -g -pg