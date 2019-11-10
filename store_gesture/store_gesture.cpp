#include <iostream>
#include "../mediapipe/landmarks_to_shm/landmarks_to_shm.h" 

int main()
{
    landmarks_to_shm::gesture ges;
    //ges.store_gestures3d(".");
    
    //Open the managed segment
    boost::interprocess::managed_shared_memory segment(
        boost::interprocess::open_or_create, 
        landmarks_datatype::shm_name, 
        landmarks_datatype::shm_size);

    //Find the vector using the c-string name
    landmarks_datatype::coordinate3d_t *norm_landmark3d_ptr = segment.find<landmarks_datatype::coordinate3d_t>(
        landmarks_datatype::norm_landmark_name).first;

    ges.resize(norm_landmark3d_ptr);
/*
    //std::puts("before");
    for(int i=0; i<21; i++){
        std::cout << norm_landmark3d_ptr[i].x << " " << norm_landmark3d_ptr[i].y * (-1) << "\n";
    }
*/

    ges.rotate(norm_landmark3d_ptr);

    //std::puts("after");
    for(int i=0; i<21; i++){
        std::cout << norm_landmark3d_ptr[i].x << " " << norm_landmark3d_ptr[i].y * (-1) << "\n";
    }

    return 0;
}