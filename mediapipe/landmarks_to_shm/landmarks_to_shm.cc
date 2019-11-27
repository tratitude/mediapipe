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
        landmarks_datatype::shm_size);
// normLand3d
    //Construct an array named norm_landmark_name in shared memory
    landmarks_datatype::coordinate3d_t *normLand3d = 
        segment.construct<landmarks_datatype::coordinate3d_t>(
        landmarks_datatype::norm_landmark_name)
        [landmarks_datatype::norm_landmark_size]();

// bbCentral
// x=x_center, y=y_center, z=match_gesture
    landmarks_datatype::coordinate3d_t *bbCentral = 
        segment.construct<landmarks_datatype::coordinate3d_t>(
        landmarks_datatype::bbCentral_name)[1]();

#ifdef PRINT_DEBUG
    std::puts("In shm");
#endif
}

landmarks_to_shm::shm::~shm()
{
/*
    boost::interprocess::shared_memory_object::remove(
        landmarks_datatype::shm_name);
*/
    //Open the managed segment
    boost::interprocess::managed_shared_memory segment(
        boost::interprocess::open_only, 
        landmarks_datatype::shm_name);

    //When done, destroy norm_landmark from the segment
    segment.destroy<landmarks_datatype::coordinate3d_t>(
        landmarks_datatype::norm_landmark_name);
    
    //When done, destroy norm_landmark from the segment
    segment.destroy<landmarks_datatype::coordinate3d_t>(
        landmarks_datatype::bbCentral_name);

#ifdef PRINT_DEBUG
    std::puts("In ~shm");
#endif
}

void landmarks_to_shm::shm::print_shm(const char *_val_shm_name)
{
    //Open the managed segment
    boost::interprocess::managed_shared_memory segment(
      boost::interprocess::open_copy_on_write, 
      landmarks_datatype::shm_name);

    //Find the vector using the c-string name
    landmarks_datatype::coordinate3d_t *val = 
        segment.find<landmarks_datatype::coordinate3d_t>(
        _val_shm_name).first;

    int val_size = 
        segment.find<landmarks_datatype::coordinate3d_t>(
        _val_shm_name).second;
#ifdef PRINT_DEBUG
    std::puts("print_shm: _val_shm_name, val_size, address");
    std::printf("%s %d %p\n", _val_shm_name, val_size, val);
#endif
    std::puts("print_shm: val");
    for(int i=0; i<val_size; i++){
          std::printf("%s: %d = (%f, %f, %f)\n",
           _val_shm_name, i, val[i].x, val[i].y, val[i].z);
    }
}

void landmarks_to_shm::shm::print_shm_norm_landmark3d(void)
{
    print_shm(landmarks_datatype::norm_landmark_name);
}

void landmarks_to_shm::shm::print_shm_bbCentral(void)
{
    print_shm(landmarks_datatype::bbCentral_name);
}

void landmarks_to_shm::shm::get_norm_landmark3d_ptr(
    landmarks_datatype::coordinate3d_t* _norm_landmark3d_ptr)
{
    //Open the managed segment
    boost::interprocess::managed_shared_memory segment(
        boost::interprocess::open_or_create, 
        landmarks_datatype::shm_name, 
        landmarks_datatype::shm_size);

    //Find the vector using the c-string name
    _norm_landmark3d_ptr = segment.find<landmarks_datatype::coordinate3d_t>(
        landmarks_datatype::norm_landmark_name).first;

#ifdef PRINT_DEBUG
    std::puts("In get_norm_landmark3d_ptr");
    std::printf("_norm_landmark3d_ptr adress: %p\n", _norm_landmark3d_ptr);
#endif
}

void landmarks_to_shm::shm::get_bbCentral_ptr(landmarks_datatype::coordinate3d_t* _bbCentral_ptr)
{
    //Open the managed segment
    boost::interprocess::managed_shared_memory segment(
        boost::interprocess::open_or_create, 
        landmarks_datatype::shm_name, 
        landmarks_datatype::shm_size);

    //Find the vector using the c-string name
    _bbCentral_ptr = segment.find<landmarks_datatype::coordinate3d_t>(
        landmarks_datatype::bbCentral_name).first;

#ifdef PRINT_DEBUG
    std::puts("In get_bbCentral_ptr");
    std::printf("_bbCentral_ptr adress: %p\n", _bbCentral_ptr);
#endif
}

