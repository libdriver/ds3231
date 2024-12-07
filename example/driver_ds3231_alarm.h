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
 * @file      driver_ds3231_alarm.h
 * @brief     driver ds3231 alarm header file
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
  
#ifndef DRIVER_DS3231_ALARM_H
#define DRIVER_DS3231_ALARM_H

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
 * @brief ds3231 alarm example default definition
 */
#define DS3231_ALARM_DEFAULT_AGING_OFFSET        0        /**< 0 offset */

/**
 * @brief  alarm irq handler
 * @return status code
 *         - 0 success
 *         - 1 run failed
 * @note   none
 */
uint8_t ds3231_alarm_irq_handler(void);

/**
 * @brief     alarm example init
 * @param[in] *alarm_receive_callback pointer to a alarm receive callback function callback
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t ds3231_alarm_init(void (*alarm_receive_callback)(uint8_t type));

/**
 * @brief  alarm example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t ds3231_alarm_deinit(void);

/**
 * @brief     alarm example set the alarm 1
 * @param[in] *t pointer to a time structure
 * @param[in] mode alarm 1 interrupt mode
 * @return    status code
 *            - 0 success
 *            - 1 set alarm1 failed
 * @note      none
 */
uint8_t ds3231_alarm_set_alarm1(ds3231_time_t *t, ds3231_alarm1_mode_t mode);

/**
 * @brief      alarm example get the alarm 1
 * @param[out] *t pointer to a time structure
 * @param[out] *mode pointer to a alarm 1 interrupt mode buffer
 * @return     status code
 *             - 0 success
 *             - 1 get alarm1 failed
 * @note       none
 */
uint8_t ds3231_alarm_get_alarm1(ds3231_time_t *t, ds3231_alarm1_mode_t *mode);

/**
 * @brief     alarm example set the alarm 2
 * @param[in] *t pointer to a time structure
 * @param[in] mode alarm 2 interrupt mode
 * @return    status code
 *            - 0 success
 *            - 1 set alarm2 failed
 * @note      none
 */
uint8_t ds3231_alarm_set_alarm2(ds3231_time_t *t, ds3231_alarm2_mode_t mode);

/**
 * @brief      alarm example get the alarm 2
 * @param[out] *t pointer to a time structure
 * @param[out] *mode pointer to a alarm 2 interrupt mode buffer
 * @return     status code
 *             - 0 success
 *             - 1 get alarm2 failed
 * @note       none
 */
uint8_t ds3231_alarm_get_alarm2(ds3231_time_t *t, ds3231_alarm2_mode_t *mode);

/**
 * @brief     alarm clear the interrupt flag
 * @param[in] alarm alarm number
 * @return    status code
 *            - 0 success
 *            - 1 alarm clear failed
 * @note      none
 */
uint8_t ds3231_alarm_clear_flag(ds3231_alarm_t alarm);

/**
 * @brief     enable the alarm
 * @param[in] alarm alarm number
 * @return    status code
 *            - 0 success
 *            - 1 enable alarm failed
 * @note      none
 */
uint8_t ds3231_alarm_enable(ds3231_alarm_t alarm);

/**
 * @brief     disable the alarm
 * @param[in] alarm alarm number
 * @return    status code
 *            - 0 success
 *            - 1 disable alarm failed
 * @note      none
 */
uint8_t ds3231_alarm_disable(ds3231_alarm_t alarm);

/**
 * @brief     alarm example set the time
 * @param[in] *t pointer to a time structure
 * @return    status code
 *            - 0 success
 *            - 1 set time failed
 * @note      none
 */
uint8_t ds3231_alarm_set_time(ds3231_time_t *t);

/**
 * @brief      alarm example get the time
 * @param[out] *t pointer to a time structure
 * @return     status code
 *             - 0 success
 *             - 1 get time failed
 * @note       none
 */
uint8_t ds3231_alarm_get_time(ds3231_time_t *t);

/**
 * @brief     alarm example set the time by a unix timestamp
 * @param[in] timestamp unix timestamp
 * @return    status code
 *            - 0 success
 *            - 1 set timestamp failed
 * @note      none
 */
uint8_t ds3231_alarm_set_timestamp(time_t timestamp);

/**
 * @brief      alarm example get the time in a unix timestamp
 * @param[out] *timestamp pointer to a unix timestamp buffer
 * @return     status code
 *             - 0 success
 *             - 1 get timestamp failed
 * @note       none
 */
uint8_t ds3231_alarm_get_timestamp(time_t *timestamp);

/**
 * @brief     alarm example set the local time zone
 * @param[in] zone local time zone
 * @return    status code
 *            - 0 success
 * @note      none
 */
uint8_t ds3231_alarm_set_timestamp_time_zone(int8_t zone);

/**
 * @brief      alarm example get the local time zone
 * @param[out] *zone pointer to a local time zone buffer
 * @return     status code
 *             - 0 success
 * @note       none
 */
uint8_t ds3231_alarm_get_timestamp_time_zone(int8_t *zone);

/**
 * @brief      alarm example get the current temperature
 * @param[out] *raw pointer to a raw temperature buffer
 * @param[out] *s pointer to a converted temperature buffer
 * @return     status code
 *             - 0 success
 *             - 1 read temperature failed
 * @note       none
 */
uint8_t ds3231_alarm_get_temperature(int16_t *raw, float *s);

/**
 * @brief      alarm example get the ascii time
 * @param[out] *buf pointer to an ascii buffer
 * @param[in]  len data length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t ds3231_alarm_alarm_ascii_time(char *buf, uint8_t len);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
