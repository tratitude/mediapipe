#include "landmarks_datatype.h"
#include <cstdlib>
#include <limits.h>
#include <fstream>

namespace landmarks_datatype{
    const int norm_landmark_size = 21;
    // norm_landmark 4*3*21=252 bytes, bbCentral 4*3=12 bytes
    const int shm_size = 1024;
    const char *norm_landmark_name = "NL1";
    const char *norm_landmark_name_second = "NL2";
    const char *shm_name = "NeoHand1";
    const char *shm_name_second = "NeoHand2";
    const char *bbCentral_name = "BB1";
    const char *bbCentral_name_second = "BB2";

    coordinate3d_t cross_product(const coordinate3d_t &a, const coordinate3d_t &b)
    {
        float x = a.y*b.z - a.z*b.y;
        float y = a.z*b.x - a.x*b.z;
        float z = a.x*b.y - a.y*b.x;
        return {x, y, z};
    }

    float dot_product(const coordinate3d_t &a, const coordinate3d_t &b)
    {
    #ifdef THREE_D
        return a.x*b.x + a.y*b.y + a.z*b.z;
    #else
        return a.x*b.x + a.y*b.y;
    #endif
    }

    std::ostream& operator <<(std::ostream& os, const coordinate3d_t &p)
    {
        os << p.x << " " << p.y << " " << p.z << " " << p.angle << "\n";
        return os;
    }
}
