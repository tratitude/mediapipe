#include <iostream>
#include <unistd.h>
#include <string>
#include <fstream>
#include "landmarks_to_shm.h"

landmarks_to_shm::shm::shm(void)
{
    boost::interprocess::shared_memory_object::remove(
        landmarks_datatype::shm_name);

    //Create a new segment with given name and size
    boost::interprocess::managed_shared_memory segment(
        boost::interprocess::create_only, 
        landmarks_datatype::shm_name, 
        landmarks_datatype::norm_landmark_shm_size);

    //Construct a vector named "coordinate3d_t" in shared memory with argument alloc_inst
    landmarks_datatype::coordinate3d_t *normLand3d = 
        segment.construct<landmarks_datatype::coordinate3d_t>(
        landmarks_datatype::norm_landmark_name)
        [landmarks_datatype::norm_landmark_size]();

#ifdef PRINT_DEBUG
    std::puts("In shm");
    //std::printf("normLand3d adress: %p\n", normLand3d);
#endif
}

landmarks_to_shm::shm::~shm()
{
    //Open the managed segment
    boost::interprocess::managed_shared_memory segment(
        boost::interprocess::open_only, 
        landmarks_datatype::shm_name);

    //Find the vector using the c-string name
    landmarks_datatype::coordinate3d_t *arr = 
        segment.find<landmarks_datatype::coordinate3d_t>(
        landmarks_datatype::norm_landmark_name).first;

    //When done, destroy the vector from the segment
    segment.destroy<landmarks_datatype::coordinate3d_t>(
        landmarks_datatype::norm_landmark_name);
    
    boost::interprocess::shared_memory_object::remove(
        landmarks_datatype::shm_name);

#ifdef PRINT_DEBUG
    std::puts("In ~shm");
#endif
}

void landmarks_to_shm::shm::print_shm_norm_landmarks(void)
{
#ifdef PRINT_SHM_LANDMARKS
    //Open the managed segment
    boost::interprocess::managed_shared_memory segment(
      boost::interprocess::open_copy_on_write, 
      landmarks_datatype::shm_name);

    //Find the vector using the c-string name
    landmarks_datatype::coordinate3d_t *normLand3d = 
        segment.find<landmarks_datatype::coordinate3d_t>(
      landmarks_datatype::norm_landmark_name).first;

    int normLand3d_size = 
        segment.find<landmarks_datatype::coordinate3d_t>(
        landmarks_datatype::norm_landmark_name).second;

    std::puts("In print_shm_norm_landmarks");
    std::printf("normLand3d size: %d adress: %p\n", 
        normLand3d_size, normLand3d);

    for(int i=0; i<landmarks_datatype::norm_landmark_size; i++){
          std::printf("normLand3d: %d = (%f, %f, %f)\n",
           i, normLand3d[i].x, normLand3d[i].y, normLand3d[i].z);
    }
#endif
}

void landmarks_to_shm::shm::get_normLandVector(
    landmarks_datatype::coordinate3d_t **normLand3d)
{
    //Open the managed segment
  boost::interprocess::managed_shared_memory segment(
    boost::interprocess::open_or_create, 
    landmarks_datatype::shm_name, 
    landmarks_datatype::norm_landmark_shm_size);

    //Find the vector using the c-string name
    *normLand3d = segment.find<landmarks_datatype::coordinate3d_t>(
        landmarks_datatype::norm_landmark_name).first;

#ifdef PRINT_DEBUG
    std::puts("In get_normLandVector");
    std::printf("normLand3d adress: %p\n", *normLand3d);
#endif
}

//****************************************************

landmarks_to_shm::gesture::gesture(void)
{
}

landmarks_to_shm::gesture::~gesture()
{

}

void landmarks_to_shm::gesture::store_gesture(void)
{
    std::puts("Press 'd' to define gesture, press'c' to change specific gesture, press 'q' to quit");
    for(int i=0; i<gesture_max_num; i++){
        
        char input_signal;
        std::cin >> input_signal;
        
        if(input_signal == 'd'){
            store_gesture(i);
        }
        else if(input_signal == 'c'){
            int gesture_num; std::cin >> gesture_num;
            store_gesture(gesture_num);
        }
        else if(input_signal == 'q'){
            break;
        }
    }
    //sleep(10);
}

void landmarks_to_shm::gesture::store_gesture(int gesture_num)
{
    std::printf("num: %d gesture\n", gesture_num);
    std::ofstream gesture_file(std::to_string(gesture_num)+".gesture");

    //Open the managed segment
    boost::interprocess::managed_shared_memory segment(
        boost::interprocess::open_copy_on_write, 
        landmarks_datatype::shm_name);

    //Find the vector using the c-string name
    landmarks_datatype::coordinate3d_t *normLand3d = 
        segment.find<landmarks_datatype::coordinate3d_t>(
        landmarks_datatype::norm_landmark_name).first;

    int normLand3d_size = segment.find<landmarks_datatype::coordinate3d_t>(
        landmarks_datatype::norm_landmark_name).second;
    
    for(int i=0; i<normLand3d_size; i++){
        gesture_file << i << " " << normLand3d[i].x << " "
        << normLand3d[i].y << " " << normLand3d[i].z << "\n";

        std::cout << i << " " << normLand3d[i].x << " "
        << normLand3d[i].y << " " << normLand3d[i].z << "\n";
    }

    std::puts("input gesture name");
    std::puts("(gesture name)_(right or left hand)_(back or front hand)_(size)_(position)_(angle)");

    std::string gesture_name;
    std::cin >> gesture_name;

    gesture_file << gesture_name << "\n";

    gesture_file.close();
}