//****************************************************

landmarks_to_shm::gesture::gesture(void)
{
    init_gestures3d();
    init_norm_landmark3d();
    init_cmp_angle_joints();
}

landmarks_to_shm::gesture::~gesture()
{
    delete_gestures3d();
    delete_norm_landmark3d();
    delete_cmp_angle_joints();
}

void landmarks_to_shm::gesture::store_gestures3d(const std::string &_dir)
{
    for(int i=0; i<gesture_max_num_; i++){
        std::puts("Usage: [OPTION]");
        std::puts("d             : redefine all gesture from 0~20");
        std::puts("c gesture_num : change specific gesture by gesture_num");
        std::puts("q             : quit");

        char input_signal;
        std::cin >> input_signal;
        
        if(input_signal == 'd'){
            store_gestures3d(i, _dir);
        }
        else if(input_signal == 'c'){
            int gesture_num; std::cin >> gesture_num;
            store_gestures3d(gesture_num, _dir);
        }
        else if(input_signal == 'q'){
            break;
        }
    }
}

void landmarks_to_shm::gesture::store_gestures3d(int _gesture_num, const std::string &_dir)
{
    std::printf("num: %d gesture\n", _gesture_num);

    const std::string path = _dir + "/" + std::to_string(_gesture_num)+".gesture";
    #ifdef PRINT_DEBUG
        std::puts("load_gesture: dir");
        std::cout << path;
    #endif

    std::ofstream gesture_file(path);
    if(!gesture_file){
        std::perror("store_gesture");
        exit(EXIT_FAILURE);
    }

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
        gesture_file << i << " " << normLand3d[i];

    #ifdef PRINT_DEBUG
        std::cout << i << " " << normLand3d[i];
    #endif
    }

    std::puts("input gesture name");
    std::puts("(gesture name)_(right or left hand)_(back or front hand)_(size)_(position)_(angle)");

    std::string gesture_name;
    std::cin >> gesture_name;

    gesture_file << gesture_name << "\n";

    gesture_file.close();

    //init_gesture_num();
    //load_resize_rotate_gestures3d(_dir);
}

void landmarks_to_shm::gesture::load_resize_rotate_gestures3d(const std::string &_dir)
{
    for(int i=0; i<gesture_num_; i++){
        const std::string path = _dir + "/" + std::to_string(i)+".gesture";
    #ifdef PRINT_DEBUG
        std::puts("load_gesture: dir");
        std::cout << path << "\n";
    #endif

        std::ifstream gesture_file(path);
        if(!gesture_file){
            std::perror("load_gesture");
            exit(EXIT_FAILURE);
        }

        std::string s;
        for(int j=0; j<landmarks_datatype::norm_landmark_size; j++){
            std::getline(gesture_file, s);
            std::istringstream ss(s);
            int index;

            ss >> index >> gestures3d_[i].co[j].x >> gestures3d_[i].co[j].y >> gestures3d_[i].co[j].z;
        }
        std::getline(gesture_file, s);
        gestures3d_[i].name = s;

        resize_rotate3d(gestures3d_[i].co);
        //init_angle3d(gestures3d_[i].co);
        
        gesture_file.close();
    }
}

void landmarks_to_shm::gesture::resize_rotate3d(landmarks_datatype::coordinate3d_t* _landmark3d)
{
    resize(_landmark3d);
    norm_root(_landmark3d);
    init_angle3d(_landmark3d);
#ifdef THREE_D
    // 3d
    init_crossVector(_landmark3d);
    Rodrigues_z(_landmark3d);
#endif
    rotate2d_y(_landmark3d);
}

void landmarks_to_shm::gesture::init_gesture_num(void)
{
    gesture_num_ = 10;
}

