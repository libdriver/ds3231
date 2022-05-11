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
 * @file      driver_ds3231_output.c
 * @brief     driver ds3231 output source file
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

#include "driver_ds3231_output.h"

static ds3231_handle_t gs_handle;        /**< ds3231 handle */
static int8_t gs_time_zone = 0;          /**< local zone */

/**
 * @brief  output example init
 * @return status code
 *         - 0 success
 *         - 1 init failed
 * @note   none
 */
uint8_t ds3231_output_init(void)
{
    uint8_t res;
    int8_t reg;
    
    /* link functions */
    DRIVER_DS3231_LINK_INIT(&gs_handle, ds3231_handle_t);
    DRIVER_DS3231_LINK_IIC_INIT(&gs_handle, ds3231_interface_iic_init);
    DRIVER_DS3231_LINK_IIC_DEINIT(&gs_handle, ds3231_interface_iic_deinit);
    DRIVER_DS3231_LINK_IIC_READ(&gs_handle, ds3231_interface_iic_read);
    DRIVER_DS3231_LINK_IIC_WRITE(&gs_handle, ds3231_interface_iic_write);
    DRIVER_DS3231_LINK_DELAY_MS(&gs_handle, ds3231_interface_delay_ms);
    DRIVER_DS3231_LINK_DEBUG_PRINT(&gs_handle, ds3231_interface_debug_print);
    DRIVER_DS3231_LINK_RECEIVE_CALLBACK(&gs_handle, ds3231_interface_receive_callback);

    /* init ds3231 */
    res = ds3231_init(&gs_handle);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: init failed.\n");
        
        return 1;
    }
    
    /* set oscillator */
    res = ds3231_set_oscillator(&gs_handle, DS3231_BOOL_TRUE);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: set oscillator failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable alarm1 */
    res = ds3231_set_alarm_interrupt(&gs_handle, DS3231_ALARM_1, DS3231_BOOL_FALSE);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: set alarm1 interrupt failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable alarm2 */
    res = ds3231_set_alarm_interrupt(&gs_handle, DS3231_ALARM_2, DS3231_BOOL_FALSE);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: set alarm2 interrupt failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set square wave */
    res = ds3231_set_pin(&gs_handle, DS3231_PIN_SQUARE_WAVE);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: set pin failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    
    /* convert to register */
    res = ds3231_aging_offset_convert_to_register(&gs_handle, DS3231_OUTPUT_DEFAULT_AGING_OFFSET, (int8_t *)&reg);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: convert to register failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set aging offset */
    res = ds3231_set_aging_offset(&gs_handle, reg);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: set aging offset failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    
    return 0;
}

/**
 * @brief  output example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t ds3231_output_deinit(void)
{
    if (ds3231_deinit(&gs_handle) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief     output example set the time
 * @param[in] *t points to a time structure
 * @return    status code
 *            - 0 success
 *            - 1 set time failed
 * @note      none
 */
