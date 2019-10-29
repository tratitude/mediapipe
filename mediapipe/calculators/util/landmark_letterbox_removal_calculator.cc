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

#include <cmath>
#include <vector>

#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/formats/landmark.pb.h"
#include "mediapipe/framework/port/ret_check.h"

#include "mediapipe/landmarks_to_shm/landmarks_to_shm.h"

#ifdef PRINT_DEBUG
  #include <iostream>
#endif

namespace mediapipe {

namespace {

constexpr char kLandmarksTag[] = "LANDMARKS";
constexpr char kLetterboxPaddingTag[] = "LETTERBOX_PADDING";

}  // namespace

// Adjusts landmark locations on a letterboxed image to the corresponding
// locations on the same image with the letterbox removed. This is useful to map
// the landmarks inferred from a letterboxed image, for example, output of
// the ImageTransformationCalculator when the scale mode is FIT, back to the
// corresponding input image before letterboxing.
//
// Input:
//   LANDMARKS: An std::vector<NormalizedLandmark> representing landmarks on an
//   letterboxed image.
//
//   LETTERBOX_PADDING: An std::array<float, 4> representing the letterbox
//   padding from the 4 sides ([left, top, right, bottom]) of the letterboxed
//   image, normalized to [0.f, 1.f] by the letterboxed image dimensions.
//
// Output:
//   LANDMARKS: An std::vector<NormalizedLandmark> representing landmarks with
//   their locations adjusted to the letterbox-removed (non-padded) image.
//
// Usage example:
// node {
//   calculator: "LandmarkLetterboxRemovalCalculator"
//   input_stream: "LANDMARKS:landmarks"
//   input_stream: "LETTERBOX_PADDING:letterbox_padding"
//   output_stream: "LANDMARKS:adjusted_landmarks"
// }
class LandmarkLetterboxRemovalCalculator : public CalculatorBase {
 public:
  static ::mediapipe::Status GetContract(CalculatorContract* cc) {
    RET_CHECK(cc->Inputs().HasTag(kLandmarksTag) &&
              cc->Inputs().HasTag(kLetterboxPaddingTag))
        << "Missing one or more input streams.";

    cc->Inputs().Tag(kLandmarksTag).Set<std::vector<NormalizedLandmark>>();
    cc->Inputs().Tag(kLetterboxPaddingTag).Set<std::array<float, 4>>();

    cc->Outputs().Tag(kLandmarksTag).Set<std::vector<NormalizedLandmark>>();

    return ::mediapipe::OkStatus();
  }

  ::mediapipe::Status Open(CalculatorContext* cc) override {
    cc->SetOffset(TimestampDiff(0));

    return ::mediapipe::OkStatus();
  }

  ::mediapipe::Status Process(CalculatorContext* cc) override {
    // Only process if there's input landmarks.
    if (cc->Inputs().Tag(kLandmarksTag).IsEmpty()) {
      return ::mediapipe::OkStatus();
    }

    const auto& input_landmarks =
        cc->Inputs().Tag(kLandmarksTag).Get<std::vector<NormalizedLandmark>>();
    const auto& letterbox_padding =
        cc->Inputs().Tag(kLetterboxPaddingTag).Get<std::array<float, 4>>();

    const float left = letterbox_padding[0];
    const float top = letterbox_padding[1];
    const float left_and_right = letterbox_padding[0] + letterbox_padding[2];
    const float top_and_bottom = letterbox_padding[1] + letterbox_padding[3];

    auto output_landmarks =
        absl::make_unique<std::vector<NormalizedLandmark>>();

/*********************************************************************
Todo: get landmarks
        float landmarks.x()
        float landmarks.y()
        float landmarks.z()
      need check or not ?
      output_landmarks or normalized_landmarks or absolute_landmarks ?
*********************************************************************/
  static landmarks_to_shm::landmarks landmarks_shm;
/*
  //Open the managed segment
  boost::interprocess::managed_shared_memory segment(boost::interprocess::open_copy_on_write, "NormLandmarks");

  //Find the vector using the c-string name
  landmarks_to_shm::NormLandVector *norm_land_vector_ptr = segment.find<landmarks_to_shm::NormLandVector>("NormLandVector").first;
*/
  landmarks_to_shm::NormLandVector *norm_land_vector_ptr;
  landmarks_shm.get_normLandVector(&norm_land_vector_ptr);
#ifdef PRINT_DEBUG
  std::printf("%p\n", norm_land_vector_ptr);
#endif
  norm_land_vector_ptr->clear();
#ifdef PRINT_DEBUG
  std::puts("clear");
#endif
  norm_land_vector_ptr->shrink_to_fit();
#ifdef PRINT_DEBUG
  std::puts("shrink_to_fit");
#endif
  /*landmarks_shm.landmarks.clear();
  landmarks_shm.landmarks.clear();*/
/********************************************************************/

/*********************************************************************
Todo: get letterbox_padding
Issue: if there has value not equal to 0 in letterbox_padding,
        will cause
I1028 23393 gl_context.cc:630] Found unchecked GL error: GL_INVALID_FRAMEBUFFER_OPERATION
W1028 23393 gl_context.cc:651] Ignoring unchecked GL error.
*********************************************************************/
  /*
  for(int i=0; i<4; i++){
    landmarks_shm.letterbox_padding[i] = letterbox_padding[i];
  }
  /*
/********************************************************************/

    for (const auto& landmark : input_landmarks) {
      NormalizedLandmark new_landmark;
      const float new_x = (landmark.x() - left) / (1.0f - left_and_right);
      const float new_y = (landmark.y() - top) / (1.0f - top_and_bottom);

      new_landmark.set_x(new_x);
      new_landmark.set_y(new_y);
      // Keep z-coord as is.
      new_landmark.set_z(landmark.z());

      output_landmarks->emplace_back(new_landmark);
/********************************************************************
push_back norm_landmarks
********************************************************************/
      norm_land_vector_ptr->push_back(new_landmark);
    #ifdef PRINT_DEBUG
      std::puts("push_back");
    #endif
/*******************************************************************/
    }

    cc->Outputs()
        .Tag(kLandmarksTag)
        .Add(output_landmarks.release(), cc->InputTimestamp());

/********************************************************************
print landmarks and letterbox_padding
********************************************************************/
    //landmarks_shm.print_norm_landmarks();
    //landmarks_shm.print_landmarks();
    //landmarks_shm.print_letterbox_padding();
/*******************************************************************/

/********************************************************************
save landmarks
********************************************************************/
    //landmarks_shm.save_norm_landmarks(landmarks_shm.norm_landmarks);
/*******************************************************************/

/********************************************************************
print shm landmarks
********************************************************************/
    landmarks_shm.print_shm_norm_landmarks();
/*******************************************************************/

    return ::mediapipe::OkStatus();
  }
};
REGISTER_CALCULATOR(LandmarkLetterboxRemovalCalculator);

}  // namespace mediapipe
