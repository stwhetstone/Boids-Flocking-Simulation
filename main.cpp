#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <cmath>
#include <stdlib.h>

#include "SDL3/SDL.h"
#include "SDL3/SDL_main.h"

#include "Vec2f.hpp"
#include "Boid.hpp"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720


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
	
	SDL_SetRenderVSync(renderer, 1);

	std::vector<Boid> flock;
	
	srand(time(NULL));
	for(int i = 0; i < 100; i++) {
		flock.push_back(Boid(
			Vec2f(WINDOW_WIDTH / 2 + rand() % 100, WINDOW_HEIGHT / 2 + rand() % 100),
			Vec2f(rand() % 250 - 250, rand() % 250 - 250),
			250, 
			100,
			i
		));
	}

	SDL_Event e;
	bool quit = false;

	std::chrono::steady_clock::time_point curTime = std::chrono::steady_clock::now();
	std::chrono::steady_clock::time_point prevTime = curTime;	

	while(!quit) {
		while(SDL_PollEvent(&e) != 0) {
			if(e.type == SDL_EVENT_QUIT) {
				quit = true;
			}
		}

		curTime = std::chrono::steady_clock::now();
		std::chrono::duration<float> time = curTime - prevTime;
		prevTime = curTime;

		float deltaTime = time.count();
		
		SDL_SetRenderTarget(renderer, texture);
		
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		for(auto &boid : flock) {
			SDL_RenderLine(renderer, boid.pos.x() - 10, boid.pos.y() , boid.pos.x() + 10, boid.pos.y());
			SDL_RenderLine(renderer, boid.pos.x() - 10, boid.pos.y() , boid.pos.x(), boid.pos.y() - 17);
			SDL_RenderLine(renderer, boid.pos.x() + 10, boid.pos.y() , boid.pos.x(), boid.pos.y() - 17);
			
			boid.update(deltaTime, flock);
			boid.checkBoundary(WINDOW_WIDTH, WINDOW_HEIGHT);
		}
		
		SDL_SetRenderTarget(renderer, nullptr);
		SDL_RenderTexture(renderer, texture, nullptr, nullptr);
		
		SDL_RenderPresent(renderer);
	}

	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	return 0;
}
