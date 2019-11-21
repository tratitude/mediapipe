// Copyright 2019 The MediaPipe Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// An example of sending OpenCV webcam frames into a MediaPipe graph.
// This example requires a linux computer and a GPU with EGL support drivers.

#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/formats/image_frame.h"
#include "mediapipe/framework/formats/image_frame_opencv.h"
#include "mediapipe/framework/port/commandlineflags.h"
#include "mediapipe/framework/port/file_helpers.h"
#include "mediapipe/framework/port/opencv_highgui_inc.h"
#include "mediapipe/framework/port/opencv_imgproc_inc.h"
#include "mediapipe/framework/port/opencv_video_inc.h"
#include "mediapipe/framework/port/parse_text_proto.h"
#include "mediapipe/framework/port/status.h"
#include "mediapipe/gpu/gl_calculator_helper.h"
#include "mediapipe/gpu/gpu_buffer.h"
#include "mediapipe/gpu/gpu_shared_data_internal.h"

//Take stream from /mediapipe/graphs/hand_tracking/hand_tracking_mobile.pbtxt
// RendererSubgraph - LANDMARKS:hand_landmarks
#include "mediapipe/calculators/util/landmarks_to_render_data_calculator.pb.h"
#include "mediapipe/framework/formats/landmark.pb.h"
// RectToRenderDataCalculator
// RendererSubgraph - NORM_RECT:hand_rect
#include "mediapipe/calculators/util/rect_to_render_data_calculator.pb.h"
#include "mediapipe/framework/formats/rect.pb.h"

#include "mediapipe/landmarks_to_shm/landmarks_to_shm.h"

#include <iostream>
#include <thread>
#include <ctime>

//#define IMSHOW_ENABLE
#define FPS_TEST
#define FPS60

constexpr char kInputStream[] = "input_video";
constexpr char kOutputStream[] = "output_video";
constexpr char kWindowName[] = "MediaPipe";
constexpr char kLandmarksStream[] = "hand_landmarks";
constexpr char kRectStream[] = "hand_rect";

DEFINE_string(
    calculator_graph_config_file, "",
    "Name of file containing text format CalculatorGraphConfig proto.");
DEFINE_string(input_video_path, "",
              "Full path of video to load. "
              "If not provided, attempt to use a webcam.");
DEFINE_string(output_video_path, "",
              "Full path of where to save result (.mp4 only). "
              "If not provided, show result in a window.");
