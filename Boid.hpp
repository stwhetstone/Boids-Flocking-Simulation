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
			Vec2f targetPos, targetVel, separation;
			int nearby = 0, range = 100;

			for(auto& boid : flock) {
				if(boid.index != this->index) {
					if(
						this->pos.x() + range > boid.pos.x() &&
						this->pos.x() - range < boid.pos.x() &&
						this->pos.y() + range > boid.pos.y() &&
						this->pos.y() - range < boid.pos.y()
					) {
                        Vec2f dist = this->pos - boid.pos;
                        dist.normalize();
                        separation += dist / (range * range * 1.0f);

						targetPos += boid.pos;
						targetVel += boid.vel;
						
						nearby++;
					}
				}
			}

			Vec2f cohesion, alignment;

			if(nearby > 0) {
				targetPos /= nearby;
                targetVel /= nearby;
                //separation /= nearby;

                
				Vec2f desiredVel(targetPos.x() - this->pos.x(), targetPos.y() - this->pos.y());
				desiredVel.normalize();
                desiredVel *= maxSpeed;

                separation.normalize();
                separation *= maxForce;
				
				cohesion = desiredVel - this->vel;
				alignment = targetVel - this->vel;
				separation -= this->vel;
                
                cohesion.limit(this->maxForce);
                alignment.limit(this->maxForce);
                separation.limit(this->maxForce);
			}
			this->accel = cohesion + alignment + separation * 1.1f;

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