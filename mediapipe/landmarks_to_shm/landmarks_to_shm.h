#ifndef LANDMARKS_TO_SHM
#define LANDMARKS_TO_SHM

#include "mediapipe/framework/formats/landmark.pb.h"
#include "mediapipe/framework/calculator_framework.h"
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>

//#define PRINT_LANDMARKS
//#define PRINT_LETTERBOX
#define PRINT_SHM_LANDMARKS

namespace landmarks_to_shm{
    class shm{
    public:
        shm(void);
        ~shm();
        void save_norm_landmarks(const std::vector<mediapipe::NormalizedLandmark>& norm_landmarks);
        void get_norm_landmarks(float norm_landmarks[21][3]);
        void print_shm_norm_landmarks(void);
    };

    class landmarks : public shm{
    public:
        void print_norm_landmarks(void);
        void print_landmarks(void);
        void print_letterbox_padding(void);

        std::vector<mediapipe::NormalizedLandmark> norm_landmarks;
        std::vector<mediapipe::Landmark> landmarks;
        std::array<float, 4> letterbox_padding;
    };
}

#endif