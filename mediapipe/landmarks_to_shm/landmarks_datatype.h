#ifndef LANDMARKS_DATATYPE_H
#define LANDMARKS_DATATYPE_H
namespace landmarks_datatype{
    struct normLand3d_t{
        float x, y, z;
    };

    struct letterbox_t{
        float left, top, right, bottom;
    };
    extern const int norm_landmark_size;
    extern const char *norm_landmark_name;
    extern const char *shm_name;
}
#endif