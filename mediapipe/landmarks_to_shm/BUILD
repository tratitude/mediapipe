# Copyright 2019 The MediaPipe Authors.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

licenses(["notice"])  # Apache 2.0

package(default_visibility = ["//visibility:private"])

load("//mediapipe/framework/port:build_config.bzl", "mediapipe_cc_proto_library")

# https://docs.bazel.build/versions/master/be/c-cpp.html#cc_library
cc_library(
    name = "landmarks_to_shm",
    srcs = ["landmarks_to_shm.cc"],
    hdrs = ["landmarks_to_shm.h"],
    deps = [
        "//mediapipe/calculators/tflite:tflite_tensors_to_landmarks_calculator_cc_proto",
        "//mediapipe/framework:calculator_framework",
        "//mediapipe/framework/formats:landmark_cc_proto",
        # is not visible from target
        # resolve by moving landmarks_to_shm folder under mediapipe/
        # because visibility of landmark_cc_proto is "//mediapipe:__subpackages__"
        "//mediapipe/framework/port:ret_check",
        "@org_tensorflow//tensorflow/lite:framework",
    ],
    visibility = ["//visibility:public"],
    alwayslink = 1,
)