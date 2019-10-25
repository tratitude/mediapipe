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

proto_library(
    name = "ssd_anchors_calculator_proto",
    srcs = ["ssd_anchors_calculator.proto"],
    visibility = ["//visibility:public"],
    deps = ["//mediapipe/framework:calculator_proto"],
)

proto_library(
    name = "tflite_custom_op_resolver_calculator_proto",
    srcs = ["tflite_custom_op_resolver_calculator.proto"],
    visibility = ["//visibility:public"],
    deps = ["//mediapipe/framework:calculator_proto"],
)

proto_library(
    name = "tflite_inference_calculator_proto",
    srcs = ["tflite_inference_calculator.proto"],
    visibility = ["//visibility:public"],
    deps = ["//mediapipe/framework:calculator_proto"],
)

proto_library(
    name = "tflite_converter_calculator_proto",
    srcs = ["tflite_converter_calculator.proto"],
    visibility = ["//visibility:public"],
    deps = ["//mediapipe/framework:calculator_proto"],
)

proto_library(
    name = "tflite_tensors_to_segmentation_calculator_proto",
    srcs = ["tflite_tensors_to_segmentation_calculator.proto"],
    visibility = ["//visibility:public"],
    deps = ["//mediapipe/framework:calculator_proto"],
)

proto_library(
    name = "tflite_tensors_to_detections_calculator_proto",
    srcs = ["tflite_tensors_to_detections_calculator.proto"],
    visibility = ["//visibility:public"],
    deps = ["//mediapipe/framework:calculator_proto"],
)

proto_library(
    name = "tflite_tensors_to_classification_calculator_proto",
    srcs = ["tflite_tensors_to_classification_calculator.proto"],
    visibility = ["//visibility:public"],
    deps = ["//mediapipe/framework:calculator_proto"],
)

proto_library(
    name = "tflite_tensors_to_landmarks_calculator_proto",
    srcs = ["tflite_tensors_to_landmarks_calculator.proto"],
    visibility = ["//visibility:public"],
    deps = ["//mediapipe/framework:calculator_proto"],
)

mediapipe_cc_proto_library(
    name = "ssd_anchors_calculator_cc_proto",
    srcs = ["ssd_anchors_calculator.proto"],
    cc_deps = ["//mediapipe/framework:calculator_cc_proto"],
    visibility = ["//visibility:public"],
    deps = [":ssd_anchors_calculator_proto"],
)

mediapipe_cc_proto_library(
    name = "tflite_custom_op_resolver_calculator_cc_proto",
    srcs = ["tflite_custom_op_resolver_calculator.proto"],
    cc_deps = ["//mediapipe/framework:calculator_cc_proto"],
    visibility = ["//visibility:public"],
    deps = [":tflite_custom_op_resolver_calculator_proto"],
)

mediapipe_cc_proto_library(
    name = "tflite_converter_calculator_cc_proto",
    srcs = ["tflite_converter_calculator.proto"],
    cc_deps = ["//mediapipe/framework:calculator_cc_proto"],
    visibility = ["//visibility:public"],
    deps = [":tflite_converter_calculator_proto"],
)

mediapipe_cc_proto_library(
    name = "tflite_tensors_to_segmentation_calculator_cc_proto",
    srcs = ["tflite_tensors_to_segmentation_calculator.proto"],
    cc_deps = ["//mediapipe/framework:calculator_cc_proto"],
    visibility = ["//visibility:public"],
    deps = [":tflite_tensors_to_segmentation_calculator_proto"],
)

mediapipe_cc_proto_library(
    name = "tflite_inference_calculator_cc_proto",
    srcs = ["tflite_inference_calculator.proto"],
    cc_deps = ["//mediapipe/framework:calculator_cc_proto"],
    visibility = ["//visibility:public"],
    deps = [":tflite_inference_calculator_proto"],
)

mediapipe_cc_proto_library(
    name = "tflite_tensors_to_detections_calculator_cc_proto",
    srcs = ["tflite_tensors_to_detections_calculator.proto"],
    cc_deps = ["//mediapipe/framework:calculator_cc_proto"],
    visibility = ["//visibility:public"],
    deps = [":tflite_tensors_to_detections_calculator_proto"],
)

mediapipe_cc_proto_library(
    name = "tflite_tensors_to_classification_calculator_cc_proto",
    srcs = ["tflite_tensors_to_classification_calculator.proto"],
    cc_deps = ["//mediapipe/framework:calculator_cc_proto"],
    visibility = ["//visibility:public"],
    deps = [":tflite_tensors_to_classification_calculator_proto"],
)

mediapipe_cc_proto_library(
    name = "tflite_tensors_to_landmarks_calculator_cc_proto",
    srcs = ["tflite_tensors_to_landmarks_calculator.proto"],
    cc_deps = ["//mediapipe/framework:calculator_cc_proto"],
    visibility = ["//visibility:public"],
    deps = [":tflite_tensors_to_landmarks_calculator_proto"],
)

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
        "//mediapipe/framework/port:ret_check",
        "@org_tensorflow//tensorflow/lite:framework",
    ],
    visibility = ["//visibility:public"],
    alwayslink = 1,
)