void landmarks_to_shm::gesture::print_gestures3d(void)
{
    for(int i=0; i<gesture_num_; i++){
        std::cout << i << " : " << gestures3d_[i].name << "\n";
        for(int j=0; j<landmarks_datatype::norm_landmark_size; j++){
            std::cout << gestures3d_[i].co[j];
        }
    }
}

float landmarks_to_shm::gesture::similarity(void)
{
    int max_gesture = -1;
    float max_similarity = (-1)*landmarks_datatype::norm_landmark_size*(landmarks_datatype::image_size.x+landmarks_datatype::image_size.y);
    
    // normalize landmarks by gesture distance of root to middle MCP
    for(int gesture_cnt=0; gesture_cnt<gesture_num_; gesture_cnt++){
        float gesture_dis = gestures3d_[gesture_cnt].co[end_keypoint_index_].distance();
        float landmark_dis = norm_landmark3d_[end_keypoint_index_].distance();
        if(landmark_dis == 0.f)
            continue;
        for(int landmark_cnt=1; landmark_cnt<landmarks_datatype::norm_landmark_size; landmark_cnt++){
            norm_landmark3d_[landmark_cnt] = norm_landmark3d_[landmark_cnt] * (gesture_dis / landmark_dis);
        }
    }

    for(int gesture_cnt=0; gesture_cnt<gesture_num_; gesture_cnt++){
        float avg_sim = 0.f;
        for(int landmark_cnt=1; landmark_cnt<landmarks_datatype::norm_landmark_size; landmark_cnt++){
            float dis = (gestures3d_[gesture_cnt].co[landmark_cnt] - norm_landmark3d_[landmark_cnt]).distance();
            avg_sim -= dis;
        
        #ifdef PRINT_DEBUG
            std::puts("similarity: landmark_cnt, dis");
            std::cout << landmark_cnt << " " << dis << "\n";
        #endif
        
        }
    
    #ifdef PRINT_DEBUG
        std::puts("similarity: gesture_cnt, avg_sim");
        std::cout << gesture_cnt << " " << avg_sim << "\n";
    #endif
    
        if(avg_sim > max_similarity){
            max_similarity = avg_sim;
            max_gesture = gesture_cnt;
        }
    }
    float match_gesture = max_similarity < similarity_distance_lowbound_ ? -1 : max_gesture;

#ifdef PRINT_DEBUG
    std::puts("similarity: match_gesture, max_gesture");
    std::cout << match_gesture << " " << max_gesture << "\n";
    std::puts("similarity: max_similarity");
    std::cout << max_similarity << "\n";
#endif

    return match_gesture;
}

float landmarks_to_shm::gesture::angle_similarity(void)
{
    int max_gesture = -1;
    float max_sim = 200;

    for(int gesture_cnt=0; gesture_cnt<gesture_num_; gesture_cnt++){
        float ges_sim = 0.f;
        for(int joint_cnt=0; joint_cnt<cmp_angle_joints_num_; joint_cnt++){
            int joint = cmp_angle_joints_[joint_cnt];
            ges_sim += (gestures3d_[gesture_cnt].co[joint].angle - norm_landmark3d_[joint].angle);
        }
        if(ges_sim < max_sim){
            max_sim = ges_sim;
            max_gesture = gesture_cnt;
        }
    }
    if(max_sim > similarity_angle_upbound_){
        max_gesture = -1;
    }
    return max_gesture;
}

