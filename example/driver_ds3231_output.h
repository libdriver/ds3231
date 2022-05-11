/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 *
 * @file      driver_ds3231_output.h
 * @brief     driver ds3231 output header file
 * @version   2.0.0
 * @author    Shifeng Li
 * @date      2021-03-15
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2021/03/15  <td>2.0      <td>Shifeng Li  <td>format the code
 * <tr><td>2020/11/30  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */
  
#ifndef DRIVER_DS3231_OUTPUT_H
#define DRIVER_DS3231_OUTPUT_H

#include "driver_ds3231_interface.h"
#include <time.h>

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @addtogroup ds3231_example_driver
 * @{
 */

/**
 * @brief ds3231 output example default definition
 */
#define DS3231_OUTPUT_DEFAULT_AGING_OFFSET        0        /**< 0 offset */

/**
 * @brief  output example init
 * @return status code
 *         - 0 success
 *         - 1 init failed
 * @note   none
 */
uint8_t ds3231_output_init(void);

/**
 * @brief  output example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t ds3231_output_deinit(void);

/**
 * @brief     output example set the time
 * @param[in] *t points to a time structure
 * @return    status code
 *            - 0 success
 *            - 1 set time failed
 * @note      none
 */
uint8_t ds3231_output_set_time(ds3231_time_t *t);

/**
 * @brief      output example get the time
 * @param[out] *t points to a time structure
 * @return     status code
 *             - 0 success
 *             - 1 get time failed
 * @note       none
 */
uint8_t ds3231_output_get_time(ds3231_time_t *t);

/**
 * @brief     output example set the time by a unix timestamp
 * @param[in] timestamp is a unix timestamp
 * @return    status code
 *            - 0 success
 *            - 1 set timestamp failed
 * @note      none
 */
uint8_t ds3231_output_set_timestamp(time_t timestamp);

/**
 * @brief     output example set the local time zone
 * @param[in] zone is the local time zone
 * @return    status code
 *            - 0 success
 * @note      none
 */
uint8_t ds3231_output_set_timestamp_time_zone(int8_t zone);

/**
 * @brief      output example get the time in a unix timestamp
 * @param[out] *timestamp points to a unix timestamp buffer
 * @return     status code
 *             - 0 success
 *             - 1 get timestamp failed
 * @note       none
 */
uint8_t ds3231_output_get_timestamp(time_t *timestamp);

/**
 * @brief      output example get the local time zone
 * @param[out] *zone points to a local time zone buffer
 * @return     status code
 *             - 0 success
 * @note       none
 */
uint8_t ds3231_output_get_timestamp_time_zone(int8_t *zone);

/**
 * @brief      output example get the current temperature
 * @param[out] *raw points to a raw temperature buffer
 * @param[out] *s points to a converted temperature buffer
 * @return     status code
 *             - 0 success
 *             - 1 read temperature failed
 * @note       none
 */
uint8_t ds3231_output_get_temperature(int16_t *raw, float *s);

/**
 * @brief      output example get the ascii time
 * @param[out] *buf points to an ascii buffer
 * @param[in]  len is the data length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t ds3231_output_get_ascii_time(char *buf, uint8_t len);

/**
 * @brief     output example enable or disable the square wave
 * @param[in] enable is a bool value
 * @return    status code
 *             - 0 success
 *             - 1 set square wave failed
 * @note      none
 */
uint8_t ds3231_output_set_square_wave(ds3231_bool_t enable);

/**
 * @brief      output example get the square wave status
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *              - 0 success
 *              - 1 get square wave failed
 * @note       none
 */
uint8_t ds3231_output_get_square_wave(ds3231_bool_t *enable);

/**
 * @brief     output example enable or disable the 32KHz output
 * @param[in] enable is a bool value
 * @return    status code
 *             - 0 success
 *             - 1 set 32khz output failed
 * @note      none
 */
uint8_t ds3231_output_set_32khz_output(ds3231_bool_t enable);

/**
 * @brief      output example get the 32KHz output status
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *              - 0 success
 *              - 1 get 32KHz output failed
 * @note       none
 */
uint8_t ds3231_output_get_32khz_output(ds3231_bool_t *enable);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
