#include "landmarks_datatype.h"

namespace landmarks_datatype{
    const int norm_landmark_size = 21;
    // norm_landmark 4*3*21=252 bytes, bbCentral 4*3=12 bytes
    const int shm_size = 1024;
    const char *norm_landmark_name = "NormLandArr";
    const char *shm_name = "NormLandmarks";
    const char *bbCentral_name = "BoundingBoxCentral";
    const coordinate3d_t image_size = {256, 256, 1};
    const char *gesture_path = "/home/csieert/code/mediapipe/store_gesture";
}