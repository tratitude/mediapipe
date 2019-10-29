#include <iostream>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>

//#include "../landmarks_to_shm.h"

void print_shm_norm_landmarks(void)
{
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
}

int main()
{
    print_shm_norm_landmarks();

    return 0;
}