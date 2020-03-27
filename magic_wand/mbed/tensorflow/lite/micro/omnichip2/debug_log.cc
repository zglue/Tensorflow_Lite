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

#include <string.h>

#include "nrfx_uarte.h"
#include "tensorflow/lite/micro/debug_log.h"

#define NRF52_INVALID_GPIO_PIN 0xFF
#define BOARD_UART0_RX_PIN 8  //(GPIO_INPUT  | GPIO_PIN8)
#define BOARD_UART0_TX_PIN 6  //(GPIO_OUTPUT | GPIO_VALUE_ONE | GPIO_PIN6)
#define BOARD_UART0_CTS_PIN NRF52_INVALID_GPIO_PIN
#define BOARD_UART0_RTS_PIN NRF52_INVALID_GPIO_PIN

extern "C" void DebugLog(const char* s) {
  static bool is_initialized = false;
  static nrfx_uarte_t serial0_uarte;
  if (!is_initialized) {
    serial0_uarte = NRFX_UARTE_INSTANCE(0);

    static const nrfx_uarte_config_t m_uarte0_drv_config = {
        .pseltxd = BOARD_UART0_TX_PIN,
        .pselrxd = BOARD_UART0_RX_PIN,
        .pselcts = BOARD_UART0_CTS_PIN,
        .pselrts = BOARD_UART0_RTS_PIN,
        .p_context = NULL,
        .hwfc = (nrf_uarte_hwfc_t)NRFX_UARTE_DEFAULT_CONFIG_HWFC,
        .parity = (nrf_uarte_parity_t)NRFX_UARTE_DEFAULT_CONFIG_PARITY,
        .baudrate = (nrf_uarte_baudrate_t)NRFX_UARTE_DEFAULT_CONFIG_BAUDRATE,
        .interrupt_priority = NRFX_UARTE_DEFAULT_CONFIG_IRQ_PRIORITY,
    };

    nrfx_uarte_init(&serial0_uarte, &m_uarte0_drv_config, NULL);

    is_initialized = true;
  }

  nrfx_uarte_tx(&serial0_uarte, (uint8_t*)s, strlen(s));
}