void landmarks_to_shm::gesture::gesture_similarity_test(void)
{
    std::puts("gesture_similarity_test");
    float min_dis = 1000.f;
    for(int gesture_cnt=0; gesture_cnt < gesture_num_; gesture_cnt++){
        float gesture_dis = gestures3d_[gesture_cnt].co[end_keypoint_index_].distance();
        landmarks_datatype::coordinate3d_t cmp[landmarks_datatype::norm_landmark_size];

        for(int inner_cnt=gesture_cnt; inner_cnt < gesture_num_; inner_cnt++){
            for(int i=0; i<landmarks_datatype::norm_landmark_size; i++){
                cmp[i] = gestures3d_[inner_cnt].co[i];
            }

            float cmp_dis = cmp[end_keypoint_index_].distance();
            for(int i=0; i<landmarks_datatype::norm_landmark_size; i++){
                cmp[i] = cmp[i] * (gesture_dis / cmp_dis);
            }
            
            float sim_dis = 0.f;
            for(int i=0; i<landmarks_datatype::norm_landmark_size; i++){
                landmarks_datatype::coordinate3d_t tmp = gestures3d_[gesture_cnt].co[i] - cmp[i];
                sim_dis += tmp.distance();
            }
            if(sim_dis != 0.f){
                min_dis = std::min(sim_dis, min_dis);
            }
            std::printf("%d %d sim_dis: %f\n", gesture_cnt, inner_cnt, sim_dis);
        }
    }
    // gesture 1, 7
    // min_dis: 123.833733
    std::printf("min_dis: %f\n", min_dis);
}

void landmarks_to_shm::gesture::rotate2d_y(
    landmarks_datatype::coordinate3d_t* _landmark3d)
{
    const landmarks_datatype::coordinate3d_t p0 = _landmark3d[start_keypoint_index_];
    const landmarks_datatype::coordinate3d_t p1 = _landmark3d[end_keypoint_index_];
    float rotation_angle = target_angle_ - std::atan2(-(p1.y - p0.y), p1.x - p0.x);
    //rotation_angle = NormalizeRadians(rotation_angle);
    const float cosine = cosf(rotation_angle);
    const float sine = sinf(rotation_angle);

    //const landmarks_datatype::coordinate3d_t root = _landmark3d[start_keypoint_index_];
    for(int i=0; i<landmarks_datatype::norm_landmark_size; i++){
        if(!i){
            _landmark3d[i] = {0.f, 0.f};
        }
        else{            
            // rotate clockwise
            _landmark3d[i] = {cosine*_landmark3d[i].x + sine*_landmark3d[i].y, cosine*_landmark3d[i].y - sine*_landmark3d[i].x};
        }
    }
}

void landmarks_to_shm::gesture::Rodrigues_z(landmarks_datatype::coordinate3d_t* _landmark3d)
{
    landmarks_datatype::coordinate3d_t v_head(crossVector.x, crossVector.y, crossVector.z+1);
    float dis = sqrt(crossVector.x*crossVector.x + crossVector.y*crossVector.y + crossVector.z*crossVector.z);
    if(dis != 0.f){
        v_head = v_head * (1 / dis);
    }
#ifdef PRINT_DEBUG
    std::puts("Rodrigues_z: v_head");
    std::cout << v_head;
#endif
    for(int i=1; i<landmarks_datatype::norm_landmark_size; i++){
        _landmark3d[i] = _landmark3d[i] * (-1) + v_head * 2 * landmarks_datatype::dot_product3d(v_head, _landmark3d[i]);
    }
#ifdef PRINT_DEBUG
    std::puts("Rodrigues_z: _landmark3d");
    for(int i=0; i<21; i++){
        std::cout << i << " " <<  _landmark3d[i];
    }
#endif
}

void landmarks_to_shm::gesture::init_gestures3d(void)
{
    init_gesture_num();
    gestures3d_ = new landmarks_datatype::gesture_t [gesture_num_];
    for(int i=0; i<gesture_num_; i++){
        gestures3d_[i].co = new landmarks_datatype::coordinate3d_t [landmarks_datatype::norm_landmark_size];
    }
}

void landmarks_to_shm::gesture::delete_gestures3d(void)
{
    for(int i=0; i<gesture_num_; i++){
        delete [] gestures3d_[i].co;
    }
    delete [] gestures3d_;
}

void landmarks_to_shm::gesture::init_norm_landmark3d(void)
{
    norm_landmark3d_ = new landmarks_datatype::coordinate3d_t [landmarks_datatype::norm_landmark_size];
}

void landmarks_to_shm::gesture::delete_norm_landmark3d(void)
{
    delete [] norm_landmark3d_;
}

