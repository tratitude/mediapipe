#ifndef LANDMARKS_DATATYPE_H
#define LANDMARKS_DATATYPE_H

#include <cmath>

//#define THREE_D

namespace landmarks_datatype{
    struct normLand3d_t{
        float x, y, z;

        normLand3d_t(float _x = 0.0, float _y = 0.0, float _z = 0.0)
            :x(_x), y(_y), z(_z){}
    #ifndef THREE_D
        bool operator==(const normLand3d_t p)
        {
            return (x==p.x && y==p.y);
        }

        normLand3d_t& operator=(const normLand3d_t &p)
        {
            x = p.x; y = p.y;
            return *this;
        }

        normLand3d_t operator+(const normLand3d_t &p) const
        {
            return normLand3d_t(x+p.x, y+p.y, 0);
        }

        normLand3d_t operator-(const normLand3d_t &p) const
        {
            return normLand3d_t(x-p.x, y-p.y, 0);
        }

        // inner product
        normLand3d_t operator*(const normLand3d_t &p) const
        {
            return normLand3d_t(x*p.x, y*p.y, 0);
        }

        // scalar multiplication
        normLand3d_t operator*(const float c) const
        {
            return normLand3d_t(x*c, y*c, 0);
        }

        float distance(void)
        {
            return (sqrtf(x*x + y*y));
        }
    #elif
        bool operator==(const normLand3d_t p)
        {
            return (x==p.x && y==p.y && z==p.z);
        }

        normLand3d_t& operator=(const normLand3d_t &p)
        {
            x = p.x; y = p.y; z = p.z;
            return *this;
        }

        normLand3d_t operator+(const normLand3d_t &p) const
        {
            return normLand3d_t(x+p.x, y+p.y, z+p.z);
        }

        normLand3d_t operator-(const normLand3d_t &p) const
        {
            return normLand3d_t(x-p.x, y-p.y, z-p.z);
        }

        // inner product
        normLand3d_t operator*(const normLand3d_t &p) const
        {
            return normLand3d_t(x*p.x, y*p.y, z*p.z);
        }

        // scalar multiplication
        normLand3d_t operator*(const float c) const
        {
            return normLand3d_t(x*c, y*c, z*c);
        }

        float distance(void)
        {
            return (cbrtf(x*x + y*y + z*z));
        }
    #endif
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