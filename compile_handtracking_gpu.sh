# Video from webcam running on desktop GPU
# This works only for linux currently
bazel build --sandbox_debug -c opt --copt -DMESA_EGL_NO_X11_HEADERS \
    mediapipe/examples/desktop/hand_tracking:hand_tracking_gpu

# It should print:
# Target //mediapipe/examples/desktop/hand_tracking:hand_tracking_gpu up-to-date:
#  bazel-bin/mediapipe/examples/desktop/hand_tracking/hand_tracking_gpu
#INFO: Elapsed time: 84.055s, Forge stats: 6858/19343 actions cached, 1.6h CPU used, 0.9s queue time, 1.68 GB ObjFS output (novel bytes: 485.1 MB), 0.0 MB local output, Critical Path: 48.14s, Remote (99.40% of the time): [queue: 0.00%, setup: 5.59%, process: 74.44%]
#INFO: Streaming build results to: http://sponge2/00c7f95f-6fbc-432d-8978-f5d361efca3b
#INFO: Build completed successfully, 22455 total actions
