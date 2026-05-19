.PHONY: main

main: main.c
	mkdir -p build
	gcc -Wall -Wextra -o build/main \
		-lm -lraylib \
		-L./thirdparty/raylib-6.0_linux_amd64/lib \
		-I./thirdparty/raylib-6.0_linux_amd64/include \
		main.c
