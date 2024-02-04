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
 * @file      driver_ds3231_register_test.c
 * @brief     driver ds3231 register test source file
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

#include "driver_ds3231_register_test.h"
#include <stdlib.h>

static ds3231_handle_t gs_handle;        /**< ds3231 handle */

/**
 * @brief  register test
 * @return status code
 *         - 0 success
 *         - 1 test failed
 * @note   none
 */
uint8_t ds3231_register_test(void)
{
    uint8_t res;
    int8_t offset, offset_check;
    uint8_t status;
    float o, o_check;
    ds3231_info_t info;
    ds3231_time_t time_in, time_out;
    ds3231_alarm1_mode_t mode1;
    ds3231_alarm2_mode_t mode2;
    ds3231_bool_t enable;
    ds3231_pin_t pin;
    
    /* link functions */
    DRIVER_DS3231_LINK_INIT(&gs_handle, ds3231_handle_t);
    DRIVER_DS3231_LINK_IIC_INIT(&gs_handle, ds3231_interface_iic_init);
    DRIVER_DS3231_LINK_IIC_DEINIT(&gs_handle, ds3231_interface_iic_deinit);
    DRIVER_DS3231_LINK_IIC_READ(&gs_handle, ds3231_interface_iic_read);
    DRIVER_DS3231_LINK_IIC_WRITE(&gs_handle, ds3231_interface_iic_write);
    DRIVER_DS3231_LINK_DELAY_MS(&gs_handle, ds3231_interface_delay_ms);
    DRIVER_DS3231_LINK_DEBUG_PRINT(&gs_handle, ds3231_interface_debug_print);
    DRIVER_DS3231_LINK_RECEIVE_CALLBACK(&gs_handle, ds3231_interface_receive_callback);
    
    /* get ds3231 info */
    res = ds3231_info(&info);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: get info failed.\n");
       
        return 1;
    }
    else
    {
        /* print ds3231 info */
        ds3231_interface_debug_print("ds3231: chip is %s.\n", info.chip_name);
        ds3231_interface_debug_print("ds3231: manufacturer is %s.\n", info.manufacturer_name);
        ds3231_interface_debug_print("ds3231: interface is %s.\n", info.interface);
        ds3231_interface_debug_print("ds3231: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        ds3231_interface_debug_print("ds3231: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        ds3231_interface_debug_print("ds3231: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        ds3231_interface_debug_print("ds3231: max current is %0.2fmA.\n", info.max_current_ma);
        ds3231_interface_debug_print("ds3231: max temperature is %0.1fC.\n", info.temperature_max);
        ds3231_interface_debug_print("ds3231: min temperature is %0.1fC.\n", info.temperature_min);
    }
    
    /* start register test */
    ds3231_interface_debug_print("ds3231: start register test.\n");
    
    /* init ds3231 */
    res = ds3231_init(&gs_handle);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: init failed.\n");
        
        return 1;
    }
    
    /* ds3231_set_time/ds3231_get_time test */
    ds3231_interface_debug_print("ds3231: ds3231_set_time/ds3231_get_time test.\n");
    
    /* 12H format */
    time_in.format = DS3231_FORMAT_12H;
    time_in.am_pm = DS3231_PM;
    time_in.year = rand() % 100 + 2000;
    time_in.month = rand() % 12 + 1;
    time_in.date = rand() % 20 + 1;
    time_in.date = rand() % 20 + 1;
    time_in.week = rand() % 7 + 1;
    time_in.hour = rand() % 11 + 1;
    time_in.minute = rand() % 60;
    time_in.second = rand() % 60;
    ds3231_interface_debug_print("ds3231: set time %04d-%02d-%02d PM %02d:%02d:%02d %d.\n",
                                 time_in.year, time_in.month, time_in.date,
                                 time_in.hour, time_in.minute, time_in.second, time_in.week
                                );
    res  = ds3231_set_time(&gs_handle, &time_in);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: set time failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    res = ds3231_get_time(&gs_handle, &time_out);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: get time failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    ds3231_interface_debug_print("ds3231: check time %s.\n", (memcmp(&time_in, &time_out, sizeof(ds3231_time_t))==0)?"ok":"error");
    
    /* 24H format */
    time_in.format = DS3231_FORMAT_24H;
    time_in.am_pm = DS3231_AM;
    time_in.year = rand() % 100 + 2090;
    time_in.month = rand() % 12 + 1;
    time_in.date = rand() % 20 + 1;
    time_in.week = rand() % 7 + 1;
    time_in.hour = rand() % 24;
    time_in.minute = rand() % 60;
    time_in.second = rand() % 60;
    ds3231_interface_debug_print("ds3231: set time %04d-%02d-%02d %02d:%02d:%02d %d.\n",
                                 time_in.year, time_in.month, time_in.date,
                                 time_in.hour, time_in.minute, time_in.second, time_in.week
                                );
    res  = ds3231_set_time(&gs_handle, &time_in);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: set time failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    res = ds3231_get_time(&gs_handle, &time_out);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: get time failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    ds3231_interface_debug_print("ds3231: check time %s.\n", (memcmp(&time_in, &time_out, sizeof(ds3231_time_t))==0)?"ok":"error");
    
    /* ds3231_set_alarm1/ds3231_get_alarm1 test */
    ds3231_interface_debug_print("ds3231: ds3231_set_alarm1/ds3231_get_alarm1 test.\n");
    
    /* 12H format */
    time_in.format = DS3231_FORMAT_12H;
    time_in.am_pm = DS3231_PM;
    time_in.year = 0;
    time_in.month = 0;
    time_in.week = rand() % 7 + 1;
    time_in.date = 1;
    time_in.hour = rand() % 11 + 1;
    time_in.minute = rand() % 60;
    time_in.second = rand() % 60;
    ds3231_interface_debug_print("ds3231: set alarm1 time %04d-%02d-%02d PM %02d:%02d:%02d %d.\n",
                                 time_in.year, time_in.month, time_in.date,
                                 time_in.hour, time_in.minute, time_in.second, time_in.week
                                );
    res = ds3231_set_alarm1(&gs_handle, &time_in, DS3231_ALARM1_MODE_WEEK_HOUR_MINUTE_SECOND_MATCH);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: set alarm1 failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    res = ds3231_get_alarm1(&gs_handle, &time_out, &mode1);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: get alarm1 failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    time_in.date = 0;
    ds3231_interface_debug_print("ds3231: check alarm1 time %s.\n", (memcmp(&time_in, &time_out, sizeof(ds3231_time_t))==0)?"ok":"error");
    
    /* 24H format */
    time_in.format = DS3231_FORMAT_24H;
    time_in.am_pm = DS3231_AM;
    time_in.year = 0;
    time_in.month = 0;
    time_in.week = rand() % 7 + 1;
    time_in.date = 1;
    time_in.hour = rand() % 24;
    time_in.minute = rand() % 60;
    time_in.second = rand() % 60;
    ds3231_interface_debug_print("ds3231: set alarm1 time %04d-%02d-%02d %02d:%02d:%02d %d.\n",
                                 time_in.year, time_in.month, time_in.date,
                                 time_in.hour, time_in.minute, time_in.second, time_in.week
                                );
    res = ds3231_set_alarm1(&gs_handle, &time_in, DS3231_ALARM1_MODE_WEEK_HOUR_MINUTE_SECOND_MATCH);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: set alarm1 failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    res = ds3231_get_alarm1(&gs_handle, &time_out, &mode1);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: get alarm1 failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    time_in.date = 0;
    ds3231_interface_debug_print("ds3231: check alarm1 time %s.\n", (memcmp(&time_in, &time_out, sizeof(ds3231_time_t))==0)?"ok":"error");
    
    /* ALARM1_MODE_ONCE_A_SECOND */
    time_in.format = DS3231_FORMAT_24H;
    time_in.am_pm = DS3231_AM;
    time_in.year = 0;
    time_in.month = 0;
    time_in.week = rand() % 7 + 1;
    time_in.date = rand() % 20 + 1;
    time_in.hour = rand() % 24;
    time_in.minute = rand() % 60;
    time_in.second = rand() % 60;
    ds3231_interface_debug_print("ds3231: set alarm1 time %04d-%02d-%02d %02d:%02d:%02d %d.\n",
                                 time_in.year, time_in.month, time_in.date,
                                 time_in.hour, time_in.minute, time_in.second, time_in.week
                                );
    res = ds3231_set_alarm1(&gs_handle, &time_in, DS3231_ALARM1_MODE_ONCE_A_SECOND);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: set alarm1 failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    ds3231_interface_debug_print("ds3231: set alarm1 mode ALARM1_MODE_ONCE_A_SECOND.\n");
    res = ds3231_get_alarm1(&gs_handle, &time_out, &mode1);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: get alarm1 failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    ds3231_interface_debug_print("ds3231: check alarm1 mode %s.\n", (mode1 == DS3231_ALARM1_MODE_ONCE_A_SECOND)?"ok":"error");
    
    /* ALARM1_MODE_SECOND_MATCH */
    time_in.format = DS3231_FORMAT_24H;
    time_in.am_pm = DS3231_AM;
    time_in.year = 0;
    time_in.month = 0;
    time_in.week = rand() % 7 + 1;
    time_in.date = rand() % 20 + 1;
    time_in.hour = rand() % 24;
    time_in.minute = rand() % 60;
    time_in.second = rand() % 60;
    ds3231_interface_debug_print("ds3231: set alarm1 time %04d-%02d-%02d %02d:%02d:%02d %d.\n",
                                 time_in.year, time_in.month, time_in.date,
                                 time_in.hour, time_in.minute, time_in.second, time_in.week
                                );
    res = ds3231_set_alarm1(&gs_handle, &time_in, DS3231_ALARM1_MODE_SECOND_MATCH);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: set alarm1 failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    ds3231_interface_debug_print("ds3231: set alarm1 mode ALARM1_MODE_SECOND_MATCH.\n");
    res = ds3231_get_alarm1(&gs_handle, &time_out, &mode1);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: get alarm1 failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    ds3231_interface_debug_print("ds3231: check alarm1 mode %s.\n", (mode1 == DS3231_ALARM1_MODE_SECOND_MATCH)?"ok":"error");
    
    /* ALARM1_MODE_MINUTE_SECOND_MATCH */
    time_in.format = DS3231_FORMAT_24H;
    time_in.am_pm = DS3231_AM;
    time_in.year = 0;
    time_in.month = 0;
    time_in.week = rand() % 7 + 1;
    time_in.date = rand() % 20 + 1;
    time_in.hour = rand() % 24;
    time_in.minute = rand() % 60;
    time_in.second = rand() % 60;
    ds3231_interface_debug_print("ds3231: set alarm1 time %04d-%02d-%02d %02d:%02d:%02d %d.\n",
                                 time_in.year, time_in.month, time_in.date,
                                 time_in.hour, time_in.minute, time_in.second, time_in.week
                                );
    res = ds3231_set_alarm1(&gs_handle, &time_in, DS3231_ALARM1_MODE_MINUTE_SECOND_MATCH);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: set alarm1 failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    ds3231_interface_debug_print("ds3231: set alarm1 mode ALARM1_MODE_MINUTE_SECOND_MATCH.\n");
    res = ds3231_get_alarm1(&gs_handle, &time_out, &mode1);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: get alarm1 failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    ds3231_interface_debug_print("ds3231: check alarm1 mode %s.\n", (mode1 == DS3231_ALARM1_MODE_MINUTE_SECOND_MATCH)?"ok":"error"); 
    
    /* ALARM1_MODE_HOUR_MINUTE_SECOND_MATCH */
    time_in.format = DS3231_FORMAT_24H;
    time_in.am_pm = DS3231_AM;
    time_in.year = 0;
    time_in.month = 0;
    time_in.week = rand() % 7 + 1;
    time_in.date = rand() % 20 + 1;
    time_in.hour = rand() % 24;
    time_in.minute = rand() % 60;
    time_in.second = rand() % 60;
    ds3231_interface_debug_print("ds3231: set alarm1 time %04d-%02d-%02d %02d:%02d:%02d %d.\n",
                                 time_in.year, time_in.month, time_in.date,
                                 time_in.hour, time_in.minute, time_in.second, time_in.week
                                );
    res = ds3231_set_alarm1(&gs_handle, &time_in, DS3231_ALARM1_MODE_HOUR_MINUTE_SECOND_MATCH);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: set alarm1 failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    ds3231_interface_debug_print("ds3231: set alarm1 mode ALARM1_MODE_HOUR_MINUTE_SECOND_MATCH.\n");
    res = ds3231_get_alarm1(&gs_handle, &time_out, &mode1);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: get alarm1 failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    ds3231_interface_debug_print("ds3231: check alarm1 mode %s.\n", (mode1 == DS3231_ALARM1_MODE_HOUR_MINUTE_SECOND_MATCH)?"ok":"error"); 
    
    /* DS3231_ALARM1_MODE_DATE_HOUR_MINUTE_SECOND_MATCH */
    time_in.format = DS3231_FORMAT_24H;
    time_in.am_pm = DS3231_AM;
    time_in.year = 0;
    time_in.month = 0;
    time_in.week = rand() % 7 + 1;
    time_in.date = rand() % 20 + 1;
    time_in.hour = rand() % 24;
    time_in.minute = rand() % 60;
    time_in.second = rand() % 60;
    ds3231_interface_debug_print("ds3231: set alarm1 time %04d-%02d-%02d %02d:%02d:%02d %d.\n",
                                 time_in.year, time_in.month, time_in.date,
                                 time_in.hour, time_in.minute, time_in.second, time_in.week
                                );
    res = ds3231_set_alarm1(&gs_handle, &time_in, DS3231_ALARM1_MODE_DATE_HOUR_MINUTE_SECOND_MATCH);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: set alarm1 failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    ds3231_interface_debug_print("ds3231: set alarm1 mode ALARM1_MODE_DATE_HOUR_MINUTE_SECOND_MATCH.\n");
    res = ds3231_get_alarm1(&gs_handle, &time_out, &mode1);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: get alarm1 failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    ds3231_interface_debug_print("ds3231: check alarm1 mode %s.\n", (mode1 == DS3231_ALARM1_MODE_DATE_HOUR_MINUTE_SECOND_MATCH)?"ok":"error"); 
    
    /* DS3231_ALARM1_MODE_WEEK_HOUR_MINUTE_SECOND_MATCH */
    time_in.format = DS3231_FORMAT_24H;
    time_in.am_pm = DS3231_AM;
    time_in.year = 0;
    time_in.month = 0;
    time_in.week = rand() % 7 + 1;
    time_in.date = rand() % 20 + 1;
    time_in.hour = rand() % 24;
    time_in.minute = rand() % 60;
    time_in.second = rand() % 60;
    ds3231_interface_debug_print("ds3231: set alarm1 time %04d-%02d-%02d %02d:%02d:%02d %d.\n",
                                 time_in.year, time_in.month, time_in.date,
                                 time_in.hour, time_in.minute, time_in.second, time_in.week
                                );
    res = ds3231_set_alarm1(&gs_handle, &time_in, DS3231_ALARM1_MODE_WEEK_HOUR_MINUTE_SECOND_MATCH);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: set alarm1 failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    ds3231_interface_debug_print("ds3231: set alarm1 mode ALARM1_MODE_WEEK_HOUR_MINUTE_SECOND_MATCH.\n");
    res = ds3231_get_alarm1(&gs_handle, &time_out, &mode1);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: get alarm1 failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    ds3231_interface_debug_print("ds3231: check alarm1 mode %s.\n", (mode1 == DS3231_ALARM1_MODE_WEEK_HOUR_MINUTE_SECOND_MATCH)?"ok":"error"); 
    
    /* ds3231_set_alarm2/ds3231_get_alarm2 test */
    ds3231_interface_debug_print("ds3231: ds3231_set_alarm2/ds3231_get_alarm2 test.\n");
    
    /* 12H format */
    time_in.format = DS3231_FORMAT_12H;
    time_in.am_pm = DS3231_AM;
    time_in.year = 0;
    time_in.month = 0;
    time_in.week = rand() % 7 + 1;
    time_in.date = rand() % 20 + 1;
    time_in.hour = rand() % 11 + 1;
    time_in.minute = rand() % 60;
    time_in.second = 0;
    ds3231_interface_debug_print("ds3231: set alarm2 time %04d-%02d-%02d PM %02d:%02d:%02d %d.\n",
                                 time_in.year, time_in.month, time_in.date,
                                 time_in.hour, time_in.minute, time_in.second, time_in.week
                                );
    res = ds3231_set_alarm2(&gs_handle, &time_in, DS3231_ALARM2_MODE_WEEK_HOUR_MINUTE_MATCH);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: set alarm2 failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    res = ds3231_get_alarm2(&gs_handle, &time_out, &mode2);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: get alarm2 failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    time_in.date = 0;
    ds3231_interface_debug_print("ds3231: check alarm2 time %s.\n", (memcmp(&time_in, &time_out, sizeof(ds3231_time_t))==0)?"ok":"error");
    
    /* 24H format */
    time_in.format = DS3231_FORMAT_24H;
    time_in.am_pm = DS3231_AM;
    time_in.year = 0;
    time_in.month = 0;
    time_in.week = rand() % 7 + 1;
    time_in.date = rand() % 20 + 1;
    time_in.hour = rand() % 24;
    time_in.minute = rand() % 60;
    time_in.second = 0;
    ds3231_interface_debug_print("ds3231: set alarm2 time %04d-%02d-%02d %02d:%02d:%02d %d.\n",
                                 time_in.year, time_in.month, time_in.date,
                                 time_in.hour, time_in.minute, time_in.second, time_in.week
                                );
    res = ds3231_set_alarm2(&gs_handle, &time_in, DS3231_ALARM2_MODE_WEEK_HOUR_MINUTE_MATCH);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: set alarm2 failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    res = ds3231_get_alarm2(&gs_handle, &time_out, &mode2);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: get alarm2 failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    time_in.date = 0;
    ds3231_interface_debug_print("ds3231: check alarm2 time %s.\n", (memcmp(&time_in, &time_out, sizeof(ds3231_time_t))==0)?"ok":"error");
    
    /* DS3231_ALARM2_MODE_ONCE_A_MINUTE */
    time_in.format = DS3231_FORMAT_24H;
    time_in.am_pm = DS3231_AM;
    time_in.year = 0;
    time_in.month = 0;
    time_in.week = rand() % 7 + 1;
    time_in.date = rand() % 20 + 1;
    time_in.hour = rand() % 24;
    time_in.minute = rand() % 60;
    time_in.second = 0;
    ds3231_interface_debug_print("ds3231: set alarm2 time %04d-%02d-%02d %02d:%02d:%02d %d.\n",
                                 time_in.year, time_in.month, time_in.date,
                                 time_in.hour, time_in.minute, time_in.second, time_in.week
                                );
    res = ds3231_set_alarm2(&gs_handle, &time_in, DS3231_ALARM2_MODE_ONCE_A_MINUTE);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: set alarm2 failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    ds3231_interface_debug_print("ds3231: set mode ALARM2_MODE_ONCE_A_MINUTE.\n");
    res = ds3231_get_alarm2(&gs_handle, &time_out, &mode2);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: get alarm2 failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    ds3231_interface_debug_print("ds3231: check alarm2 time %s.\n", (mode2 == DS3231_ALARM2_MODE_ONCE_A_MINUTE)?"ok":"error");
    
    /* ALARM2_MODE_MINUTE_MATCH */
    time_in.format = DS3231_FORMAT_24H;
    time_in.am_pm = DS3231_AM;
    time_in.year = 0;
    time_in.month = 0;
    time_in.week = rand() % 7 + 1;
    time_in.date = rand() % 20 + 1;
    time_in.hour = rand() % 24;
    time_in.minute = rand() % 60;
    time_in.second = 0;
    ds3231_interface_debug_print("ds3231: set alarm2 time %04d-%02d-%02d %02d:%02d:%02d %d.\n",
                                 time_in.year, time_in.month, time_in.date,
                                 time_in.hour, time_in.minute, time_in.second, time_in.week
                                );
    res = ds3231_set_alarm2(&gs_handle, &time_in, DS3231_ALARM2_MODE_MINUTE_MATCH);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: set alarm2 failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    ds3231_interface_debug_print("ds3231: set mode ALARM2_MODE_MINUTE_MATCH.\n");
    res = ds3231_get_alarm2(&gs_handle, &time_out, &mode2);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: get alarm2 failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    ds3231_interface_debug_print("ds3231: check alarm2 time %s.\n", (mode2 == DS3231_ALARM2_MODE_MINUTE_MATCH)?"ok":"error");
    
    /* ALARM2_MODE_HOUR_MINUTE_MATCH */
    time_in.format = DS3231_FORMAT_24H;
    time_in.am_pm = DS3231_AM;
    time_in.year = 0;
    time_in.month = 0;
    time_in.week = rand() % 7 + 1;
    time_in.date = rand() % 20 + 1;
    time_in.hour = rand() % 24;
    time_in.minute = rand() % 60;
    time_in.second = 0;
    ds3231_interface_debug_print("ds3231: set alarm2 time %04d-%02d-%02d %02d:%02d:%02d %d.\n",
                                 time_in.year, time_in.month, time_in.date,
                                 time_in.hour, time_in.minute, time_in.second, time_in.week
                                );
    res = ds3231_set_alarm2(&gs_handle, &time_in, DS3231_ALARM2_MODE_HOUR_MINUTE_MATCH);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: set alarm2 failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    ds3231_interface_debug_print("ds3231: set mode ALARM2_MODE_HOUR_MINUTE_MATCH.\n");
    res = ds3231_get_alarm2(&gs_handle, &time_out, &mode2);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: get alarm2 failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    ds3231_interface_debug_print("ds3231: check alarm2 time %s.\n", (mode2 == DS3231_ALARM2_MODE_HOUR_MINUTE_MATCH)?"ok":"error");
    
    /* DS3231_ALARM2_MODE_DATE_HOUR_MINUTE_MATCH */
    time_in.format = DS3231_FORMAT_24H;
    time_in.am_pm = DS3231_AM;
    time_in.year = 0;
    time_in.month = 0;
    time_in.week = rand() % 7 + 1;
    time_in.date = rand() % 20 + 1;
    time_in.hour = rand() % 24;
    time_in.minute = rand() % 60;
    time_in.second = 0;
    ds3231_interface_debug_print("ds3231: set alarm2 time %04d-%02d-%02d %02d:%02d:%02d %d.\n",
                                 time_in.year, time_in.month, time_in.date,
                                 time_in.hour, time_in.minute, time_in.second, time_in.week
                                );
    res = ds3231_set_alarm2(&gs_handle, &time_in, DS3231_ALARM2_MODE_DATE_HOUR_MINUTE_MATCH);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: set alarm2 failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    ds3231_interface_debug_print("ds3231: set mode ALARM2_MODE_DATE_HOUR_MINUTE_MATCH.\n");
    res = ds3231_get_alarm2(&gs_handle, &time_out, &mode2);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: get alarm2 failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    ds3231_interface_debug_print("ds3231: check alarm2 time %s.\n", (mode2 == DS3231_ALARM2_MODE_DATE_HOUR_MINUTE_MATCH)?"ok":"error");
    
    /* DS3231_ALARM2_MODE_WEEK_HOUR_MINUTE_MATCH */
    time_in.format = DS3231_FORMAT_24H;
    time_in.am_pm = DS3231_AM;
    time_in.year = 0;
    time_in.month = 0;
    time_in.week = rand() % 7 + 1;
    time_in.date = rand() % 20 + 1;
    time_in.hour = rand() % 24;
    time_in.minute = rand() % 60;
    time_in.second = 0;
    ds3231_interface_debug_print("ds3231: set alarm2 time %04d-%02d-%02d %02d:%02d:%02d %d.\n",
                                 time_in.year, time_in.month, time_in.date,
                                 time_in.hour, time_in.minute, time_in.second, time_in.week
                                );
    res = ds3231_set_alarm2(&gs_handle, &time_in, DS3231_ALARM2_MODE_WEEK_HOUR_MINUTE_MATCH);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: set alarm2 failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    ds3231_interface_debug_print("ds3231: set mode ALARM2_MODE_WEEK_HOUR_MINUTE_MATCH.\n");
    res = ds3231_get_alarm2(&gs_handle, &time_out, &mode2);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: get alarm2 failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    ds3231_interface_debug_print("ds3231: check alarm2 time %s.\n", (mode2 == DS3231_ALARM2_MODE_WEEK_HOUR_MINUTE_MATCH)?"ok":"error");
    
    /* ds3231_set_oscillator/ds3231_get_oscillator test */
    ds3231_interface_debug_print("ds3231: ds3231_set_oscillator/ds3231_get_oscillator test.\n");
    
    /* enable */
    res = ds3231_set_oscillator(&gs_handle, DS3231_BOOL_TRUE);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: set oscillator failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    ds3231_interface_debug_print("ds3231: enable.\n");
    res = ds3231_get_oscillator(&gs_handle, &enable);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: get oscillator failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    ds3231_interface_debug_print("ds3231: check bool %s.\n", (enable == DS3231_BOOL_TRUE)?"ok":"error");
    
    /* disable */
    res = ds3231_set_oscillator(&gs_handle, DS3231_BOOL_FALSE);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: set oscillator failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    ds3231_interface_debug_print("ds3231: disable.\n");
    res = ds3231_get_oscillator(&gs_handle, &enable);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: get oscillator failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    ds3231_interface_debug_print("ds3231: check bool %s.\n", (enable == DS3231_BOOL_FALSE)?"ok":"error");
    
    /* ds3231_set_alarm_interrupt/ds3231_get_alarm_interrupt */
    ds3231_interface_debug_print("ds3231: ds3231_set_alarm_interrupt/ds3231_get_alarm_interrupt test.\n");
    
    /* alarm1 enable*/
    res = ds3231_set_alarm_interrupt(&gs_handle, DS3231_ALARM_1, DS3231_BOOL_TRUE);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: set alarm interrupt failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    ds3231_interface_debug_print("ds3231: alarm1 enable.\n");
    res = ds3231_get_alarm_interrupt(&gs_handle, DS3231_ALARM_1, &enable);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: get alarm interrupt failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    ds3231_interface_debug_print("ds3231: check alarm1 bool %s.\n", (enable == DS3231_BOOL_TRUE)?"ok":"error");
    
    /* alarm1 disable*/
    res = ds3231_set_alarm_interrupt(&gs_handle, DS3231_ALARM_1, DS3231_BOOL_FALSE);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: set alarm interrupt failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    ds3231_interface_debug_print("ds3231: alarm1 disable.\n");
    res = ds3231_get_alarm_interrupt(&gs_handle, DS3231_ALARM_1, &enable);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: get alarm interrupt failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    ds3231_interface_debug_print("ds3231: check alarm1 bool %s.\n", (enable == DS3231_BOOL_FALSE)?"ok":"error");
    
    /* alarm2 enable*/
    res = ds3231_set_alarm_interrupt(&gs_handle, DS3231_ALARM_2, DS3231_BOOL_TRUE);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: set alarm interrupt failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    ds3231_interface_debug_print("ds3231: alarm2 enable.\n");
    res = ds3231_get_alarm_interrupt(&gs_handle, DS3231_ALARM_2, &enable);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: get alarm interrupt failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    ds3231_interface_debug_print("ds3231: check alarm2 bool %s.\n", (enable == DS3231_BOOL_TRUE)?"ok":"error");
    
    /* alarm2 disable*/
    res = ds3231_set_alarm_interrupt(&gs_handle, DS3231_ALARM_2, DS3231_BOOL_FALSE);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: set alarm interrupt failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    ds3231_interface_debug_print("ds3231: alarm2 disable.\n");
    res = ds3231_get_alarm_interrupt(&gs_handle, DS3231_ALARM_2, &enable);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: get alarm interrupt failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    ds3231_interface_debug_print("ds3231: check alarm2 bool %s.\n", (enable == DS3231_BOOL_FALSE)?"ok":"error");
    
    /* ds3231_set_pin/ds3231_get_pin */
    ds3231_interface_debug_print("ds3231: ds3231_set_pin/ds3231_get_pin test.\n");
    
    /* set square wave pin */
    res = ds3231_set_pin(&gs_handle, DS3231_PIN_SQUARE_WAVE);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: set pin failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    ds3231_interface_debug_print("ds3231: set square wave pin.\n");
    res = ds3231_get_pin(&gs_handle, &pin);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: get pin failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    ds3231_interface_debug_print("ds3231: check pin %s.\n", (pin == DS3231_PIN_SQUARE_WAVE)?"ok":"error");
    
    /* set interrupt pin */
    res = ds3231_set_pin(&gs_handle, DS3231_PIN_INTERRUPT);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: set pin failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    ds3231_interface_debug_print("ds3231: set interrupt pin.\n");
    res = ds3231_get_pin(&gs_handle, &pin);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: get pin failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    ds3231_interface_debug_print("ds3231: check pin %s.\n", (pin == DS3231_PIN_INTERRUPT)?"ok":"error");
    
    /* ds3231_set_square_wave/ds3231_get_square_wave */
    ds3231_interface_debug_print("ds3231: ds3231_set_square_wave/ds3231_get_square_wave test.\n");
    
    /* enable */
    res = ds3231_set_square_wave(&gs_handle, DS3231_BOOL_TRUE);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: set square wave failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    ds3231_interface_debug_print("ds3231: set enable.\n");
    res = ds3231_get_square_wave(&gs_handle, &enable);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: get square wave failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    ds3231_interface_debug_print("ds3231: check bool %s.\n", (enable == DS3231_BOOL_TRUE)?"ok":"error");
    
    /* disable */
    res = ds3231_set_square_wave(&gs_handle, DS3231_BOOL_FALSE);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: set square wave failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    ds3231_interface_debug_print("ds3231: set disable.\n");
    res = ds3231_get_square_wave(&gs_handle, &enable);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: get square wave failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    ds3231_interface_debug_print("ds3231: check bool %s.\n", (enable == DS3231_BOOL_FALSE)?"ok":"error");
    
    /* ds3231_set_32khz_output/ds3231_get_32khz_output */
    ds3231_interface_debug_print("ds3231: ds3231_set_32khz_output/ds3231_get_32khz_output test.\n");
    
    /* enable */
    res = ds3231_set_32khz_output(&gs_handle, DS3231_BOOL_TRUE);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: set 32khz output failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    ds3231_interface_debug_print("ds3231: enable.\n");
    res = ds3231_get_32khz_output(&gs_handle, &enable);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: get 32khz output failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    ds3231_interface_debug_print("ds3231: check bool %s.\n", (enable == DS3231_BOOL_TRUE)?"ok":"error");
    
    /* disable */
    res = ds3231_set_32khz_output(&gs_handle, DS3231_BOOL_FALSE);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: set 32khz output failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    ds3231_interface_debug_print("ds3231: disable.\n");
    res = ds3231_get_32khz_output(&gs_handle, &enable);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: get 32khz output failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    ds3231_interface_debug_print("ds3231: check bool %s.\n", (enable == DS3231_BOOL_FALSE)?"ok":"error");
    
    /* ds3231_set_aging_offset/ds3231_get_aging_offset */
    ds3231_interface_debug_print("ds3231: ds3231_set_aging_offset/ds3231_get_aging_offset test.\n");
    offset = -(rand() % 128);
    res = ds3231_set_aging_offset(&gs_handle, offset);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: set aging offset failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    ds3231_interface_debug_print("ds3231: set offset %d.\n", offset);
    res = ds3231_get_aging_offset(&gs_handle, (int8_t *)&offset_check);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: get aging offset failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    ds3231_interface_debug_print("ds3231: check offset %s.\n", (offset_check == offset)?"ok":"error");
    
    /* ds3231_get_status_offset */
    ds3231_interface_debug_print("ds3231: ds3231_get_status test.\n");
    res = ds3231_get_status(&gs_handle, (uint8_t *)&status);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: get status failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    ds3231_interface_debug_print("ds3231: check status 0x%02X.\n", status);
    
    /* ds3231_aging_offset_convert_to_register/ds3231_aging_offset_convert_to_data test */
    ds3231_interface_debug_print("ds3231: ds3231_aging_offset_convert_to_register/ds3231_aging_offset_convert_to_data test.\n");
    o = (float)(rand() % 100) /100.0f;
    res = ds3231_aging_offset_convert_to_register(&gs_handle, o, (int8_t *)&offset);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: aging offset convert to register failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    ds3231_interface_debug_print("ds3231: offset is %d.\n", offset);
    res = ds3231_aging_offset_convert_to_data(&gs_handle, offset, (float *)&o_check);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: aging offset convert to data failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    ds3231_interface_debug_print("ds3231: offset is %0.3f.\n", o);
    ds3231_interface_debug_print("ds3231: convert offset is %0.3f.\n", o_check);
    
    /* finish register test */
    ds3231_interface_debug_print("ds3231: finish register test.\n");
    (void)ds3231_deinit(&gs_handle);
    
    return 0;
}
