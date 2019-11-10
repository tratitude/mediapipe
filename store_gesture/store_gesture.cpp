#include <iostream>
#include <fstream>
#include <string>
#include "../mediapipe/landmarks_to_shm/landmarks_to_shm.h" 

void print_gesture()
{
    for(int i=0; i<10; i++){
        std::string inputpath = "./"+std::to_string(i)+".gesture";
        std::ifstream gesture_file(inputpath);
        if(!gesture_file){
            std::perror("input open");
            exit(EXIT_FAILURE);
        }

        std::string outputpath = "./"+std::to_string(i)+".gestureLog";
        std::ofstream log_file(outputpath);
        if(!log_file){
            std::perror("output open");
            exit(EXIT_FAILURE);
        }

        std::string s;
        for(int j=0; j<landmarks_datatype::norm_landmark_size; j++){
            std::getline(gesture_file, s);
            std::istringstream ss(s);
            int index;
            float x, y, z;

            ss >> index >> x >> y >> z;
            log_file << x*256 << " " << y*(-256) << "\n";
        }
        std::getline(gesture_file, s);
    }
}

int main()
{
    print_gesture();
    //landmarks_to_shm::gesture ges;
    //ges.store_gestures3d(".");

/*
    //Open the managed segment
    boost::interprocess::managed_shared_memory segment(
        boost::interprocess::open_or_create, 
        landmarks_datatype::shm_name, 
        landmarks_datatype::shm_size);

    //Find the vector using the c-string name
    landmarks_datatype::coordinate3d_t *norm_landmark3d_ptr = segment.find<landmarks_datatype::coordinate3d_t>(
        landmarks_datatype::norm_landmark_name).first;

    ges.resize(norm_landmark3d_ptr);
    ges.rotate(norm_landmark3d_ptr);

    //std::puts("after");
    for(int i=0; i<21; i++){
        std::cout << norm_landmark3d_ptr[i].x << " " << norm_landmark3d_ptr[i].y * (-1) << "\n";
    }
*/
    return 0;
}