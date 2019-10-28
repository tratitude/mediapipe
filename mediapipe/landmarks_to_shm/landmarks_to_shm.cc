#include <iostream>
#include "landmarks_to_shm.h"

void landmarks_to_shm::landmarks::print_norm_landmarks(void)
{
    int num = 0;
    for(const auto & landmark : norm_landmarks){
        std::cout << ++num << ": x: " << landmark.x() << " y: " << landmark.y()
            << " z: " << landmark.z()<< std::endl;
    }
}

void landmarks_to_shm::landmarks::print_landmarks(void)
{
    int num = 0;
    for(const auto & landmark : landmarks){
        std::cout << ++num << ": x: " << landmark.x() << " y: " << landmark.y()
            << " z: " << landmark.z() << std::endl;
    }
}

void landmarks_to_shm::landmarks::print_letterbox_padding(void)
{
    std::cout << "left: " << letterbox_padding[0] << 
        " top: " << letterbox_padding[1] << 
        " right: " << letterbox_padding[2] << 
        " bottom: " << letterbox_padding[3] << std::endl;
}