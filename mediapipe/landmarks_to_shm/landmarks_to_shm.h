#ifndef LANDMARKS_TO_SHM_H
#define LANDMARKS_TO_SHM_H

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/allocators/allocator.hpp>

#include "landmarks_datatype.h"

//#define PRINT_DEBUG

namespace landmarks_to_shm{
    class shm{
    public:
        shm(void);
        ~shm();
        void get_normLandVector(landmarks_datatype::coordinate3d_t**);
        void print_shm_norm_landmarks(void);
    };

    class gesture{
    public:
        gesture(void);
        ~gesture();
        void store_gesture(void);
        void store_gesture(int gesture_num);
        void load_gesture(std::string dir);
        void similarity(void);
        void print_gestures(void);
        
        const int gesture_max_num = 32;
        int gesture_num;
        landmarks_datatype::gesture_t *gestures;
        
    private:
        void init_gesture_num(void);
    };
}

#endif