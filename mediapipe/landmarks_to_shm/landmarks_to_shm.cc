#include <iostream>
#include <unistd.h>
#include <string>
#include <fstream>
#include <sstream>
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
    gesture_num = 6; //init_gesture_num();
    gestures = new landmarks_datatype::gesture_t [gesture_num];
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
    init_gesture_num();
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

void landmarks_to_shm::gesture::load_gesture(std::string dir)
{
    for(int i=0; i<gesture_num; i++){
        dir += std::to_string(i)+".gesture";
    #ifdef PRINT_DEBUG
        std::cout << dir << "\n";
    #endif
        std::ifstream gesture_file(dir);
        if(!gesture_file){
            std::perror("load_gesture");
            exit(EXIT_FAILURE);
        }
        std::string s;

        gestures[i].co = new landmarks_datatype::coordinate3d_t [landmarks_datatype::norm_landmark_size];

        for(int j=0; j<landmarks_datatype::norm_landmark_size; j++){
            std::getline(gesture_file, s);
            std::istringstream ss(s);
            int index;

            ss >> index >> gestures[i].co[j].x >> gestures[i].co[j].y >> gestures[i].co[j].z;
        }
        std::getline(gesture_file, s);
        gestures[i].name = s;

        for(int j=0; j<landmarks_datatype::norm_landmark_size; j++){
            gestures[i].co[j] = gestures[i].co[j] * (image_size);
        }
        rotate2d(gestures[i].co);

        gesture_file.close();
    }
}

void landmarks_to_shm::gesture::init_gesture_num(void)
{

}

void landmarks_to_shm::gesture::print_gestures(void)
{
    for(int i=0; i<gesture_num; i++){
        std::cout << i << " : " << gestures[i].name << "\n";
        for(int j=0; j<landmarks_datatype::norm_landmark_size; j++){
            std::cout << gestures[i].co[j].x << " " << gestures[i].co[j].y <<
                " " << gestures[i].co[j].z << "\n";
        }
    }
}

void landmarks_to_shm::gesture::similarity(void)
{
    //Open the managed segment
    boost::interprocess::managed_shared_memory segment(
        boost::interprocess::open_copy_on_write, 
        landmarks_datatype::shm_name);

    //Find the vector using the c-string name
    landmarks_datatype::coordinate3d_t *normLand3d = 
        segment.find<landmarks_datatype::coordinate3d_t>(
        landmarks_datatype::norm_landmark_name).first;

}

void landmarks_to_shm::gesture::similarity(landmarks_datatype::coordinate3d_t* norm_landmark)
{
    for(int i=0; i<landmarks_datatype::norm_landmark_size; i++){
        norm_landmark[i] = norm_landmark[i] * image_size;
    }
#ifdef PRINT_DEBUG
    std::puts("similarity, after resized to full image: norm_landmark");
    for(int i=0; i<21; i++){
        std::cout << i << " " <<  norm_landmark[i].x << " " << 
            norm_landmark[i].y << " " << norm_landmark[i].z << "\n";
    }
#endif

    rotate2d(norm_landmark);
#ifdef PRINT_DEBUG
    std::puts("similarity, after rotate2d: norm_landmark");
    for(int i=0; i<21; i++){
        std::cout << i << " " <<  norm_landmark[i].x << " " << 
            norm_landmark[i].y << " " << norm_landmark[i].z << "\n";
    }
#endif
    // shm
    uint32_t match_gesture = 0;

    int max_gesture = 0;
    float max_similarity = 0.f;

    for(int i=0; i<gesture_num; i++){
        float avg_sim = 0.f;
        for(int j=1; j<landmarks_datatype::norm_landmark_size; j++){
            float inner = (gestures[i].co[j] * norm_landmark[j]).distance();
            float product = gestures[i].co[j].distance()*norm_landmark[j].distance();
            avg_sim +=  inner / product;
        /*
        #ifdef PRINT_DEBUG
            std::puts("similarity: inner, product");
            std::cout << inner << " " << product << "\n";
        #endif
        */
        }
    /*
    #ifdef PRINT_DEBUG
        std::puts("similarity: avg_sim");
        std::cout << avg_sim << "\n";
    #endif
    */
        if(avg_sim > max_similarity){
            max_similarity = avg_sim;
            max_gesture = i;
        }
    }
    if(max_similarity < similarity_lowbound * landmarks_datatype::norm_landmark_size){
        match_gesture = 0;
    }
    else{
        match_gesture = 1 << max_gesture;
    }
#ifdef PRINT_DEBUG
    std::puts("similarity: match_gesture");
    std::cout << match_gesture << "\n";
    std::puts("similarity: average similarity");
    std::cout << max_similarity/21 << "\n";
#endif
}

void landmarks_to_shm::gesture::rotate2d(
    landmarks_datatype::coordinate3d_t* norm_landmark)
{
    const landmarks_datatype::coordinate3d_t p0 = norm_landmark[start_keypoint_index_];
    const landmarks_datatype::coordinate3d_t p1 = norm_landmark[end_keypoint_index_];
    float rotation_angle = target_angle_ - std::atan2(-(p1.y - p0.y), p1.x - p0.x);
    rotation_angle = NormalizeRadians(rotation_angle);

    const landmarks_datatype::coordinate3d_t root = norm_landmark[start_keypoint_index_];
    for(int i=0; i<landmarks_datatype::norm_landmark_size; i++){
        if(!i){
            norm_landmark[i] = {0.f, 0.f};
        }
        else{
            const float cosine = cosf(rotation_angle);
            const float sine = sinf(rotation_angle);
            const landmarks_datatype::coordinate3d_t vec = norm_landmark[i] - root;
            
            // rotate clockwise
            norm_landmark[i] = {cosine*vec.x - sine*vec.y, cosine*vec.y + sine*vec.x};
        }
    }
}
