#ifndef LANDMARKS_TO_SHM
#define LANDMARKS_TO_SHM

#include "mediapipe/framework/formats/landmark.pb.h"
#include "mediapipe/framework/calculator_framework.h"

namespace landmarks_to_shm{
    class landmarks{
    public:
        void print_landmarks();
        absl::make_unique<std::vector<NormalizedLandmark>>() norm_landmarks;
    };
}

#endif