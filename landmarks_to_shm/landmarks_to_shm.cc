#include "landmarks_to_shm.h"

landmarks_to_shm::landmarks::print_landmarks()
{
    for(const auto & landmark : norm_landmarks){
        cout << "x: " << landmark.x() << " y: " << landmark.y()
            << " z: " << landmark.z()<< endl;
    }
}