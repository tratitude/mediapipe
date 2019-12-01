#include <iostream>
#include <fstream>
#include <string>
#include "../mediapipe/landmarks_to_shm/landmarks_to_shm.h"
#include <cstdlib>

landmarks_to_shm::gesture ges(
    landmarks_datatype::norm_landmark_name, 
    landmarks_datatype::bbCentral_name, 
    landmarks_datatype::shm_name);

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
        boost::interprocess::open_or_create, ges.shm_name_, 
        landmarks_datatype::shm_size);

    //Find the vector using the c-string name
    landmarks_datatype::coordinate3d_t *norm_landmark3d_ptr = segment.find<landmarks_datatype::coordinate3d_t>(
        ges.landmark_shm_name_).first;
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
    char menu_select;
    std::puts("p: print gesture picture");
    std::puts("s: print gesture picture from shm");
    std::puts("d: define gesture");
    std::puts("c: calculate defined gesture similarity");
    std::puts("q: quit");
    while(std::cin >> menu_select){
        switch(menu_select){
            case 'p':
                print_gesture();
                break;
            case 's':
                print_gesture_shm();
                break;
            case 'd':
                ges.store_gestures3d();
                break;
            case 'c':
                ges.load_resize_rotate_gestures3d();
                ges.gesture_similarity_test();
                break;
            case 'q':
                exit(EXIT_SUCCESS);
                break;
        }
    }
    return 0;
}