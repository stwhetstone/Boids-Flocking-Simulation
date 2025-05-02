#include <cmath>
#include <algorithm>

#ifndef VEC2F_H
#define VEC2F_H

template <typename T>
class Vec2 {
    public:
        T x, y;

        Vec2() {
            this->x = 0;
            this->y = 0;
        }
        
        Vec2(T x, T y) {
            this->x = x;
            this->y = y;
        }


        static void rotate(Vec2& v, Vec2& center, T angle) {
            T tmpx = v.x, tmpy = v.y; 
            v.x = center.x + ((tmpx - center.x) * cos(angle) - (tmpy - center.y) * sin(angle));
			v.y = center.y + ((tmpx - center.x) * sin(angle) + (tmpy - center.y) * cos(angle));
        }


        float length() {
            return std::sqrt(this->x * this->x + this->y * this->y);
        }

        void normalize() {
            this->x /= length();
            this->y /= length();
        }

        void limit(T max) {
            if(this->x >= 0) {
                this->x = std::min(this->x, max);
            } else if(this->x < 0) {
                this->x = std::max(this->x, -1.0f * max);
            }
        
            if(this->y >= 0) {
                this->y = std::min(this->y, max);
            } else if(this->y < 0) {
                this->y = std::max(this->y, -1.0f * max);
            }
        }


        Vec2 operator + (Vec2 other) {
            return Vec2(this->x + other.x, this->y + other.y);
        }

        Vec2 operator - (Vec2 other) {
            return Vec2(this->x - other.x, this->y - other.y);
        }

        Vec2 operator * (T a) {
            return Vec2(this->x * a, this->y * a );
        }

        Vec2 operator / (T a) {
            return Vec2(this->x / a, this->y / a );
        }
        
        void operator += (Vec2 other) {
            this->x += other.x;
            this->y += other.y;
        }

        void operator -= (Vec2 other) {
            this->x -= other.x;
            this->y -= other.y;
        }

        void operator *= (T a) {
            this->x *= a;
            this->y *= a;
        }

        void operator /= (T a) {
            this->x /= a;
            this->y /= a;
        }

        void operator = (Vec2 a) {
            this->x = a.x;
            this->y = a.y;
        }
};
#endif