uint8_t ds3231_output_set_time(ds3231_time_t *t)
{
    /* set time */
    if (ds3231_set_time(&gs_handle, t) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief     output example set the time by a unix timestamp
 * @param[in] timestamp is a unix timestamp
 * @return    status code
 *            - 0 success
 *            - 1 set timestamp failed
 * @note      none
 */
uint8_t ds3231_output_set_timestamp(time_t timestamp)
{
    ds3231_time_t t;
    struct tm *timeptr;
    
    /* convert times */
    timestamp += (time_t)(gs_time_zone * 3600);
    timeptr = localtime(&timestamp);
    t.am_pm = DS3231_AM;
    t.date = (uint8_t)timeptr->tm_mday;
    t.format = DS3231_FORMAT_24H;
    t.hour = (uint8_t)timeptr->tm_hour;
    t.minute = (uint8_t)timeptr->tm_min;
    t.month = (uint8_t)timeptr->tm_mon + 1;
    t.second = (uint8_t)timeptr->tm_sec;
    if (timeptr->tm_wday == 0)
    {
        t.week  = 7;
    }
    else
    {
        t.week = (uint8_t)timeptr->tm_wday;
    }
    t.year = (uint16_t)(timeptr->tm_year + 1900);
    
    /* set time */
    if (ds3231_set_time(&gs_handle, &t) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief     output example set the local time zone
 * @param[in] zone is the local time zone
 * @return    status code
 *            - 0 success
 * @note      none
 */
uint8_t ds3231_output_set_timestamp_time_zone(int8_t zone)
{
    gs_time_zone = zone;
    
    return 0;
}

/**
 * @brief      output example get the time
 * @param[out] *t points to a time structure
 * @return     status code
 *             - 0 success
 *             - 1 get time failed
 * @note       none
 */
uint8_t ds3231_output_get_time(ds3231_time_t *t)
{
    /* get time */
    if (ds3231_get_time(&gs_handle, t) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief      output example get the time in a unix timestamp
 * @param[out] *timestamp points to a unix timestamp buffer
 * @return     status code
 *             - 0 success
 *             - 1 get timestamp failed
 * @note       none
 */
uint8_t ds3231_output_get_timestamp(time_t *timestamp)
{
    ds3231_time_t t;
    struct tm timeptr;
    
    /* get time */
    if (ds3231_get_time(&gs_handle, &t) != 0)
    {
        return 1;
    }
    timeptr.tm_year = t.year - 1900;
    timeptr.tm_mon = t.month - 1;
    timeptr.tm_wday = t.week; 
    timeptr.tm_mday = t.date;
    if (t.format == DS3231_FORMAT_24H)
    {
        timeptr.tm_hour = t.hour;
    }
    else
    {
        timeptr.tm_hour = t.hour % 12 + t.am_pm * 12;
    }
    timeptr.tm_min = t.minute;
    timeptr.tm_sec = t.second;
    
    /* make time */
    *timestamp = mktime(&timeptr) - gs_time_zone * 3600;
    
    return 0;
}

/**
 * @brief      output example get the local time zone
 * @param[out] *zone points to a local time zone buffer
 * @return     status code
 *             - 0 success
 * @note       none
 */
uint8_t ds3231_output_get_timestamp_time_zone(int8_t *zone)
{
    *zone = gs_time_zone;
    
    return 0;
}

/**
 * @brief      output example get the current temperature
 * @param[out] *raw points to a raw temperature buffer
 * @param[out] *s points to a converted temperature buffer
 * @return     status code
 *             - 0 success
 *             - 1 read temperature failed
 * @note       none
 */
uint8_t ds3231_output_get_temperature(int16_t *raw, float *s)
{
    /* get temperature */
    if (ds3231_get_temperature(&gs_handle, raw, s) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief      output example get the ascii time
 * @param[out] *buf points to an ascii buffer
 * @param[in]  len is the data length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t ds3231_output_get_ascii_time(char *buf, uint8_t len)
{
    ds3231_time_t t;
    
    /* get time */
    if (ds3231_get_time(&gs_handle, &t) != 0)
    {
        return 1;
    }
    
    if (t.format == DS3231_FORMAT_24H)
    {
        (void)snprintf(buf, len, "%04d-%02d-%02d %02d:%02d:%02d %d.\n", t.year, t.month, t.date, t.hour, t.minute, t.second, t.week);
    }
    else
    {
        (void)snprintf(buf, len, "%04d-%02d-%02d %s %02d:%02d:%02d %d.\n", t.year, t.month, t.date, (t.am_pm == DS3231_AM) ? "AM" : "PM",
                       t.hour, t.minute, t.second, t.week
                      );
    }
    
    return 0;
}

/**
 * @brief     output example enable or disable the square wave
 * @param[in] enable is a bool value
 * @return    status code
 *             - 0 success
 *             - 1 set square wave failed
 * @note      none
 */
uint8_t ds3231_output_set_square_wave(ds3231_bool_t enable)
{
    /* set square wave */
    if (ds3231_set_square_wave(&gs_handle, enable) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief      output example get the square wave status
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *              - 0 success
 *              - 1 get square wave failed
 * @note       none
 */
uint8_t ds3231_output_get_square_wave(ds3231_bool_t *enable)
{
    /* get square wave */
    if (ds3231_get_square_wave(&gs_handle, enable) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief     output example enable or disable the 32KHz output
 * @param[in] enable is a bool value
 * @return    status code
 *             - 0 success
 *             - 1 set 32khz output failed
 * @note      none
 */
uint8_t ds3231_output_set_32khz_output(ds3231_bool_t enable)
{
    /* set 32khz_output */
    if (ds3231_set_32khz_output(&gs_handle, enable) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief      output example get the 32KHz output status
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *              - 0 success
 *              - 1 get 32KHz output failed
 * @note       none
 */
uint8_t ds3231_output_get_32khz_output(ds3231_bool_t *enable)
{
    /* get 32khz_output */
    if (ds3231_get_32khz_output(&gs_handle, enable) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
