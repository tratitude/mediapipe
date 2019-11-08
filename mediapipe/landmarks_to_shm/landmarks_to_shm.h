#ifndef LANDMARKS_TO_SHM_H
#define LANDMARKS_TO_SHM_H

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/allocators/allocator.hpp>

#include <cmath>

#include "landmarks_datatype.h"

#define PRINT_DEBUG

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
        void similarity(landmarks_datatype::coordinate3d_t* norm_landmark);
        void print_gestures(void);
        void rotate2d(landmarks_datatype::coordinate3d_t* norm_landmark);

        // Wraps around an angle in radians to within -M_PI and M_PI.
        inline float NormalizeRadians(float angle) {
            return angle - 2 * M_PI * std::floor((angle - (-M_PI)) / (2 * M_PI));
        }
        
        const int gesture_max_num = 32;
        int gesture_num;
        landmarks_datatype::gesture_t *gestures;
        const float similarity_lowbound = 0.8f;
        
    private:
        void init_gesture_num(void);

        const float target_angle_ = M_PI * 0.5f;  // In radians.
        const int start_keypoint_index_ = 0;  // wrist joint
        const int end_keypoint_index_ = 9;  // middle MCP joint
        const landmarks_datatype::coordinate3d_t image_size = {256.f, 256.f};
    };
}

#endif