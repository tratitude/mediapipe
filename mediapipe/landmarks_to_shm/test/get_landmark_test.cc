#include <iostream>

#include "landmarks_to_shm.h"

int main()
{
    float landmarks[21][3];
    landmarks_to_shm::shm shm;
    while(1){
        shm.get_norm_landmarks(landmarks);
        for(int i=0; i<21; i++){
            std::cout << i << ": x: " << landmarks[i][0] << 
            " y: " << landmarks[i][1] << 
            " z: " << landmarks[i][2] << std::endl;
        }
    }
}