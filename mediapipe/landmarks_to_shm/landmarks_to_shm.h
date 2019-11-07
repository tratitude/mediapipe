#ifndef LANDMARKS_TO_SHM_H
#define LANDMARKS_TO_SHM_H

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/allocators/allocator.hpp>

#include "landmarks_datatype.h"

//#define PRINT_LANDMARKS
//#define PRINT_LETTERBOX
#define PRINT_SHM_LANDMARKS
#define PRINT_DEBUG

namespace landmarks_to_shm{
    class shm{
    public:
        shm(void);
        ~shm();
        void get_normLandVector(landmarks_datatype::normLand3d_t**);
        void print_shm_norm_landmarks(void);
    };

    class gesture{
    public:
        gesture(void);
        void store_gesture(void);
        void store_gesture(int gesture_num);
        void similarity(void);
        ~gesture();
        const int gesture_max_num = 32;
    };
}

#endif