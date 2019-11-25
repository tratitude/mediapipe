#include <iostream>
#include <fstream>
#include <string>
#include "../mediapipe/landmarks_to_shm/landmarks_to_shm.h" 

landmarks_to_shm::gesture ges;

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

void print_gesture_shm()
{
    std::string outputpath = "./out.gestureLog";
    std::ofstream log_file(outputpath);
    if(!log_file){
        std::perror("output open");
        exit(EXIT_FAILURE);
    }

    //Open the managed segment
    boost::interprocess::managed_shared_memory segment(
        boost::interprocess::open_or_create, 
        landmarks_datatype::shm_name, 
        landmarks_datatype::shm_size);

    //Find the vector using the c-string name
    landmarks_datatype::coordinate3d_t *norm_landmark3d_ptr = segment.find<landmarks_datatype::coordinate3d_t>(
        landmarks_datatype::norm_landmark_name).first;
    ges.resize(norm_landmark3d_ptr);
    ges.norm_root(norm_landmark3d_ptr);
#ifndef THREE_D
    ges.rotate2d_y(norm_landmark3d_ptr);
#else
    ges.init_crossVector(norm_landmark3d_ptr);
    ges.Rodrigues_z(norm_landmark3d_ptr);
    ges.rotate2d_y(norm_landmark3d_ptr);
    //ges.rotate3d_yz(norm_landmark3d_ptr);
    //ges.rotate3d_z(norm_landmark3d_ptr);
#endif
    for(int j=0; j<landmarks_datatype::norm_landmark_size; j++){
        log_file << norm_landmark3d_ptr[j].x << " " << -norm_landmark3d_ptr[j].y << "\n";
    }
    log_file.close();
}

int main()
{
// print gesture picture
    //print_gesture();

// print gesture picture from shm
/*
    while(true){
        char c; std::cin >> c;
        print_gesture_shm();
    }
*/
// define gesture
    //ges.store_gestures3d(".");

// calculate defined gesture similarity
    ges.load_resize_rotate_gestures3d(".");
    ges.gesture_similarity_test();

// test shm
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