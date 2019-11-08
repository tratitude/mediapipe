#ifndef LANDMARKS_DATATYPE_H
#define LANDMARKS_DATATYPE_H

#include <cmath>
#include <string>

//#define THREE_D

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
        
    #ifndef THREE_D
        coordinate3d_t operator+(const coordinate3d_t &p) const
        {
            return coordinate3d_t(x+p.x, y+p.y, 0);
        }

        coordinate3d_t operator-(const coordinate3d_t &p) const
        {
            return coordinate3d_t(x-p.x, y-p.y, 0);
        }

        // inner product
        coordinate3d_t operator*(const coordinate3d_t &p) const
        {
            return coordinate3d_t(x*p.x, y*p.y, 0);
        }

        // scalar multiplication
        coordinate3d_t operator*(const float c) const
        {
            return coordinate3d_t(x*c, y*c, 0);
        }

        float distance(void)
        {
            return (sqrtf(x*x + y*y));
        }
    #elif
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

        float distance(void)
        {
            return (cbrtf(x*x + y*y + z*z));
        }
    #endif
    };

    struct gesture_t{
        coordinate3d_t *co;
        std::string name;
    };

    struct letterbox_t{
        float left, top, right, bottom;
    };
    
    extern const int norm_landmark_size;
    extern const int norm_landmark_shm_size;
    extern const char *norm_landmark_name;
    extern const char *shm_name;
}
#endif