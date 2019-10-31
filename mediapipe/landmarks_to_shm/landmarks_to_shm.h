#ifndef LANDMARKS_TO_SHM
#define LANDMARKS_TO_SHM

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/allocators/allocator.hpp>

//#define PRINT_LANDMARKS
//#define PRINT_LETTERBOX
#define PRINT_SHM_LANDMARKS
#define PRINT_DEBUG

namespace landmarks_to_shm{
    struct normLand3d_t{
        float x, y, z;
    };

    struct letterbox_t{
        float left, top, right, bottom;
    };

    class shm{
    public:
        shm(void);
        ~shm();
        void get_normLandVector(normLand3d_t**);
        void print_shm_norm_landmarks(void);

        const int norm_landmark_size = 21;
        const char *norm_landmark_name = "NormLandArr";
        const char *shm_name = "NormLandmarks";
    };
}

#endif