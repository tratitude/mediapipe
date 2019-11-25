#ifndef LANDMARKS_DATATYPE_H
#define LANDMARKS_DATATYPE_H

#include <cmath>
#include <string>
#include <limits>
#include <iostream>

#define THREE_D
#define FLOAT_MIN 1e-10

namespace landmarks_datatype{
    struct coordinate3d_t{
        float x, y, z;

        coordinate3d_t(float _x = 0.0, float _y = 0.0, float _z = 0.0)
            :x(_x), y(_y), z(_z){}

        bool operator==(const coordinate3d_t p)
        {
            return (x==p.x && y==p.y && z==p.z);
        }

        coordinate3d_t& operator=(const coordinate3d_t &p)
        {
            x = p.x; y = p.y; z = p.z;
            return *this;
        }

        float distance(void)
        {
            float total = 0.f;
            if(fabs(x) > FLOAT_MIN)
                total += x*x;
            if(fabs(y) > FLOAT_MIN)
                total += y*y;
            return (sqrtf(total));
        }

        float distance3d(void)
        {
            float total = 0.f;
            if(fabs(x) > FLOAT_MIN)
                total += x*x;
            if(fabs(y) > FLOAT_MIN)
                total += y*y;
            if(fabs(z) > FLOAT_MIN)
                total += z*z;
            return (cbrtf(total));
        }
/*
        // cross product axb
        coordinate3d_t& cross(const coordinate3d_t &a, const coordinate3d_t &b)
        {
            x = a.y*b.z - a.z*b.y;
            y = a.z*b.x - a.x*b.z;
            z = a.x*b.y - a.y*b.x;
            return *this;
        }
*/
    #ifndef THREE_D
        coordinate3d_t operator+(const coordinate3d_t &p) const
        {
            return coordinate3d_t(x+p.x, y+p.y, z);
        }

        coordinate3d_t operator-(const coordinate3d_t &p) const
        {
            return coordinate3d_t(x-p.x, y-p.y, z);
        }

        // inner product
        coordinate3d_t operator*(const coordinate3d_t &p) const
        {
            return coordinate3d_t(x*p.x, y*p.y, z);
        }

        // scalar multiplication
        coordinate3d_t operator*(const float c) const
        {
            return coordinate3d_t(x*c, y*c, z);
        }
    #else
        coordinate3d_t operator+(const coordinate3d_t &p) const
        {
            return coordinate3d_t(x+p.x, y+p.y, z+p.z);
        }

        coordinate3d_t operator-(const coordinate3d_t &p) const
        {
            return coordinate3d_t(x-p.x, y-p.y, z-p.z);
        }

        // inner product
        coordinate3d_t operator*(const coordinate3d_t &p) const
        {
            return coordinate3d_t(x*p.x, y*p.y, z*p.z);
        }

        // scalar multiplication
        coordinate3d_t operator*(const float c) const
        {
            return coordinate3d_t(x*c, y*c, z*c);
        }
    #endif
    };

    struct gesture_t{
        coordinate3d_t *co;
        std::string name;
    };
    
    extern const int norm_landmark_size;
    extern const int shm_size;
    extern const char *norm_landmark_name;
    extern const char *shm_name;
    extern const char *bbCentral_name;
    extern const coordinate3d_t image_size;
    extern const char *gesture_path;

    coordinate3d_t cross_product(const coordinate3d_t &a, const coordinate3d_t &b);
    float dot_product(const coordinate3d_t &a, const coordinate3d_t &b);
    std::ostream& operator <<(std::ostream& os, const coordinate3d_t &p);
}
#endif