#include <iostream>
#include <vector>
#include <chrono>
#include <stdlib.h>

#include "SDL3/SDL.h"
#include "SDL3/SDL_main.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

typedef struct Vec2f {
	float x;
	float y;
} Vec2f;

class Boid {
	public: 
		Vec2f pos, vel, accel;

		Boid(Vec2f pos, Vec2f vel, Vec2f accel) {
			this->pos = pos;
			this->vel = vel;
			this->accel = accel;
		}

		void update() {
			this->pos.x += this->vel.x;
			this->pos.y += this->vel.y;

			this->vel.x += this->accel.x;
			this->vel.y += this->accel.y;

			if(this->pos.x + 20 >= WINDOW_WIDTH || this->pos.x - 20 < 0) {
				this->vel.x *= -0.75f;
			}

			if(this->pos.y >= WINDOW_HEIGHT - 1) {
				this->vel.y *= -0.95f;
			} 
		}
};

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

	Boid b((Vec2f){ WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 } ,
			(Vec2f){ 0.05f, 0 }, 
			(Vec2f){ 0, 0.003f });

	while(!quit) {
		while(SDL_PollEvent(&e) != 0) {
			if(e.type == SDL_EVENT_QUIT) {
				quit = true;
			}
		}
		
		SDL_SetRenderTarget(renderer, texture);
		
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		
		
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderLine(renderer, b.pos.x - 20, b.pos.y , b.pos.x + 20, b.pos.y);
		SDL_RenderLine(renderer, b.pos.x - 20, b.pos.y , b.pos.x, b.pos.y - 34.641f);
		SDL_RenderLine(renderer, b.pos.x + 20, b.pos.y , b.pos.x, b.pos.y - 34.641f);
		
		SDL_SetRenderTarget(renderer, nullptr);
		SDL_RenderTexture(renderer, texture, nullptr, nullptr);
		
		SDL_RenderPresent(renderer);

		b.update();
	}


	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	return 0;
}
