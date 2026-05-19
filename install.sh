#!/usr/bin/bash

# built for arch/cachyos
# update the compiler tags for your OS
gcc -Wall -Wextra -g imgViewer.c -o imgViewer $(pkg-config --cflags --libs sdl2 SDL2_image)


cp ./imgViewer /usr/local/bin/