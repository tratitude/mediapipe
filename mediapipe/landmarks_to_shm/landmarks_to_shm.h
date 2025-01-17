#ifndef LANDMARKS_TO_SHM_H
#define LANDMARKS_TO_SHM_H

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/allocators/allocator.hpp>

#include <cmath>

#include "landmarks_datatype.h"


namespace landmarks_to_shm{
    class shm{
    public:
        shm(const char _landmark_shm_name[], const char _bbCentral_shm_name[], const char _shm_name[]);
        ~shm();
        // seg fault
        void get_norm_landmark3d_ptr(landmarks_datatype::coordinate3d_t* _norm_landmark3d_ptr);
        void get_bbCentral_ptr(landmarks_datatype::coordinate3d_t* _bbCentral_ptr);
        void print_shm_norm_landmark3d(void);
        void print_shm_bbCentral(void);
        void print_shm(const char *_val_shm_name);

        char landmark_shm_name_[10];
        char bbCentral_shm_name_[10];
        char shm_name_[10];
    };

    class gesture{
    public:
        gesture(const char _landmark_shm_name[], const char _bbCentral_shm_name[], const char _shm_name[]);
        ~gesture();
        void store_gestures3d(void);
        void store_gestures3d(int _gesture_num);
        void load_resize_rotate_gestures3d(void);
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
        // angle of vector x = a-b and vector y = c-b
        // angle = acos( (x dot y)/(|x|*|y|) )
        float cosine_angle(landmarks_datatype::coordinate3d_t a, landmarks_datatype::coordinate3d_t b, landmarks_datatype::coordinate3d_t c);
        float angle_similarity(void);

        void init_angle(landmarks_datatype::coordinate3d_t* _landmark3d);
        void init_crossVector(landmarks_datatype::coordinate3d_t* _landmark3d);
        void init_cmp_angle_joints(void);
        void delete_cmp_angle_joints(void);
        void init_gesture_path(void);
        void init_image_size(void);

        // Wraps around an angle in radians to within -M_PI and M_PI.
        inline float NormalizeRadians(float angle) {
            return angle - 2 * M_PI * std::floor((angle - (-M_PI)) / (2 * M_PI));
        }

        char landmark_shm_name_[10];
        char bbCentral_shm_name_[10];
        char shm_name_[10];

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
        const float similarity_distance_lowbound_ = -170.f;
        const int start_crossVector_index_ = 13;
        const int end_crossVector_index_ = 5;
        const float similarity_angle_upbound_ = 2.0f;
        const int cmp_angle_joints_num_ = 15;
        int *cmp_angle_joints_;
        landmarks_datatype::coordinate3d_t image_size_;
        char *gesture_path_;
    };
}

#endif