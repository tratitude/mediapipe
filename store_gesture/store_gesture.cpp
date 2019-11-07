#include <iostream>
#include "../mediapipe/landmarks_to_shm/landmarks_to_shm.h"

int main()
{
    /*
    landmarks_to_shm::gesture ges;
    ges.store_gesture();
    */

    landmarks_datatype::normLand3d_t p1, p2(1.0, 2.0, 3.0);
    std::cout << p2.distance() << std::endl;

    return 0;
}