.PHONY: main

main: main.c
	mkdir -p build
	gcc -Wall -Wextra -o build/ledsb \
		-Wl,-rpath,./thirdparty/raylib-6.0_linux_amd64/lib \
		-L./thirdparty/raylib-6.0_linux_amd64/lib \
		-I./thirdparty/raylib-6.0_linux_amd64/include \
		main.c -lm -lraylib