void landmarks_to_shm::gesture::load_resize_rotate_norm_landmark3d(
    landmarks_datatype::coordinate3d_t *_shm_norm_landmark3d)
{
    for(int i=0; i<landmarks_datatype::norm_landmark_size; i++){
        norm_landmark3d_[i] = _shm_norm_landmark3d[i];
    }
#ifdef PRINT_DEBUG
    std::puts("similarity, after load: norm_landmark3d_");
    for(int i=0; i<21; i++){
        std::cout << i << " " <<  norm_landmark3d_[i];
    }
#endif

    resize_rotate3d(norm_landmark3d_);
    //init_angle3d(norm_landmark3d_);
}

void landmarks_to_shm::gesture::resize(
    landmarks_datatype::coordinate3d_t* _norm_landmark3d)
{
    for(int i=0; i<landmarks_datatype::norm_landmark_size; i++){
        _norm_landmark3d[i] = _norm_landmark3d[i] * landmarks_datatype::image_size;
    }
#ifdef PRINT_DEBUG
    std::puts("resize: _norm_landmark3d");
    for(int i=0; i<21; i++){
        std::cout << i << " " <<  _norm_landmark3d[i];
    }
#endif
}

void landmarks_to_shm::gesture::init_crossVector(landmarks_datatype::coordinate3d_t* _landmark3d)
{
    const landmarks_datatype::coordinate3d_t a = _landmark3d[start_crossVector_index_] - _landmark3d[start_keypoint_index_];
    const landmarks_datatype::coordinate3d_t b = _landmark3d[end_crossVector_index_] - _landmark3d[start_keypoint_index_];

    crossVector = landmarks_datatype::cross_product(a, b);
#ifdef PRINT_DEBUG
    std::puts("init_crossVector: crossVector");
    std::cout << crossVector;
#endif
}

void landmarks_to_shm::gesture::norm_root(landmarks_datatype::coordinate3d_t* _landmark3d)
{
    landmarks_datatype::coordinate3d_t root = _landmark3d[start_keypoint_index_];
    _landmark3d[0] = {0.f, 0.f, 0.f};
    for(int i=1; i<landmarks_datatype::norm_landmark_size; i++){
        _landmark3d[i] = _landmark3d[i] - root;
    }
#ifdef PRINT_DEBUG
    std::puts("norm_root: _landmark3d");
    for(int i=0; i<21; i++){
        std::cout << i << " " <<  _landmark3d[i];
    }
#endif
}

float landmarks_to_shm::gesture::cosine_angle3d(
    landmarks_datatype::coordinate3d_t a, landmarks_datatype::coordinate3d_t b, landmarks_datatype::coordinate3d_t c)
{
    const landmarks_datatype::coordinate3d_t x = a - b;
    const landmarks_datatype::coordinate3d_t y = c - b;
    float const cosine = landmarks_datatype::dot_product3d(x, y) / (x.distance3d() * y.distance3d());
#ifdef PRINT_DEBUG
    std::puts("cosine_angle3d: x, y, cosine, x.distance, y.distance");
    std::cout << x << " " << y << " " << cosine << " " << x.distance3d() << " " << y.distance3d() << "\n";
#endif
    return acosf(cosine);
}

void landmarks_to_shm::gesture::init_angle3d(landmarks_datatype::coordinate3d_t* _landmark3d)
{
    for(int i=0; i<cmp_angle_joints_num_; i++){
        int joint = cmp_angle_joints_[i];
        _landmark3d[joint].angle = cosine_angle3d(_landmark3d[joint-1], _landmark3d[joint], _landmark3d[joint+1]);
    }
}

void landmarks_to_shm::gesture::init_cmp_angle_joints(void)
{
    cmp_angle_joints_ = new int [cmp_angle_joints_num_];
    for(int i=0; i<cmp_angle_joints_num_; i++){
        cmp_angle_joints_[i] = (i%3) + (i/3)*4 + 1;
    }
}

void landmarks_to_shm::gesture::delete_cmp_angle_joints(void)
{
    delete cmp_angle_joints_;
}
