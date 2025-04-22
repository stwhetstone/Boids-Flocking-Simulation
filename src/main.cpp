#include <vector>
#include <cmath>
#include <chrono>

#include "SDL3/SDL.h"
#include "SDL3/SDL_main.h"

#include "Vec2f.hpp"
#include "Boid.hpp"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720


int main(int argc, char **argv) {
	if(!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
		return -1;
	}

	SDL_Window *window = SDL_CreateWindow("boids", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
	SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT);

	if(!window || !renderer || !texture) {
		return -1;
	}
	
	SDL_SetRenderVSync(renderer, 1);

	std::vector<Boid> flock;
	
	srand(time(NULL));
	for(int i = 0; i < 100; i++) {
		flock.push_back(Boid(
			Vec2f(rand() % WINDOW_WIDTH, rand() % WINDOW_HEIGHT),
			Vec2f(rand() % 250 - 250, rand() % 250 - 250),
			300, 
			30,
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
			Vec2f top(boid.pos.x(), boid.pos.y() + Boid::size * 4),
				bottomLeft(boid.pos.x() - Boid::size, boid.pos.y() + Boid::size / 2), 
				bottomRight(boid.pos.x() + Boid::size, boid.pos.y() + Boid::size / 2);
			
			// find angle between vel and x axis then rotate clockwise 90 degrees
			float angle = atan2(boid.vel.y(), boid.vel.x()) - 3.14159625 / 2.0;
			
			Vec2f::rotate(&top, &boid.pos, angle);
			Vec2f::rotate(&bottomLeft, &boid.pos, angle);
			Vec2f::rotate(&bottomRight, &boid.pos, angle);

			// bottom left to bottom right
			SDL_RenderLine(renderer, bottomLeft.x(), bottomLeft.y() , bottomRight.x(), bottomRight.y());
			// bottom left to top
			SDL_RenderLine(renderer, bottomLeft.x(), bottomLeft.y(), top.x(), top.y());
			// bottom right to top
			SDL_RenderLine(renderer, bottomRight.x(), bottomRight.y(), top.x(), top.y());
			
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
