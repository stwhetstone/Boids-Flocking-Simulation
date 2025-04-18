#include <iostream>
#include <vector>
#include "Vec2f.hpp"

#ifndef BOID_H
#define BOID_H
namespace Simulation {
    
}
class Boid {
	public: 
		Vec2f pos, vel, accel;
		float maxForce, maxSpeed;
		int index;

		Boid() {
			this->pos = Vec2f();
			this->vel = Vec2f();
			this->accel = Vec2f();

			this->maxForce = 0;
			this->maxSpeed = 0;
			
			this->index = -1;
		}

		Boid(Vec2f pos, Vec2f vel, float maxSpeed, float maxForce, int index) {
			this->pos = pos;
			this->vel = vel;
			this->accel = Vec2f();

			this->maxSpeed = maxSpeed;
			this->maxForce = maxForce;

			this->index = index;
		}

		void update(float deltaTime, std::vector<Boid> flock) {
			Vec2f cohesion, alignment, separation, 
					avgPos, avgVel;
			int nearby = 0, range = 70;

			for(auto& boid : flock) {
				float dist = std::sqrt((this->pos.x() - boid.pos.x()) * (this->pos.x() - boid.pos.x()) + (this->pos.y() - boid.pos.y()) * (this->pos.y() - boid.pos.y()));
				if(boid.index != this->index && dist < range) {
						avgPos += boid.pos;

						nearby++;
				}
			}

			if(nearby > 0) {
				avgPos /= nearby;
				avgPos -= this->pos;
				avgPos.normalize();
				avgPos *= maxSpeed;

				cohesion = avgPos - this->vel;
				cohesion.limit(maxForce);
			}

			this->accel = cohesion;

			this->vel += this->accel * deltaTime;
            
			this->vel.limit(this->maxSpeed);
			this->pos += this->vel * deltaTime;

			this->accel *= 0;
		}

        void checkBoundary(const int w, const int h) {
            if(this->pos.x() > w) {
				this->pos.x(0);
			} else if(this->pos.x() < 0) {
				this->pos.x(w);
			}
			if(this->pos.y() > h) {
				this->pos.y(0);
			} else if(this->pos.y() < 0) {
				this->pos.y(h);
			}
        }
};

#endif