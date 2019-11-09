#include "landmarks_datatype.h"

namespace landmarks_datatype{
    const int norm_landmark_size = 21;
    const int norm_landmark_shm_size = 1024;  // need 4*3*21=252 bytes
    const char *norm_landmark_name = "NormLandArr";
    const char *shm_name = "NormLandmarks";
    const char *bbCentral_name = "BoundingBoxCentral";
    const int bbCentral_shm_size = 16;  // need 4*3=12 bytes
    const coordinate3d_t image_size = {256, 256};
}