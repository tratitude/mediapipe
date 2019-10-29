#ifndef LANDMARKS_TO_SHM
#define LANDMARKS_TO_SHM

#include "mediapipe/framework/formats/landmark.pb.h"
#include "mediapipe/framework/calculator_framework.h"
//#include <boost/interprocess/shared_memory_object.hpp>
//#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/allocators/allocator.hpp>

//#define PRINT_LANDMARKS
//#define PRINT_LETTERBOX
#define PRINT_SHM_LANDMARKS
#define PRINT_DEBUG

namespace landmarks_to_shm{
    //Define an STL compatible allocator of ints that allocates from the managed_shared_memory.
    //This allocator will allow placing containers in the segment
    typedef boost::interprocess::allocator<mediapipe::NormalizedLandmark, boost::interprocess::managed_shared_memory::segment_manager>  ShmemAllocator;

    //Alias a vector that uses the previous STL-like allocator so that allocates
    //its values from the segment
    typedef boost::interprocess::vector<mediapipe::NormalizedLandmark, ShmemAllocator> NormLandVector;

    class shm{
    public:
        shm(void);
        ~shm();
        //void save_norm_landmarks(const std::vector<mediapipe::NormalizedLandmark>& norm_landmarks);
        //void get_norm_landmarks(float norm_landmarks[21][3]);
        void get_normLandVector(NormLandVector**);
        void print_shm_norm_landmarks(void);

        NormLandVector *norm_landmarks;
        //std::vector<mediapipe::Landmark> *landmarks;
        std::array<float, 4> *letterbox_padding;
    };

    class landmarks : public shm{
    public:
        void print_norm_landmarks(void);
        void print_landmarks(void);
        void print_letterbox_padding(void);
    };
}

#endif