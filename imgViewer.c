#include <SDL2/SDL_events.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <sys/types.h>


#ifdef _WIN32
    #include <io.h>
    #define isatty _isatty  // Windows names it _isatty
#else
    #include <unistd.h>     // Linux/macOS use unistd.h
#endif


void fuckery(SDL_Surface *ptr_surface, SDL_Window *ptr_window, int width, int height){

	uint32_t color = SDL_MapRGB(ptr_surface->format, 0x00, 0xFF, 0x00);
	SDL_Rect pixel;
	for(int y = 0; y < height; y+=20){
		for (int x = 0; x<width; x+=20){
			pixel = (SDL_Rect){x, y, 20, 20};

			SDL_FillRect(ptr_surface, &pixel, color);

			SDL_UpdateWindowSurface(ptr_window);
		}
	}
}

int main(int argc, char* argv[]){

	FILE *ptr_stdin = (argc > 1) ? fopen(argv[1], "r") : stdin;
	if (ptr_stdin == NULL) {
		perror("Error opening file or no input given");
		printf("Usage: imgViewer ./FILEPATH_OF_PPM_IMG or imgViewer < PPM_IMG");
		return 1;
    }

	char *ptr_throwaway = calloc(1000, sizeof(char));

	// Header Data From PPM File
	int img_width = -1;
	int img_height = -1;
	int MAX_COLOR_VALUE = 0;

	// Read First Line (specifier p3 or p6 - ignore )
	fgets(ptr_throwaway, 1000, ptr_stdin);
	// Read Second Line (comment - ignore )
	fgets(ptr_throwaway, 1000, ptr_stdin);

	// Read Third Line (dimensions: width & height)
	fgets(ptr_throwaway, 1000, ptr_stdin);
	sscanf(ptr_throwaway, "%d %d\n", &img_width, &img_height);

	// Read Fourth (Max Color Value )
	fgets(ptr_throwaway, 1000, ptr_stdin);
	sscanf(ptr_throwaway, "%d", &MAX_COLOR_VALUE);

	free(ptr_throwaway);


	printf("width %d and height %d of the ppm image with max color value of %d\n", img_width, img_height, MAX_COLOR_VALUE);


	SDL_Window *ptr_window = SDL_CreateWindow("Musabs Image Viewer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, img_width, img_height, 0);

	SDL_Surface *ptr_surface = SDL_GetWindowSurface(ptr_window);

	Uint32 color = SDL_MapRGB(ptr_surface->format, 0x00, 0x00, 0x00);

	SDL_Rect pixel= (SDL_Rect){0, 0, 1, 1};

	//fuckery(ptr_surface, ptr_window, width, height);

	for (int y = 0; y < img_height; y++){
		int x = 0;
		for (;x < img_width; x++){
			uint8_t r, g, b;
			r = (char)fgetc(ptr_stdin); g = (char)fgetc(ptr_stdin); b = (char)fgetc(ptr_stdin); 
			color = SDL_MapRGB(ptr_surface->format, r,  g, b);

			//printf("color of current pixel %x ", color);

			pixel.x = x; pixel.y = y;
			SDL_FillRect(ptr_surface, &pixel, color);
			
		}
		SDL_UpdateWindowSurface(ptr_window);
	}

	Uint8 apprunning = 1;
	while(apprunning){
		SDL_Event event;
		while(SDL_PollEvent(&event)){
			if(event.type == SDL_QUIT){
				apprunning = 0;
			}
		}
		SDL_Delay(100);
	}

	return 0;
}




