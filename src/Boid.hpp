#include <vector>
#include <cmath>

#include "Vec2.hpp"

#ifndef BOID_H
#define BOID_H

class Boid {
	public: 
		static int size;
		Vec2<float> pos, vel, accel;
		float maxForce, maxSpeed;
		int index;

		Boid() {
			this->pos = Vec2<float>();
			this->vel = Vec2<float>();
			this->accel = Vec2<float>();

			this->maxForce = 0;
			this->maxSpeed = 0;
			
			this->index = -1;
		}

		Boid(Vec2<float> pos, Vec2<float> vel, float maxSpeed, float maxForce, int index) {
			this->pos = pos;
			this->vel = vel;
			this->accel = Vec2<float>();

			this->maxSpeed = maxSpeed;
			this->maxForce = maxForce;

			this->index = index;
		}

		void update(float deltaTime, std::vector<Boid>& flock) {
			Vec2<float> cohesion, alignment, separation, 
					avgPos, avgVel, repulsion;
			int nearby = 0, range = 40;

			for(auto& boid : flock) {
				float dist = std::sqrt(
										(this->pos.x - boid.pos.x) * (this->pos.x - boid.pos.x) + 
										(this->pos.y - boid.pos.y) * (this->pos.y - boid.pos.y)
									);
				if(boid.index != this->index && dist < range) {
						avgPos += boid.pos;
						avgVel += boid.vel;
						repulsion += (this->pos - boid.pos) / (dist * dist);

						nearby++;
				}
			}

			/* 
				https://www.red3d.com/cwr/steer/gdc99/
				    steering_force = truncate (steering_direction, max_force)
					acceleration = steering_force / mass
					velocity = truncate (velocity + acceleration, max_speed)
					position = position + velocity

					desired_velocity = normalize (position - target) * max_speed
    				steering = desired_velocity - velocity
			*/
			if(nearby > 0) {
				avgPos /= nearby;
				avgPos.normalize();
				avgPos *= maxSpeed;

				cohesion = avgPos - this->vel;
				cohesion.limit(maxForce);

				avgVel /= nearby;
				avgVel.normalize();
				avgVel *= maxSpeed;

				alignment = avgVel - this->vel;
				alignment.limit(maxForce);

				repulsion /= nearby;
				repulsion.normalize();
				repulsion *= maxSpeed;

				separation = repulsion - this->vel;
				separation.limit(maxForce); 
			}

			float cwt = 3, awt = 2, swt = 0.7;
			this->accel = (cohesion * cwt) + (alignment * awt) + (separation * swt);
			
			this->vel += this->accel * deltaTime;
			this->vel.limit(this->maxSpeed);
            
			this->pos += this->vel * deltaTime;

			this->accel *= 0;
		}

        void checkBoundary(const int w, const int h) {
            if(this->pos.x > w) {
				this->pos.x = 0;
			} else if(this->pos.x < 0) {
				this->pos.x = w;
			}
			if(this->pos.y > h) {
				this->pos.y = 0;
			} else if(this->pos.y < 0) {
				this->pos.y = h;
			}
        }
};

int Boid::size = 4;

#endif
