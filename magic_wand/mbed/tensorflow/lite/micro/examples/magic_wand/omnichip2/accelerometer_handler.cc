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

#include "tensorflow/lite/micro/examples/magic_wand/accelerometer_handler.h"
#include <math.h>
#include <string.h>
#include "tensorflow/lite/micro/examples/magic_wand/constants.h"

#include "m_drv_mc36xx.h"

// A buffer holding the last 200 sets of 3-channel values
float save_data[600] = {0.0};
// Most recent position in the save_data buffer
int begin_index = 0;
// True if there is not yet enough data to run inference
bool pending_initial_data = true;
// How often we should save a measurement during downsampling
int sample_every_n;
// The number of measurements since we last saved one
int sample_skip_counter = 1;

static void enable_mc3672_fifo() {
  M_DRV_MC36XX_SetMode(E_M_DRV_MC36XX_MODE_STANDBY);
  M_DRV_MC36XX_SetSampleRate(E_M_DRV_MC36XX_CWAKE_SR_LP_27Hz,
                             E_M_DRV_MC36XX_SNIFF_SR_DEFAULT_6Hz);
  M_DRV_MC36XX_ConfigRegRngResCtrl(E_M_DRV_MC36XX_RANGE_4G,
                                   E_M_DRV_MC36XX_RES_12BIT);
  M_DRV_MC36XX_EnableFIFO(E_M_DRV_MC36XX_FIFO_CTL_ENABLE,
                          E_M_DRV_MC36XX_FIFO_MODE_NORMAL, 1);
  M_DRV_MC36XX_ConfigINT(1, 0, 0, 0, 0);
  M_DRV_MC36XX_SetMode(E_M_DRV_MC36XX_MODE_CWAKE);
  mcube_delay_ms(50);  // wait > 3 cycles of heartbeat clock (500Hz)
}

TfLiteStatus SetupAccelerometer(tflite::ErrorReporter* error_reporter) {
  // Switch on the IMU
  if (M_DRV_MC36XX_Init() != 0) {
    error_reporter->Report("Failed to initialize IMU");
    return kTfLiteError;
  }

  enable_mc3672_fifo();

  // Determine how many measurements to keep in order to meet kTargetHz
  float sample_rate = 27;  // TODO: remove hardcode
  sample_every_n = static_cast<int>(roundf(sample_rate / kTargetHz));

  error_reporter->Report("Magic starts!");

  return kTfLiteOk;
}

bool ReadAccelerometer(tflite::ErrorReporter* error_reporter, float* input,
                       int length, bool reset_buffer) {
  // Clear the buffer if required, e.g. after a successful prediction
  if (reset_buffer) {
    memset(save_data, 0, 600 * sizeof(float));
    begin_index = 0;
    pending_initial_data = true;
  }

  // Keep track of whether we stored any new data
  bool new_data = false;

  // check if new data arrived since last reg read
  if (_M_DRV_MC36XX_REG_STATUS_1_NEW_DATA(
          M_DRV_MC36XX_ReadReg(E_M_DRV_MC36XX_REG_STATUS_1)) == 0x08) {
    // while fifo is not empty, continue reading
    while (_M_DRV_MC36XX_REG_STATUS_1_FIFO_EMPTY(
               M_DRV_MC36XX_ReadReg(E_M_DRV_MC36XX_REG_STATUS_1)) != 0x10) {
      // Throw away this sample unless it's the nth
      if (sample_skip_counter != sample_every_n) {
        sample_skip_counter += 1;
        continue;
      }

      float read_data[1][3];
      M_DRV_MC36XX_ReadData(read_data, 1);

      float x_transformed = read_data[0][0] * 1000;
      float y_transformed = read_data[0][1] * 1000;
      float z_transformed = read_data[0][2] * 1000;

      save_data[begin_index++] = x_transformed;
      save_data[begin_index++] = y_transformed;
      save_data[begin_index++] = z_transformed;
      // Since we took a sample, reset the skip counter
      sample_skip_counter = 1;
      // If we reached the end of the circle buffer, reset
      if (begin_index >= 600) {
        begin_index = 0;
      }
      new_data = true;
    }
  }

  // Skip this round if data is not ready yet
  if (!new_data) {
    return false;
  }

  // Check if we are ready for prediction or still pending more initial data
  if (pending_initial_data && begin_index >= 150) {
    pending_initial_data = false;
  }

  // Return if we don't have enough data
  if (pending_initial_data) {
    return false;
  }

  // Copy the requested number of bytes to the provided input tensor
  for (int i = 0; i < length; ++i) {
    int ring_array_index = begin_index + i - length;
    if (ring_array_index < 0) {
      ring_array_index += 600;
    }
    input[i] = save_data[ring_array_index];
  }

  return true;
}

void collect_data(tflite::ErrorReporter* error_reporter) {
  // check if new data arrived since last reg read
  if (_M_DRV_MC36XX_REG_STATUS_1_NEW_DATA(
          M_DRV_MC36XX_ReadReg(E_M_DRV_MC36XX_REG_STATUS_1)) == 0x08) {
    // while fifo is not empty, continue reading
    while (_M_DRV_MC36XX_REG_STATUS_1_FIFO_EMPTY(
               M_DRV_MC36XX_ReadReg(E_M_DRV_MC36XX_REG_STATUS_1)) != 0x10) {
      float read_data[1][3];
      M_DRV_MC36XX_ReadData(read_data, 1);
      error_reporter->Report("%d,", (int)(read_data[0][0] * 1000));
      error_reporter->Report("%d,", (int)(read_data[0][1] * 1000));
      error_reporter->Report("%d\r\n", (int)(read_data[0][2] * 1000));
    }
  }
}