//****************************************************************************************/
// Warring! Global varible
landmarks_to_shm::shm shmObj;
landmarks_to_shm::gesture gesObj;
double input_video_fps = 0.f;
double gesture_fps = 0.f;
double gesture_time = 0.f;
long long unsigned int gesture_cnt = 0;
struct timespec start, end;
//****************************************************************************************/
// POSIX time measurement
struct timespec diff(struct timespec start, struct timespec end) {
  struct timespec temp;
  if ((end.tv_nsec-start.tv_nsec)<0) {
    temp.tv_sec = end.tv_sec-start.tv_sec-1;
    temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
  } else {
    temp.tv_sec = end.tv_sec-start.tv_sec;
    temp.tv_nsec = end.tv_nsec-start.tv_nsec;
  }
  return temp;
}
//****************************************************************************************/
::mediapipe::Status RunMPPGraph() {
  std::string calculator_graph_config_contents;
  MP_RETURN_IF_ERROR(mediapipe::file::GetContents(
      FLAGS_calculator_graph_config_file, &calculator_graph_config_contents));
  LOG(INFO) << "Get calculator graph config contents: "
            << calculator_graph_config_contents;
  mediapipe::CalculatorGraphConfig config =
      mediapipe::ParseTextProtoOrDie<mediapipe::CalculatorGraphConfig>(
          calculator_graph_config_contents);

  LOG(INFO) << "Initialize the calculator graph.";
  mediapipe::CalculatorGraph graph;
  MP_RETURN_IF_ERROR(graph.Initialize(config));

  LOG(INFO) << "Initialize the GPU.";
  ASSIGN_OR_RETURN(auto gpu_resources, mediapipe::GpuResources::Create());
  MP_RETURN_IF_ERROR(graph.SetGpuResources(std::move(gpu_resources)));
  mediapipe::GlCalculatorHelper gpu_helper;
  gpu_helper.InitializeForTest(graph.GetGpuResources().get());

  LOG(INFO) << "Initialize the camera or load the video.";
  cv::VideoCapture capture(cv::CAP_DSHOW + 0);
  const bool load_video = !FLAGS_input_video_path.empty();
  if (load_video) {
    capture.open(FLAGS_input_video_path);
  } else {
    capture.open(0);
  }
  RET_CHECK(capture.isOpened());
#ifdef FPS60
// Found unchecked GL error: GL_INVALID_FRAMEBUFFER_OPERATION
  int fourcc = cv::VideoWriter::fourcc('M', 'J', 'P', 'G');
  capture.set(cv::CAP_PROP_FOURCC, fourcc);
  capture.set(cv::CAP_PROP_FRAME_WIDTH,1280);
  capture.set(cv::CAP_PROP_FRAME_HEIGHT,720);
  capture.set(cv::CAP_PROP_FPS, 60);
#endif

  cv::VideoWriter writer;
  const bool save_video = !FLAGS_output_video_path.empty();
  if (save_video) {
    LOG(INFO) << "Prepare video writer.";
    cv::Mat test_frame;
    capture.read(test_frame);                    // Consume first frame.
    capture.set(cv::CAP_PROP_POS_AVI_RATIO, 0);  // Rewind to beginning.
    writer.open(FLAGS_output_video_path,
              #ifdef FPS60
                mediapipe::fourcc('M', 'J', 'P', 'G'),  // .mjpg
              #elif
                mediapipe::fourcc('a', 'v', 'c', '1'),  // .mp4
              #endif
                capture.get(cv::CAP_PROP_FPS), test_frame.size());
    RET_CHECK(writer.isOpened());
  } else {
    cv::namedWindow(kWindowName, /*flags=WINDOW_AUTOSIZE*/ 1);
  }

  LOG(INFO) << "Start running the calculator graph.";
#ifdef IMSHOW_ENABLE
  ASSIGN_OR_RETURN(mediapipe::OutputStreamPoller poller,
                   graph.AddOutputStreamPoller(kOutputStream));
#endif
  // hand landmarks stream
  ASSIGN_OR_RETURN(mediapipe::OutputStreamPoller poller_landmark,
            graph.AddOutputStreamPoller(kLandmarksStream));
  
  // rect stream
  ASSIGN_OR_RETURN(mediapipe::OutputStreamPoller poller_rect,
            graph.AddOutputStreamPoller(kRectStream));

  MP_RETURN_IF_ERROR(graph.StartRun({}));

  LOG(INFO) << "Start grabbing and processing frames.";

  size_t frame_timestamp = 0;
  bool grab_frames = true;
#ifdef FPS_TEST
  // get fps
  input_video_fps = capture.get(cv::CAP_PROP_FPS);
  // time start
  clock_gettime(CLOCK_MONOTONIC_COARSE, &start);
#endif
  while (grab_frames) {
    // Capture opencv camera or video frame.
    cv::Mat camera_frame_raw;
    capture >> camera_frame_raw;
    if (camera_frame_raw.empty()) break;  // End of video.
    cv::Mat camera_frame;
    cv::cvtColor(camera_frame_raw, camera_frame, cv::COLOR_BGR2RGB);
    if (!load_video) {
      cv::flip(camera_frame, camera_frame, /*flipcode=HORIZONTAL*/ 1);
    }

    // Wrap Mat into an ImageFrame.
    auto input_frame = absl::make_unique<mediapipe::ImageFrame>(
        mediapipe::ImageFormat::SRGB, camera_frame.cols, camera_frame.rows,
        mediapipe::ImageFrame::kGlDefaultAlignmentBoundary);
    cv::Mat input_frame_mat = mediapipe::formats::MatView(input_frame.get());
    camera_frame.copyTo(input_frame_mat);

    // Prepare and add graph input packet.
    MP_RETURN_IF_ERROR(
        gpu_helper.RunInGlContext([&input_frame, &frame_timestamp, &graph,
                                   &gpu_helper]() -> ::mediapipe::Status {
          // Convert ImageFrame to GpuBuffer.
          auto texture = gpu_helper.CreateSourceTexture(*input_frame.get());
          auto gpu_frame = texture.GetFrame<mediapipe::GpuBuffer>();
          glFlush();
          texture.Release();
          // Send GPU image packet into the graph.
          MP_RETURN_IF_ERROR(graph.AddPacketToInputStream(
              kInputStream, mediapipe::Adopt(gpu_frame.release())
                                .At(mediapipe::Timestamp(frame_timestamp++))));
          return ::mediapipe::OkStatus();
        }));

    // Get the graph result packet, or stop if that fails.
  #ifdef IMSHOW_ENABLE
    mediapipe::Packet packet;
  #endif
    mediapipe::Packet landmark_packet;
    mediapipe::Packet rect_packet;
  #ifdef IMSHOW_ENABLE
    if (!poller.Next(&packet)) break;
  #endif
    if (!poller_landmark.Next(&landmark_packet)) break;
    if (!poller_rect.Next(&rect_packet)) break;

    std::unique_ptr<mediapipe::ImageFrame> output_frame;
    auto& output_landmarks = landmark_packet.Get<std::vector<::mediapipe::NormalizedLandmark>>();
    auto& output_rect = rect_packet.Get<::mediapipe::NormalizedRect>();
  #ifdef IMSHOW_ENABLE
    // Convert GpuBuffer to ImageFrame.
    MP_RETURN_IF_ERROR(gpu_helper.RunInGlContext(
        [&packet, &output_frame, &gpu_helper]() -> ::mediapipe::Status {
          auto& gpu_frame = packet.Get<mediapipe::GpuBuffer>();
          auto texture = gpu_helper.CreateSourceTexture(gpu_frame);
          output_frame = absl::make_unique<mediapipe::ImageFrame>(
              mediapipe::ImageFormatForGpuBufferFormat(gpu_frame.format()),
              gpu_frame.width(), gpu_frame.height(),
              mediapipe::ImageFrame::kGlDefaultAlignmentBoundary);
          gpu_helper.BindFramebuffer(texture);
          const auto info =
              mediapipe::GlTextureInfoForGpuBufferFormat(gpu_frame.format(), 0);
          glReadPixels(0, 0, texture.width(), texture.height(), info.gl_format,
                       info.gl_type, output_frame->MutablePixelData());
          glFlush();
          texture.Release();
          return ::mediapipe::OkStatus();
        }));
  
    // Convert back to opencv for display or saving.
    cv::Mat output_frame_mat = mediapipe::formats::MatView(output_frame.get());
    cv::cvtColor(output_frame_mat, output_frame_mat, cv::COLOR_RGB2BGR);
    if (save_video) {
      writer.write(output_frame_mat);
    } else {
    
      cv::imshow(kWindowName, output_frame_mat);
      // Press any key to exit.
      const int pressed_key = cv::waitKey(1);
      if (pressed_key >= 0 && pressed_key != 255) grab_frames = false;
    }
  #endif
    // restore landmark to shm array
    //Open the managed segment
    boost::interprocess::managed_shared_memory segment(
        boost::interprocess::open_or_create, 
        landmarks_datatype::shm_name,
        landmarks_datatype::shm_size);

    //Find the vector using the c-string name
    landmarks_datatype::coordinate3d_t *norm_landmark = 
        segment.find<landmarks_datatype::coordinate3d_t>(
        landmarks_datatype::norm_landmark_name).first;
    for (int counter = 0; counter<landmarks_datatype::norm_landmark_size; counter++) {
      norm_landmark[counter] = {output_landmarks[counter].x(), output_landmarks[counter].y(), output_landmarks[counter].z()};
    }

    // shmObj, gesObj is global object
    gesObj.load_resize_rotate_norm_landmark3d(norm_landmark);
    
    float match_gesture;
    gesObj.similarity(&match_gesture);

    landmarks_datatype::coordinate3d_t *bbCentral = 
        segment.find<landmarks_datatype::coordinate3d_t>(
        landmarks_datatype::bbCentral_name).first;
    // x=x_center, y=y_center, z=match_gesture
    bbCentral[0] = {output_rect.x_center(), output_rect.y_center(), match_gesture};
  #ifdef FPS_TEST
    // count gesture fps
    //if(gesture_cnt < ULLONG_MAX-2){
    if(gesture_cnt < 1000){
      ++gesture_cnt;
    }
    else{
      std::puts("gesture_fps_cnt finished");
      break;
    }
  #endif
  }
#ifdef FPS_TEST
  // time end
  clock_gettime(CLOCK_MONOTONIC_COARSE, &end);
  struct timespec temp = diff(start, end);
  gesture_time = (temp.tv_sec + (double) temp.tv_nsec / 1000000000.0);
  gesture_fps = gesture_cnt / gesture_time;
#endif
  LOG(INFO) << "Shutting down.";
  if (writer.isOpened()) writer.release();
  MP_RETURN_IF_ERROR(graph.CloseInputStream(kInputStream));
  return graph.WaitUntilDone();
}

int main(int argc, char** argv) {
  // ges, shm are global object
  gesObj.load_resize_rotate_gestures3d(landmarks_datatype::gesture_path);
  // relative path failed
  //gesObj.load_gesture("../../../store_gesture");
  gesObj.print_gestures3d();

  // maybe need run similarity after getting landmark every frame
  //std::thread similarity(&landmarks_to_shm::gesture::similarity, &ges);

  google::InitGoogleLogging(argv[0]);
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  ::mediapipe::Status run_status = RunMPPGraph();
  if (!run_status.ok()) {
    LOG(ERROR) << "Failed to run the graph: " << run_status.message();
  } else {
    LOG(INFO) << "Success!";
  }
#ifdef FPS_TEST
  std::puts("*********************************************************");
  std::printf("input_video_fps: %lf\n", input_video_fps);
  std::printf("gesture_time: %lf gesture_cnt: %lld gesture_fps: %lf\n", gesture_time, gesture_cnt, gesture_fps);
  std::puts("*********************************************************");
#endif
  return 0;
}
