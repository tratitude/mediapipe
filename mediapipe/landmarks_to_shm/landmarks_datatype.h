#ifndef LANDMARKS_DATATYPE_H
#define LANDMARKS_DATATYPE_H
namespace landmarks_datatype{
    struct normLand3d_t{
        float x, y, z;

        normLand3d_t(float _x = 0.0, float _y = 0.0, float _z = 0.0)
            :x(_x), y(_y), z(_z){}

        bool operator==(const normLand3d_t n)
        {
            return (x==n.x && y==n.y && z==n.z);
        }
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