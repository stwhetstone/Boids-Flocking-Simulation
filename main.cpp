#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <cmath>
#include <stdlib.h>

#include "SDL3/SDL.h"
#include "SDL3/SDL_main.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

typedef struct Vec2f {
	float x;
	float y;
} Vec2f;

void limitVec(Vec2f *vec, float max) {
	if(vec->x >= 0) {
		vec->x = std::min(vec->x, max);
	} else if(vec->x < 0) {
		vec->x = std::max(vec->x, -1.0f * max);
	}

	if(vec->y >= 0) {
		vec->y = std::min(vec->y, max);
	} else if(vec->y < 0) {
		vec->y = std::max(vec->y, -1.0f * max);
	}
}

class Boid {
	public: 
		Vec2f pos, vel, accel;
		float maxForce, maxSpeed;
		int index;

		Boid(Vec2f pos, Vec2f vel, float maxSpeed, float maxForce, int index) {
			this->pos = pos;
			this->vel = vel;
			this->accel = (Vec2f){ 0.0, 0.0 };

			this->maxSpeed = maxSpeed;

			this->maxForce = maxForce;

			this->index = index;
		}

		void update(float deltaTime, std::vector<Boid> flock) {
			Vec2f targetPos = { 0.0f , 0.0f };
			int nearby = 0, range = 100;

			for(const auto& boid : flock) {
				if(boid.index != this->index) {
					if(
						this->pos.x + range > boid.pos.x &&
						this->pos.x - range < boid.pos.x &&
						this->pos.y + range > boid.pos.y &&
						this->pos.y - range < boid.pos.y
					) {
						targetPos.x += boid.pos.x;
						targetPos.y += boid.pos.y;
						
						nearby++;
					}
				}
			}

			Vec2f cohesion = {0.0f, 0.0f};

			if(nearby > 0) {
				targetPos.x /= nearby;
				targetPos.y /= nearby;

				Vec2f desiredVel = {targetPos.x - this->pos.x, targetPos.y - this->pos.y};
				
				float mag = std::sqrt(desiredVel.x * desiredVel.x + desiredVel.y * desiredVel.y);
				if(mag > 0) {
					desiredVel.x = (desiredVel.x / mag) * this->maxSpeed;
					desiredVel.y = (desiredVel.y / mag) * this->maxSpeed;
				}

				cohesion = (Vec2f){desiredVel.x - this->vel.x, desiredVel.y - this->vel.y};
			}

			
			limitVec(&cohesion, this->maxForce);
			this->accel.x = cohesion.x * 2;
			this->accel.y = cohesion.y * 2;
			
			this->vel.x += this->accel.x * deltaTime;
			this->vel.y += this->accel.y * deltaTime;
			
			limitVec(&this->vel, this->maxSpeed);
			this->pos.x += this->vel.x * deltaTime;
			this->pos.y += this->vel.y * deltaTime;
			
			if(this->pos.x > WINDOW_WIDTH) {
				this->pos.x = 0;
			} else if(this->pos.x < 0) {
				this->pos.x = WINDOW_WIDTH;
			}
			if(this->pos.y > WINDOW_HEIGHT) {
				this->pos.y = 0;
			} else if(this->pos.y < 0) {
				this->pos.y = WINDOW_HEIGHT;
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
	
	srand(time(NULL));
	for(int i = 0; i < 100; i++) {
		flock.push_back(Boid(
			(Vec2f){ WINDOW_WIDTH / 2 + rand() % 10, WINDOW_HEIGHT / 2 + rand() % 10 } ,
			(Vec2f){ rand() % 500 - 250, rand() % 500 - 250 },
			300, 
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
			SDL_RenderLine(renderer, boid.pos.x - 10, boid.pos.y , boid.pos.x + 10, boid.pos.y);
			SDL_RenderLine(renderer, boid.pos.x - 10, boid.pos.y , boid.pos.x, boid.pos.y - 17);
			SDL_RenderLine(renderer, boid.pos.x + 10, boid.pos.y , boid.pos.x, boid.pos.y - 17);
			
			boid.update(deltaTime, flock);
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
