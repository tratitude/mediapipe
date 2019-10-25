#ifndef LANDMARKS_TO_SHM
#define LANDMARKS_TO_SHM

#include "mediapipe/framework/formats/landmark.pb.h"
#include "mediapipe/framework/calculator_framework.h"

namespace landmarks_to_shm{
    class landmarks{
    public:
        void print_norm_landmarks(void);
        void print_landmarks(void);
        std::vector<mediapipe::NormalizedLandmark> norm_landmarks;
        std::vector<mediapipe::Landmark> landmarks;
    };
}

#endif