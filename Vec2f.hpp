#include <cmath>
#include <algorithm>

#ifndef VEC2F_H
#define VEC2F_H

class Vec2f {
    public:
        float length;
    
        Vec2f() {
            this->_x = 0;
            this->_y = 0;
            
            this->length = 0;
        }
        Vec2f(float x, float y) {
            this->_x = x;
            this->_y = y;
            
            this->length = std::sqrt(this->_x * this->_x + this->_y * this->_y);
        }

        float x() {
            return this->_x;
        }
        
        void x(float x) {
            this->_x = x;
            this->length = std::sqrt(this->_x * this->_x + this->_y * this->_y);
        }

        float y() {
            return this->_y;
        }
        
        float y(float y) {
            this->_y = y;
            this->updateLength();
        }

        void limit(float max) {
            if(this->_x >= 0) {
                this->x(std::min(this->_x, max));
            } else if(this->_x < 0) {
                this->x(std::max(this->_x, -1.0f * max));
            }
        
            if(this->_y >= 0) {
                this->y(std::min(this->_y, max));
            } else if(this->_y < 0) {
                this->y(std::max(this->_y, -1.0f * max));
            }
        }

        void normalize() {
            this->_x /= this->length;
            this->_y /= this->length;
        }
        
        Vec2f operator + (Vec2f other) {
            return Vec2f(this->_x + other.x(), this->_y + other.y());
        }

        Vec2f operator - (Vec2f other) {
            return Vec2f(this->_x - other.x(), this->_y - other.y());
        }

        Vec2f operator * (float a) {
            return Vec2f(this->_x * a, this->_y * a );
        }

        Vec2f operator / (float a) {
            return Vec2f(this->_x / a, this->_y / a );
        }
        
        void operator += (Vec2f other) {
            this->_x += other.x();
            this->_y += other.y();
            
            this->updateLength();
        }

        void operator -= (Vec2f other) {
            this->_x -= other.x();
            this->_y -= other.y();
            
            this->updateLength();
        }

        void operator *= (float a) {
            this->_x *= a;
            this->_y *= a;
            
            this->updateLength();
        }

        void operator /= (float a) {
            this->_x /= a;
            this->_y /= a;
            
            this->updateLength();
        }

        void operator = (Vec2f a) {
            this->_x = a.x();
            this->_y = a.y();
            
            this->updateLength();
        }
        
    private:
        float _x, _y;
        
        void updateLength() {
            this->length = std::sqrt(this->_x * this->_x + _y * this->_y);

        }
    
        
        
};


#endif




