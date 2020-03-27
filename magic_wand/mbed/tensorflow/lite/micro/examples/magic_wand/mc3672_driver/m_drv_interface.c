/******************************************************************************
 *
 * Copyright (c) 2018 mCube, Inc.  All rights reserved.
 *
 * This source is subject to the mCube Software License.
 * This software is protected by Copyright and the information and source code
 * contained herein is confidential. The software including the source code
 * may not be copied and the information contained herein may not be used or
 * disclosed except with the written permission of mCube Inc.
 *
 * All other rights reserved.
 *****************************************************************************/

/**
 * @file    m_drv_interface.h
 * @author  mCube
 * @date    22 June 2018
 * @brief   Header of system I2C/SPI interface for mCube drivers.
 * @see     http://www.mcubemems.com
 */

#ifndef __M_DRV_INTERFACE_H__
#define __M_DRV_INTERFACE_H__

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 *** INCLUDE FILES
 *****************************************************************************/
/** Standard header. */
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/** mCube functions include */
#include "m_drv_interface.h"

/** Nordic functions include */
#include "nrf_delay.h"
#include "nrfx_twi.h"

/** TWI instance */
static const nrfx_twi_t m_twi = NRFX_TWI_INSTANCE(0);

/** DEFINES */
#define MC3672_I2C_ADDR 0x6C

/** Platform dependent bus functions. */
extern int m_drv_i2c_init(void) {
  const nrfx_twi_config_t twi_mc3672_config = {
      .scl = 26,
      .sda = 25,
      .frequency = NRFX_TWI_DEFAULT_CONFIG_FREQUENCY,
      .interrupt_priority = NRFX_TWI_DEFAULT_CONFIG_IRQ_PRIORITY,
      .hold_bus_uninit = NRFX_TWI_DEFAULT_CONFIG_HOLD_BUS_UNINIT};

  nrfx_twi_init(&m_twi, &twi_mc3672_config, NULL, NULL);
  nrfx_twi_enable(&m_twi);
}

extern void mcube_delay_ms(uint32_t ms) { nrf_delay_ms(ms); }

extern uint8_t mcube_write_regs(bool bSpi, uint8_t chip_select, uint8_t reg,
                                uint8_t* value, uint8_t size) {
  uint8_t p_data[65] = {reg};  // sizeof(MC3672_register_map) == 65
  memcpy(&(p_data[1]), value, size);
  nrfx_twi_tx(&m_twi, MC3672_I2C_ADDR, p_data, size + 1,
              false);  // size + 1 to include register adddress
}

extern uint8_t mcube_read_regs(bool bSpi, uint8_t chip_select, uint8_t reg,
                               uint8_t* value, uint8_t size) {
  // write register address
  nrfx_twi_tx(&m_twi, MC3672_I2C_ADDR, &reg, 1, true);
  // read register data
  nrfx_twi_rx(&m_twi, MC3672_I2C_ADDR, value, size);
}

extern uint8_t mcube_burst_read(uint8_t reg, uint8_t* value, uint8_t size);
extern uint8_t mcube_burst_read_multi(uint8_t chip_select, uint8_t reg,
                                      uint8_t* value, uint8_t size);

#ifdef __cplusplus
}
#endif

#endif  // #ifndef __M_DRV_INTERFACE_H__
