#include "landmarks_datatype.h"

namespace landmarks_datatype{
    const int norm_landmark_size = 21;
    // norm_landmark 4*3*21=252 bytes, bbCentral 4*3=12 bytes
    const int shm_size = 1024;
    const char *norm_landmark_name = "NormLandArr";
    const char *norm_landmark_name_second = "NormLandArrSecond";
    const char *shm_name = "NormLandmarks";
    const char *bbCentral_name = "BoundingBoxCentral";
    const coordinate3d_t image_size = {256, 256, 1};
    const char *gesture_path = "/home/fdmdkw/code/project/mediapipe/store_gesture";

    coordinate3d_t cross_product(const coordinate3d_t &a, const coordinate3d_t &b)
    {
        float x = a.y*b.z - a.z*b.y;
        float y = a.z*b.x - a.x*b.z;
        float z = a.x*b.y - a.y*b.x;
        return {x, y, z};
    }

    float dot_product3d(const coordinate3d_t &a, const coordinate3d_t &b)
    {
        return a.x*b.x + a.y*b.y + a.z*b.z;
    }

    std::ostream& operator <<(std::ostream& os, const coordinate3d_t &p)
    {
        os << p.x << " " << p.y << " " << p.z << " " << p.angle << "\n";
        return os;
    }
}