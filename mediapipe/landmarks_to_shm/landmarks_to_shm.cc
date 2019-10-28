#include <iostream>
#include "landmarks_to_shm.h"

//using namespace boost::interprocess;

void landmarks_to_shm::landmarks::print_norm_landmarks(void)
{
#ifdef PRINT_LANDMARKS
    std::cout << "print_norm_landmarks\n";
    int num = 0;
    for(const auto & landmark : norm_landmarks){
        std::cout << num++ << ": x: " << landmark.x() << " y: " << landmark.y()
            << " z: " << landmark.z()<< std::endl;
    }
#endif
}

void landmarks_to_shm::landmarks::print_landmarks(void)
{
#ifdef PRINT_LANDMARKS
    std::cout << "print_landmarks\n";
    int num = 0;
    for(const auto & landmark : landmarks){
        std::cout << num++ << ": x: " << landmark.x() << " y: " << landmark.y()
            << " z: " << landmark.z() << std::endl;
    }
#endif
}

void landmarks_to_shm::landmarks::print_letterbox_padding(void)
{
#ifdef PRINT_LETTERBOX
    std::cout << "print_letterbox_padding\n";
    std::cout << "left: " << letterbox_padding[0] << 
        " top: " << letterbox_padding[1] << 
        " right: " << letterbox_padding[2] << 
        " bottom: " << letterbox_padding[3] << std::endl;
#endif
}

landmarks_to_shm::shm::shm(void)
{
    using namespace boost::interprocess;

    shared_memory_object::remove("NormLandmarks");

    //Create a shared memory object.
    shared_memory_object shmobj (create_only, "NormLandmarks", read_write);

    //Set size
    shmobj.truncate(63*sizeof(float));

    //Map the whole shared memory in this process
    mapped_region region(shmobj, read_write, 0, 63);

    //Write all the memory to 1
    std::memset(region.get_address(), 0, region.get_size());
}

landmarks_to_shm::shm::~shm()
{
    using namespace boost::interprocess;

    shared_memory_object::remove("NormLandmarks");
}

void landmarks_to_shm::shm::save_norm_landmarks(
    const std::vector<mediapipe::NormalizedLandmark>& norm_landmarks)
{
    using namespace boost::interprocess;

    //Create a shared memory object.
    shared_memory_object shmobj (open_only, "NormLandmarks", read_write);

    //Map the whole shared memory in this process
    mapped_region region(shmobj, read_write, 0, 63);

    float *mem = (float*)region.get_address();
    int num = 0;
    for(const auto & landmark : norm_landmarks){
        mem[num++] = landmark.x();
        mem[num++] = landmark.y();
        mem[num++] = landmark.z();
    }
}

void landmarks_to_shm::shm::get_norm_landmarks(float norm_landmarks[21][3])
{
    using namespace boost::interprocess;

    //Create a shared memory object.
    shared_memory_object shmobj (open_only, "NormLandmarks", read_only);

    //Map the whole shared memory in this process
    mapped_region region(shmobj, read_only, 0, 63);

    float *mem = (float*)region.get_address();

    for(int i=0; i<21; i++){
        for(int j=0; j<3; j++){
            norm_landmarks[i][j] = mem[i*3 + j];
        }
    }
}

void landmarks_to_shm::shm::print_shm_norm_landmarks(void)
{
#ifdef PRINT_SHM_LANDMARKS
    using namespace boost::interprocess;

    //Create a shared memory object.
    shared_memory_object shmobj (open_only, "NormLandmarks", read_only);

    //Map the whole shared memory in this process
    mapped_region region(shmobj, read_only, 0, 63);

    float *mem = (float*)region.get_address();
    for(int i=0; i<21; i++){
        std::cout << i << ": x: " << mem[i] << 
            " y: " << mem[i+1] << 
            " z: " << mem[i+2] << std::endl;
    }
#endif
}