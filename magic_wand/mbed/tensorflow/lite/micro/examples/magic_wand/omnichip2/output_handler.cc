/* Copyright 2019 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#include "tensorflow/lite/micro/examples/magic_wand/output_handler.h"

void HandleOutput(tflite::ErrorReporter* error_reporter, int kind) {
  // Print some ASCII art for each gesture
  switch (kind) {
    case 0:
      error_reporter->Report("WING\r\n\n\n");
      break;
    case 1:
      error_reporter->Report("RING\r\n\n\n");
      break;
    case 2:
      error_reporter->Report("SLOPE\r\n\n\n");
      break;
    case 3:
      error_reporter->Report("LETTERA\r\n\n\n");
      break;
  }
}
