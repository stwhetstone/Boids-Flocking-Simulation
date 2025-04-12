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

		void update(float deltaTime) {
			this->pos.x += this->vel.x * deltaTime;
			this->pos.y += this->vel.y * deltaTime;

			this->vel.x += this->accel.x * deltaTime;
			this->vel.y += this->accel.y * deltaTime;

			if(this->pos.x + 20 >= WINDOW_WIDTH - 1) {
				this->pos.x = WINDOW_WIDTH - 21;
				this->vel.x *= -0.75f;
			} else if (this->pos.x - 20 < 1) {
				this->pos.x = 21;
				this->vel.x *= -0.75f;
			}

			if(this->pos.y >= WINDOW_HEIGHT - 1) {
				this->pos.y = WINDOW_HEIGHT - 1;
				this->vel.y *= -0.75f;
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
	
	SDL_SetRenderVSync(renderer, 1);

	std::vector<Boid> flock;
	
	
	flock.push_back(Boid(
		(Vec2f){ WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 } ,
		(Vec2f){ 150.0f, 0 }, 
		(Vec2f){ 0, 500.0f }
	));
	flock.push_back(Boid(
		(Vec2f){ WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 } ,
		(Vec2f){ 100.0f, 0 }, 
		(Vec2f){ 0, 500.0f }
	));
	flock.push_back(Boid(
		(Vec2f){ WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 } ,
		(Vec2f){ 200.0f, 0 }, 
		(Vec2f){ 0, 500.0f }
	));
	
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
			SDL_RenderLine(renderer, boid.pos.x - 20, boid.pos.y , boid.pos.x + 20, boid.pos.y);
			SDL_RenderLine(renderer, boid.pos.x - 20, boid.pos.y , boid.pos.x, boid.pos.y - 34.641f);
			SDL_RenderLine(renderer, boid.pos.x + 20, boid.pos.y , boid.pos.x, boid.pos.y - 34.641f);
			
			boid.update(deltaTime);
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
