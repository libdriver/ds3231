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
 * @file      driver_ds3231.h
 * @brief     driver ds3231 header file
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

#ifndef DRIVER_DS3231_H
#define DRIVER_DS3231_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup ds3231_driver ds3231 driver function
 * @brief    ds3231 driver modules
 * @{
 */

/**
 * @addtogroup ds3231_base_driver
 * @{
 */

/**
 * @brief ds3231 bool enumeration definition
 */
typedef enum
{
    DS3231_BOOL_FALSE = 0x00,        /**< disable function */
    DS3231_BOOL_TRUE  = 0x01,        /**< enable function */
} ds3231_bool_t;

/**
 * @brief ds3231 alarm enumeration definition
 */
typedef enum
{
    DS3231_ALARM_1 = 0x00,        /**< alarm 1 */
    DS3231_ALARM_2 = 0x01,        /**< alarm 2 */
} ds3231_alarm_t;

/**
 * @brief ds3231 am pm enumeration definition
 */
typedef enum
{
    DS3231_AM = 0x00,        /**< am */
    DS3231_PM = 0x01,        /**< pm */
} ds3231_am_pm_t;

/**
 * @brief ds3231 pin enumeration definition
 */
typedef enum
{
    DS3231_PIN_SQUARE_WAVE = 0x00,        /**< square wave pin */
    DS3231_PIN_INTERRUPT   = 0x01,        /**< interrupt pin */
} ds3231_pin_t;

/**
 * @brief ds3231 format enumeration definition
 */
typedef enum
{
    DS3231_FORMAT_12H = 0x01,        /**< 12h format */
    DS3231_FORMAT_24H = 0x00,        /**< 24h format */
} ds3231_format_t;

/**
 * @brief ds3231 alarm1 enumeration definition
 */
typedef enum
{
    DS3231_STATUS_ALARM_2 = (1 << 1),        /**< alarm 2 status */
    DS3231_STATUS_ALARM_1 = (1 << 0),        /**< alarm 1 status */
} ds3231_status_t;

/**
 * @}
 */

/**
 * @addtogroup ds3231_alarm_driver
 * @{
 */

/**
 * @brief ds3231 alarm1 enumeration definition
 */
typedef enum
{
    DS3231_ALARM1_MODE_ONCE_A_SECOND                 = 0x0F,        /**< interrupt once a second */
    DS3231_ALARM1_MODE_SECOND_MATCH                  = 0x0E,        /**< interrupt second match */
    DS3231_ALARM1_MODE_MINUTE_SECOND_MATCH           = 0x0C,        /**< interrupt minute second match */
    DS3231_ALARM1_MODE_HOUR_MINUTE_SECOND_MATCH      = 0x08,        /**< interrupt hour minute second match */
    DS3231_ALARM1_MODE_DATE_HOUR_MINUTE_SECOND_MATCH = 0x00,        /**< interrupt date hour minute second match */
    DS3231_ALARM1_MODE_WEEK_HOUR_MINUTE_SECOND_MATCH = 0x10,        /**< interrupt week hour minute second match */
} ds3231_alarm1_mode_t;

/**
 * @brief ds3231 alarm2 enumeration definition
 */
typedef enum
{
    DS3231_ALARM2_MODE_ONCE_A_MINUTE          = 0x07,        /**< interrupt once a minute */
    DS3231_ALARM2_MODE_MINUTE_MATCH           = 0x06,        /**< interrupt minute match */
    DS3231_ALARM2_MODE_HOUR_MINUTE_MATCH      = 0x04,        /**< interrupt hour minute match */
    DS3231_ALARM2_MODE_DATE_HOUR_MINUTE_MATCH = 0x00,        /**< interrupt data hour minute match */
    DS3231_ALARM2_MODE_WEEK_HOUR_MINUTE_MATCH = 0x10,        /**< interrupt week hour minute match */
} ds3231_alarm2_mode_t;

/**
 * @}
 */

/**
 * @addtogroup ds3231_base_driver
 * @{
 */

/**
 * @brief ds3231 time structure definition
 */
typedef struct ds3231_time_s
{
    uint16_t year;                 /**< year */
    uint8_t month;                 /**< month */
    uint8_t week;                  /**< week */
    uint8_t date;                  /**< date */
    uint8_t hour;                  /**< hour */
    uint8_t minute;                /**< minute */
    uint8_t second;                /**< second */
    ds3231_format_t format;        /**< data format */
    ds3231_am_pm_t am_pm;          /**< am pm */
} ds3231_time_t;

/**
 * @brief ds3231 handle structure definition
 */
typedef struct ds3231_handle_s
{
    uint8_t (*iic_init)(void);                                                          /**< point to an iic_init function address */
    uint8_t (*iic_deinit)(void);                                                        /**< point to an iic_deinit function address */
    uint8_t (*iic_write)(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len);        /**< point to an iic_write function address */
    uint8_t (*iic_read)(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len);         /**< point to an iic_read function address */
    void (*debug_print)(const char *const fmt, ...);                                    /**< point to a debug_print function address */
    void (*receive_callback)(uint8_t type);                                             /**< point to a receive_callback function address */
    void (*delay_ms)(uint32_t ms);                                                      /**< point to a delay_ms function address */
    uint8_t inited;                                                                     /**< inited flag */
} ds3231_handle_t;

/**
 * @brief ds3231 information structure definition
 */
typedef struct ds3231_info_s
{
    char chip_name[32];                /**< chip name */
    char manufacturer_name[32];        /**< manufacturer name */
    char interface[8];                 /**< chip interface name */
    float supply_voltage_min_v;        /**< chip min supply voltage */
    float supply_voltage_max_v;        /**< chip max supply voltage */
    float max_current_ma;              /**< chip max current */
    float temperature_min;             /**< chip min operating temperature */
    float temperature_max;             /**< chip max operating temperature */
    uint32_t driver_version;           /**< driver version */
} ds3231_info_t;

/**
 * @}
 */

/**
 * @defgroup ds3231_link_driver ds3231 link driver function
 * @brief    ds3231 link driver modules
 * @ingroup  ds3231_driver
 * @{
 */

/**
 * @brief     initialize ds3231_handle_t structure
 * @param[in] HANDLE pointer to a ds3231 handle structure
 * @param[in] STRUCTURE ds3231_handle_t
 * @note      none
 */
#define DRIVER_DS3231_LINK_INIT(HANDLE, STRUCTURE)         memset(HANDLE, 0, sizeof(STRUCTURE))

/**
 * @brief     link iic_init function
 * @param[in] HANDLE pointer to a ds3231 handle structure
 * @param[in] FUC pointer to an iic_init function address
 * @note      none
 */
#define DRIVER_DS3231_LINK_IIC_INIT(HANDLE, FUC)          (HANDLE)->iic_init = FUC

/**
 * @brief     link iic_deinit function
 * @param[in] HANDLE pointer to a ds3231 handle structure
 * @param[in] FUC pointer to an iic_deinit function address
 * @note      none
 */
#define DRIVER_DS3231_LINK_IIC_DEINIT(HANDLE, FUC)        (HANDLE)->iic_deinit = FUC

/**
 * @brief     link iic_read function
 * @param[in] HANDLE pointer to a ds3231 handle structure
 * @param[in] FUC pointer to an iic_read function address
 * @note      none
 */
#define DRIVER_DS3231_LINK_IIC_READ(HANDLE, FUC)          (HANDLE)->iic_read = FUC

/**
 * @brief     link iic_write function
 * @param[in] HANDLE pointer to a ds3231 handle structure
 * @param[in] FUC pointer to an iic_write function address
 * @note      none
 */
#define DRIVER_DS3231_LINK_IIC_WRITE(HANDLE, FUC)         (HANDLE)->iic_write = FUC

/**
 * @brief     link delay_ms function
 * @param[in] HANDLE pointer to a ds3231 handle structure
 * @param[in] FUC pointer to a delay_ms function address
 * @note      none
 */
#define DRIVER_DS3231_LINK_DELAY_MS(HANDLE, FUC)          (HANDLE)->delay_ms = FUC

/**
 * @brief     link debug_print function
 * @param[in] HANDLE pointer to a ds3231 handle structure
 * @param[in] FUC pointer to a debug_print function address
 * @note      none
 */
#define DRIVER_DS3231_LINK_DEBUG_PRINT(HANDLE, FUC)       (HANDLE)->debug_print = FUC

/**
 * @brief     link receive_callback function
 * @param[in] HANDLE pointer to a ds3231 handle structure
 * @param[in] FUC pointer to a receive_callback function address
 * @note      none
 */
#define DRIVER_DS3231_LINK_RECEIVE_CALLBACK(HANDLE, FUC)  (HANDLE)->receive_callback = FUC

/**
 * @}
 */

/**
 * @defgroup ds3231_base_driver ds3231 base driver function
 * @brief    ds3231 base driver modules
 * @ingroup  ds3231_driver
 * @{
 */

/**
 * @brief      get chip's information
 * @param[out] *info pointer to a ds3231 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t ds3231_info(ds3231_info_t *info);

/**
 * @brief     initialize the chip
 * @param[in] *handle pointer to a ds3231 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 iic initialization failed
 *            - 2 handle is NULL
 *            - 3 linked functions is NULL
 * @note      none
 */
uint8_t ds3231_init(ds3231_handle_t *handle);

/**
 * @brief     close the chip
 * @param[in] *handle pointer to a ds3231 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 iic deinit failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ds3231_deinit(ds3231_handle_t *handle);

/**
 * @brief     irq handler
 * @param[in] *handle pointer to a ds3231 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ds3231_irq_handler(ds3231_handle_t *handle);

/**
 * @brief     set the current time
 * @param[in] *handle pointer to a ds3231 handle structure
 * @param[in] *t pointer to a time structure
 * @return    status code
 *            - 0 success
 *            - 1 set time failed
 *            - 2 handle or time is NULL
 *            - 3 handle is not initialized
 *            - 4 time is invalid
 * @note      none
 */
uint8_t ds3231_set_time(ds3231_handle_t *handle, ds3231_time_t *t);

/**
 * @brief      get the current time
 * @param[in]  *handle pointer to a ds3231 handle structure
 * @param[out] *t pointer to a time structure
 * @return     status code
 *             - 0 success
 *             - 1 get time failed
 *             - 2 handle or time is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ds3231_get_time(ds3231_handle_t *handle, ds3231_time_t *t);

/**
 * @brief     enable or disable the oscillator
 * @param[in] *handle pointer to a ds3231 handle structure
 * @param[in] enable bool value
 * @return    status code
 *            - 0 success
 *            - 1 set oscillator failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ds3231_set_oscillator(ds3231_handle_t *handle, ds3231_bool_t enable);

/**
 * @brief      get the chip oscillator status
 * @param[in]  *handle pointer to a ds3231 handle structure
 * @param[out] *enable pointer to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get oscillator failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ds3231_get_oscillator(ds3231_handle_t *handle, ds3231_bool_t *enable);

/**
 * @brief      get the chip status
 * @param[in]  *handle pointer to a ds3231 handle structure
 * @param[out] *status pointer to a chip status buffer
 * @return     status code
 *             - 0 success
 *             - 1 get status failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ds3231_get_status(ds3231_handle_t *handle, uint8_t *status);

/**
 * @}
 */

/**
 * @defgroup ds3231_advance_driver ds3231 advance driver function
 * @brief    ds3231 advance driver modules
 * @ingroup  ds3231_driver
 * @{
 */

/**
 * @brief     set the chip pin function
 * @param[in] *handle pointer to a ds3231 handle structure
 * @param[in] pin pin function
 * @return    status code
 *            - 0 success
 *            - 1 set pin failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ds3231_set_pin(ds3231_handle_t *handle, ds3231_pin_t pin);

/**
 * @brief      get the chip pin function
 * @param[in]  *handle pointer to a ds3231 handle structure
 * @param[out] *pin pointer to a pin's function buffer
 * @return     status code
 *             - 0 success
 *             - 1 get pin failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ds3231_get_pin(ds3231_handle_t *handle, ds3231_pin_t *pin);

/**
 * @brief     enable or disable the square wave output
 * @param[in] *handle pointer to a ds3231 handle structure
 * @param[in] enable bool value
 * @return    status code
 *            - 0 success
 *            - 1 set square wave failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ds3231_set_square_wave(ds3231_handle_t *handle, ds3231_bool_t enable);

/**
 * @brief      get the square wave output status
 * @param[in]  *handle pointer to a ds3231 handle structure
 * @param[out] *enable pointer to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get square wave failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ds3231_get_square_wave(ds3231_handle_t *handle, ds3231_bool_t *enable);

/**
 * @brief     enable or disable the 32KHz output
 * @param[in] *handle pointer to a ds3231 handle structure
 * @param[in] enable bool value
 * @return    status code
 *            - 0 success
 *            - 1 set 32khz output failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ds3231_set_32khz_output(ds3231_handle_t *handle, ds3231_bool_t enable);

/**
 * @brief      get the 32KHz output status
 * @param[in]  *handle pointer to a ds3231 handle structure
 * @param[out] *enable pointer to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get 32khz output failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ds3231_get_32khz_output(ds3231_handle_t *handle, ds3231_bool_t *enable);

/**
 * @brief      get the chip temperature
 * @param[in]  *handle pointer to a ds3231 handle structure
 * @param[out] *raw pointer to a raw temperature buffer
 * @param[out] *s pointer to a converted temperature buffer
 * @return     status code
 *             - 0 success
 *             - 1 get temperature failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ds3231_get_temperature(ds3231_handle_t *handle, int16_t *raw, float *s);

/**
 * @brief     set the chip aging offset
 * @param[in] *handle pointer to a ds3231 handle structure
 * @param[in] offset time aging offset
 * @return    status code
 *            - 0 success
 *            - 1 set aging offset failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ds3231_set_aging_offset(ds3231_handle_t *handle, int8_t offset);

/**
 * @brief      get the chip aging offset
 * @param[in]  *handle pointer to a ds3231 handle structure
 * @param[out] *offset pointer to a time aging offset buffer
 * @return     status code
 *             - 0 success
 *             - 1 get aging offset failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ds3231_get_aging_offset(ds3231_handle_t *handle, int8_t *offset);

/**
 * @brief      convert a aging offset value to a register raw data
 * @param[in]  *handle pointer to a ds3231 handle structure
 * @param[in]  offset converted aging offset value
 * @param[out] *reg pointer to a register raw buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ds3231_aging_offset_convert_to_register(ds3231_handle_t *handle, float offset, int8_t *reg);

/**
 * @brief      convert a register raw data to a converted aging offset data
 * @param[in]  *handle pointer to a ds3231 handle structure
 * @param[in]  reg register raw data
 * @param[out] *offset pointer to a converted aging offset buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ds3231_aging_offset_convert_to_data(ds3231_handle_t *handle, int8_t reg, float *offset);

/**
 * @}
 */

/**
 * @defgroup ds3231_alarm_driver ds3231 alarm driver function
 * @brief    ds3231 alarm driver modules
 * @ingroup  ds3231_driver
 * @{
 */

/**
 * @brief     enable or disable the alarm interrupt
 * @param[in] *handle pointer to a ds3231 handle structure
 * @param[in] alarm alarm number
 * @param[in] enable bool value
 * @return    status code
 *            - 0 success
 *            - 1 set alarm interrupt failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ds3231_set_alarm_interrupt(ds3231_handle_t *handle, ds3231_alarm_t alarm, ds3231_bool_t enable);

/**
 * @brief      get the alarm interrupt status
 * @param[in]  *handle pointer to a ds3231 handle structure
 * @param[in]  alarm alarm number
 * @param[out] *enable pointer to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get alarm interrupt failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ds3231_get_alarm_interrupt(ds3231_handle_t *handle, ds3231_alarm_t alarm, ds3231_bool_t *enable);

/**
 * @brief     set the alarm1 time
 * @param[in] *handle pointer to a ds3231 handle structure
 * @param[in] *t pointer to a time structure
 * @param[in] mode alarm1 interrupt mode
 * @return    status code
 *            - 0 success
 *            - 1 set alarm1 failed
 *            - 2 handle or time is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ds3231_set_alarm1(ds3231_handle_t *handle, ds3231_time_t *t, ds3231_alarm1_mode_t mode);

/**
 * @brief      get the alarm1 time
 * @param[in]  *handle pointer to a ds3231 handle structure
 * @param[out] *t pointer to a time structure
 * @param[out] *mode pointer to an alarm1 interrupt mode buffer
 * @return     status code
 *             - 0 success
 *             - 1 get alarm1 failed
 *             - 2 handle or time is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ds3231_get_alarm1(ds3231_handle_t *handle, ds3231_time_t *t, ds3231_alarm1_mode_t *mode);

/**
 * @brief     set the alarm2 time
 * @param[in] *handle pointer to a ds3231 handle structure
 * @param[in] *t pointer to a time structure
 * @param[in] mode alarm2 interrupt mode
 * @return    status code
 *            - 0 success
 *            - 1 set alarm2 failed
 *            - 2 handle or time is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ds3231_set_alarm2(ds3231_handle_t *handle, ds3231_time_t *t, ds3231_alarm2_mode_t mode);

/**
 * @brief      get the alarm2 time
 * @param[in]  *handle pointer to a ds3231 handle structure
 * @param[out] *t pointer to a time structure
 * @param[out] *mode pointer to an alarm2 interrupt mode buffer
 * @return     status code
 *             - 0 success
 *             - 1 get alarm2 failed
 *             - 2 handle or time is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ds3231_get_alarm2(ds3231_handle_t *handle, ds3231_time_t *t, ds3231_alarm2_mode_t *mode);

/**
 * @brief     clear the alarm flag
 * @param[in] *handle pointer to a ds3231 handle structure
 * @param[in] alarm alarm number
 * @return    status code
 *            - 0 success
 *            - 1 alarm clear failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ds3231_alarm_clear(ds3231_handle_t *handle, ds3231_alarm_t alarm);

/**
 * @}
 */

/**
 * @defgroup ds3231_extern_driver ds3231 extern driver function
 * @brief    ds3231 extern driver modules
 * @ingroup  ds3231_driver
 * @{
 */

/**
 * @brief     set the chip register
 * @param[in] *handle pointer to a ds3231 handle structure
 * @param[in] reg iic register address
 * @param[in] *buf pointer to a data buffer
 * @param[in] len data buffer length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ds3231_set_reg(ds3231_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len);

/**
 * @brief      get the chip register
 * @param[in]  *handle pointer to a ds3231 handle structure
 * @param[in]  reg iic register address
 * @param[out] *buf pointer to a data buffer
 * @param[in]  len data buffer length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ds3231_get_reg(ds3231_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len);

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
