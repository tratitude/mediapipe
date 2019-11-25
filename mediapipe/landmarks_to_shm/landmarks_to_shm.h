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
        // seg fault
        void get_norm_landmark3d_ptr(landmarks_datatype::coordinate3d_t* _norm_landmark3d_ptr);
        void get_bbCentral_ptr(landmarks_datatype::coordinate3d_t* _bbCentral_ptr);
        void print_shm_norm_landmark3d(void);
        void print_shm_bbCentral(void);
        void print_shm(const char *_val_shm_name);
    };

    class gesture{
    public:
        gesture(void);
        ~gesture();
        void store_gestures3d(const std::string &_dir);
        void store_gestures3d(int _gesture_num, const std::string &_dir);
        void load_resize_rotate_gestures3d(const std::string &_dir);
        void print_gestures3d(void);
        void init_gestures3d(void);
        void delete_gestures3d(void);

        void init_norm_landmark3d(void);
        void delete_norm_landmark3d(void);
        void load_resize_rotate_norm_landmark3d(landmarks_datatype::coordinate3d_t *_shm_norm_landmark3d);

        float similarity(void);
        void gesture_similarity_test(void);
        void rotate2d_y(landmarks_datatype::coordinate3d_t* _landmark3d);
        void rotate3d_yz(landmarks_datatype::coordinate3d_t* _landmark3d);
        void rotate3d_z(landmarks_datatype::coordinate3d_t* _landmark3d);
        void resize(landmarks_datatype::coordinate3d_t* _norm_landmark3d);
        void norm_root(landmarks_datatype::coordinate3d_t* _landmark3d);
        void Rodrigues_z(landmarks_datatype::coordinate3d_t* _landmark3d);
        void resize_rotate3d(landmarks_datatype::coordinate3d_t* _landmark3d);

        void init_crossVector(landmarks_datatype::coordinate3d_t* _landmark3d);

        // Wraps around an angle in radians to within -M_PI and M_PI.
        inline float NormalizeRadians(float angle) {
            return angle - 2 * M_PI * std::floor((angle - (-M_PI)) / (2 * M_PI));
        }

    private:
        void init_gesture_num(void);

        int gesture_num_;
        landmarks_datatype::coordinate3d_t* norm_landmark3d_;
        // x=x_center, y=y_center, z=match_gesture
        landmarks_datatype::gesture_t *gestures3d_;
        landmarks_datatype::coordinate3d_t crossVector;

        const float target_angle_ = M_PI * 0.5f;  // In radians.
        const int start_keypoint_index_ = 0;  // wrist joint
        const int end_keypoint_index_ = 9;  // middle MCP joint
        const int gesture_max_num_ = 32;
        const float similarity_lowbound_ = -150.f;
        const int start_crossVector_index_ = 13;
        const int end_crossVector_index_ = 5;
    };
}

#endif