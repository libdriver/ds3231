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
 * @file      driver_ds3231_alarm.c
 * @brief     driver ds3231 alarm source file
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

#include "driver_ds3231_alarm.h"

static ds3231_handle_t gs_handle;        /**< ds3231 handle */
static int8_t gs_time_zone = 0;          /**< local zone */

/**
 * @brief  alarm irq handler
 * @return status code
 *         - 0 success
 *         - 1 run failed
 * @note   none
 */
uint8_t ds3231_alarm_irq_handler(void)
{
    /* irq handler */
    if (ds3231_irq_handler(&gs_handle))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief     alarm example init
 * @param[in] *alarm_receive_callback points to a alarm receive callback function callback
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t ds3231_alarm_init(uint8_t (*alarm_receive_callback)(uint8_t type))
{
    volatile uint8_t res;
    volatile int8_t reg;
    
    /* link functions */
    DRIVER_DS3231_LINK_INIT(&gs_handle, ds3231_handle_t);
    DRIVER_DS3231_LINK_IIC_INIT(&gs_handle, ds3231_interface_iic_init);
    DRIVER_DS3231_LINK_IIC_DEINIT(&gs_handle, ds3231_interface_iic_deinit);
    DRIVER_DS3231_LINK_IIC_READ(&gs_handle, ds3231_interface_iic_read);
    DRIVER_DS3231_LINK_IIC_WRITE(&gs_handle, ds3231_interface_iic_write);
    DRIVER_DS3231_LINK_DELAY_MS(&gs_handle, ds3231_interface_delay_ms);
    DRIVER_DS3231_LINK_DEBUG_PRINT(&gs_handle, ds3231_interface_debug_print);
    DRIVER_DS3231_LINK_RECEIVE_CALLBACK(&gs_handle, alarm_receive_callback);

    /* init ds3231 */
    res = ds3231_init(&gs_handle);
    if (res)
    {
        ds3231_interface_debug_print("ds3231: init failed.\n");
        
        return 1;
    }
    
    /* set oscillator */
    res = ds3231_set_oscillator(&gs_handle, DS3231_BOOL_TRUE);
    if (res)
    {
        ds3231_interface_debug_print("ds3231: set oscillator failed.\n");
        ds3231_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set interrupt */
    res = ds3231_set_pin(&gs_handle, DS3231_PIN_INTERRUPT);
    if (res)
    {
        ds3231_interface_debug_print("ds3231: set pin failed.\n");
        ds3231_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable square wave */
    res = ds3231_set_square_wave(&gs_handle, DS3231_BOOL_FALSE);
    if (res)
    {
        ds3231_interface_debug_print("ds3231: set square wave failed.\n");
        ds3231_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable 32khz output */
    res = ds3231_set_32khz_output(&gs_handle, DS3231_BOOL_FALSE);
    if (res)
    {
        ds3231_interface_debug_print("ds3231: set 32khz output failed.\n");
        ds3231_deinit(&gs_handle);
        
        return 1;
    }
    
    /* convert to register */
    res = ds3231_aging_offset_convert_to_register(&gs_handle, DS3231_ALARM_DEFAULT_AGING_OFFSET, (int8_t *)&reg);
    if (res)
    {
        ds3231_interface_debug_print("ds3231: convert to register failed.\n");
        ds3231_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set aging offset */
    res = ds3231_set_aging_offset(&gs_handle, reg);
    if (res)
    {
        ds3231_interface_debug_print("ds3231: set aging offset failed.\n");
        ds3231_deinit(&gs_handle);
        
        return 1;
    }
    
    return 0;
}

/**
 * @brief  alarm example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t ds3231_alarm_deinit(void)
{
    if (ds3231_deinit(&gs_handle))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief     alarm example set the alarm 1
 * @param[in] *time points to a time structure
 * @param[in] mode is the alarm 1 interrupt mode
 * @return    status code
 *            - 0 success
 *            - 1 set alarm1 failed
 * @note      none
 */
uint8_t ds3231_alarm_set_alarm1(ds3231_time_t *time, ds3231_alarm1_mode_t mode)
{
    /* set alarm1 */
    if (ds3231_set_alarm1(&gs_handle, time, mode))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief      alarm example get the alarm 1
 * @param[out] *time points to a time structure
 * @param[out] *mode points to a alarm 1 interrupt mode buffer
 * @return     status code
 *             - 0 success
 *             - 1 get alarm1 failed
 * @note       none
 */
uint8_t ds3231_alarm_get_alarm1(ds3231_time_t *time, ds3231_alarm1_mode_t *mode)
{
    /* get alarm1 */
    if (ds3231_get_alarm1(&gs_handle, time, mode))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief     alarm example set the alarm 2
 * @param[in] *time points to a time structure
 * @param[in] mode is the alarm 2 interrupt mode
 * @return    status code
 *            - 0 success
 *            - 1 set alarm2 failed
 * @note      none
 */
uint8_t ds3231_alarm_set_alarm2(ds3231_time_t *time, ds3231_alarm2_mode_t mode)
{
    /* set alarm2 */
    if (ds3231_set_alarm2(&gs_handle, time, mode))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief      alarm example get the alarm 2
 * @param[out] *time points to a time structure
 * @param[out] *mode points to a alarm 2 interrupt mode buffer
 * @return     status code
 *             - 0 success
 *             - 1 get alarm2 failed
 * @note       none
 */
uint8_t ds3231_alarm_get_alarm2(ds3231_time_t *time, ds3231_alarm2_mode_t *mode)
{
    /* get alarm2 */
    if (ds3231_get_alarm2(&gs_handle, time, mode))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief     alarm clear the interrupt flag
 * @param[in] alarm is the alarm number
 * @return    status code
 *            - 0 success
 *            - 1 alarm clear failed
 * @note      none
 */
uint8_t ds3231_alarm_clear_flag(ds3231_alarm_t alarm)
{
    /* alarm clear */
    if (ds3231_alarm_clear(&gs_handle, alarm))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief     enable the alarm
 * @param[in] alarm is the alarm number
 * @return    status code
 *            - 0 success
 *            - 1 enable alarm failed
 * @note      none
 */
uint8_t ds3231_alarm_enable(ds3231_alarm_t alarm)
{
    /* clear alarm flag */
    if (ds3231_alarm_clear(&gs_handle, alarm))
    {
        return 1;
    }
    
    /* enable alarm */
    if (ds3231_set_alarm_interrupt(&gs_handle, alarm, DS3231_BOOL_TRUE))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief     disable the alarm
 * @param[in] alarm is the alarm number
 * @return    status code
 *            - 0 success
 *            - 1 disable alarm failed
 * @note      none
 */
uint8_t ds3231_alarm_disable(ds3231_alarm_t alarm)
{
    /* disable alarm */
    if (ds3231_set_alarm_interrupt(&gs_handle, alarm, DS3231_BOOL_FALSE))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief     alarm example set the time
 * @param[in] *time points to a time structure
 * @return    status code
 *            - 0 success
 *            - 1 set time failed
 * @note      none
 */
uint8_t ds3231_alarm_set_time(ds3231_time_t *time)
{
    /* set time */
    if (ds3231_set_time(&gs_handle, time))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief     alarm example set the time by a unix timestamp
 * @param[in] timestamp is a unix timestamp
 * @return    status code
 *            - 0 success
 *            - 1 set timestamp failed
 * @note      none
 */
uint8_t ds3231_alarm_set_timestamp(time_t timestamp)
{
    ds3231_time_t time;
    struct tm *timeptr;
    
    /* convert times */
    timestamp += gs_time_zone * 3600;
    timeptr = localtime(&timestamp);
    time.am_pm = DS3231_AM;
    time.date = timeptr->tm_mday;
    time.format = DS3231_FORMAT_24H;
    time.hour = timeptr->tm_hour;
    time.minute = timeptr->tm_min;
    time.month = timeptr->tm_mon + 1;
    time.second = timeptr->tm_sec;
    if (timeptr->tm_wday == 0)
    {
        time.week  = 7;
    }
    else
    {
        time.week = timeptr->tm_wday;
    }
    time.year = timeptr->tm_year + 1900;
    
    /* set time */
    if (ds3231_set_time(&gs_handle, &time))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief     alarm example set the local time zone
 * @param[in] zone is the local time zone
 * @return    status code
 *            - 0 success
 * @note      none
 */
uint8_t ds3231_alarm_set_timestamp_time_zone(int8_t zone)
{
    gs_time_zone = zone;
    
    return 0;
}

/**
 * @brief      alarm example get the time
 * @param[out] *time points to a time structure
 * @return     status code
 *             - 0 success
 *             - 1 get time failed
 * @note       none
 */
uint8_t ds3231_alarm_get_time(ds3231_time_t *time)
{
    /* get time */
    if (ds3231_get_time(&gs_handle, time))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief      alarm example get the time in a unix timestamp
 * @param[out] *timestamp points to a unix timestamp buffer
 * @return     status code
 *             - 0 success
 *             - 1 get timestamp failed
 * @note       none
 */
uint8_t ds3231_alarm_get_timestamp(time_t *timestamp)
{
    ds3231_time_t time;
    struct tm timeptr;
    
    /* get time */
    if (ds3231_get_time(&gs_handle, &time))
    {
        return 1;
    }
    timeptr.tm_year = time.year - 1900;
    timeptr.tm_mon = time.month - 1;
    timeptr.tm_wday = time.week; 
    timeptr.tm_mday = time.date;
    if (time.format == DS3231_FORMAT_24H)
    {
        timeptr.tm_hour = time.hour;
    }
    else
    {
        timeptr.tm_hour = time.hour % 12 + time.am_pm * 12;
    }
    timeptr.tm_min = time.minute;
    timeptr.tm_sec = time.second;
    
    /* make time */
    *timestamp = mktime(&timeptr) - gs_time_zone * 3600;
    
    return 0;
}

/**
 * @brief      alarm example get the local time zone
 * @param[out] *zone points to a local time zone buffer
 * @return     status code
 *             - 0 success
 * @note       none
 */
uint8_t ds3231_alarm_get_timestamp_time_zone(int8_t *zone)
{
    *zone = gs_time_zone;
    
    return 0;
}

/**
 * @brief      alarm example get the current temperature
 * @param[out] *raw points to a raw temperature buffer
 * @param[out] *s points to a converted temperature buffer
 * @return     status code
 *             - 0 success
 *             - 1 read temperature failed
 * @note       none
 */
uint8_t ds3231_alarm_get_temperature(int16_t *raw, float *s)
{
    /* get temperature */
    if (ds3231_get_temperature(&gs_handle, raw, s))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief  alarm example get the ascii time
 * @return points to a ascii time buffer
 * @note   none
 */
char *ds3231_alarm_alarm_ascii_time(void)
{
    ds3231_time_t time;
    struct tm timeptr;
    
    /* get time */
    if (ds3231_get_time(&gs_handle, &time))
    {
        return NULL;
    }
    timeptr.tm_year = time.year - 1900;
    timeptr.tm_mon = time.month - 1;
    timeptr.tm_wday = time.week; 
    timeptr.tm_mday = time.date;
    if (time.format == DS3231_FORMAT_24H)
    {
        timeptr.tm_hour = time.hour;
    }
    else
    {
        timeptr.tm_hour = time.hour % 12 + time.am_pm * 12;
    }
    timeptr.tm_min = time.minute;
    timeptr.tm_sec = time.second;
    
    /* convert to ascii time */
    return asctime((const struct tm *)&timeptr);
}
