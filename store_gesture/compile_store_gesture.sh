g++ store_gesture.cpp ../mediapipe/landmarks_to_shm/landmarks_to_shm.cc \
  ../mediapipe/landmarks_to_shm/landmarks_datatype.cc \
  -lrt -lpthread \
  -o store_gesture.o