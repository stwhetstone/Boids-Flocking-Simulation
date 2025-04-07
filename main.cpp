#include <iostream>

#include "SDL3/SDL.h"
#include "SDL3/SDL_main.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600


int main() {
	if(!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
		return -1;
	}

	SDL_Window *window = SDL_CreateWindow("boids", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
	SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, WINDOW_WIDTH, WINDOW_HEIGHT);


	if(!window || !renderer || !texture) {
		return -1;
	}

	SDL_Event e;
	bool quit = false;

	while(!quit) {
		while(SDL_PollEvent(&e) != 0) {
			if(e.type == SDL_EVENT_QUIT) {
				quit = true;
			}
		}
	}

	SDL_SetRenderTarget(renderer, texture);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	SDL_SetRenderTarget(renderer, nullptr);
	SDL_RenderTexture(renderer, texture, nullptr, nullptr);

	SDL_RenderPresent(renderer);

	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	return 0;
}
