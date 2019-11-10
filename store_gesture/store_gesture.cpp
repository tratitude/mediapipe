#include <iostream>
#include "../mediapipe/landmarks_to_shm/landmarks_to_shm.h" 

int main()
{
    landmarks_to_shm::gesture ges;
    ges.store_gesture(".");
    
    return 0;
}