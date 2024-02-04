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
 * @file      driver_ds3231_readwrite_test.c
 * @brief     driver ds3231 readwrite test source file
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

#include "driver_ds3231_readwrite_test.h"
#include <stdlib.h>

static ds3231_handle_t gs_handle;        /**< ds3231 handle */

/**
 * @brief     readwrite test
 * @param[in] times is the test times
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t ds3231_readwrite_test(uint32_t times)
{
    uint8_t res;
    int8_t reg;
    uint32_t i;
    int16_t raw;
    float s;
    ds3231_info_t info;
    ds3231_time_t time_in, time_out;
    
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
    
    /* start readwrite test */
    ds3231_interface_debug_print("ds3231: start readwrite test.\n");
    
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
    
    /* disable square wave */
    res = ds3231_set_square_wave(&gs_handle, DS3231_BOOL_FALSE);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: set square wave failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable 32khz output */
    res = ds3231_set_32khz_output(&gs_handle, DS3231_BOOL_FALSE);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: set 32khz output failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    
    /* convert to register */
    res = ds3231_aging_offset_convert_to_register(&gs_handle, 0, (int8_t *)&reg);
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
    ds3231_interface_debug_print("ds3231: set time %04d-%02d-%02d PM %02d:%02d:%02d %d in 12 format.\n",
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
    for (i = 0; i < times; i++)
    {
        ds3231_interface_delay_ms(1000);
        res = ds3231_get_time(&gs_handle, &time_out);
        if (res != 0)
        {
            ds3231_interface_debug_print("ds3231: get time failed.\n");
            (void)ds3231_deinit(&gs_handle);
            
            return 1;
        }
        ds3231_interface_debug_print("ds3231: time is %04d-%02d-%02d PM %02d:%02d:%02d %d.\n",
                                     time_out.year, time_out.month, time_out.date,
                                     time_out.hour, time_out.minute, time_out.second, time_out.week
                                    );
    }
    
    /* 24H format */
    time_in.format = DS3231_FORMAT_24H;
    time_in.am_pm = DS3231_AM;
    time_in.year = rand() % 100 + 2090;
    time_in.month = rand() % 12 + 1;
    time_in.date = rand() % 20 + 1;
    time_in.week = rand() % 7 + 1;
    time_in.hour = rand() % 12 + 12;
    time_in.minute = rand() % 60;
    time_in.second = rand() % 60;
    ds3231_interface_debug_print("ds3231: set time %04d-%02d-%02d %02d:%02d:%02d %d in 24 format.\n",
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
    for (i = 0; i < times; i++)
    {
        ds3231_interface_delay_ms(1000);
        res = ds3231_get_time(&gs_handle, &time_out);
        if (res != 0)
        {
            ds3231_interface_debug_print("ds3231: get time failed.\n");
            (void)ds3231_deinit(&gs_handle);
            
            return 1;
        }
        ds3231_interface_debug_print("ds3231: time is %04d-%02d-%02d %02d:%02d:%02d %d.\n",
                                     time_out.year, time_out.month, time_out.date,
                                     time_out.hour, time_out.minute, time_out.second, time_out.week
                                    );
    }
    ds3231_interface_debug_print("ds3231: read temperature.\n");
    for (i = 0; i < times; i++)
    {
        ds3231_interface_delay_ms(1000);
        res = ds3231_get_temperature(&gs_handle, (int16_t *)&raw, (float *)&s);
        if (res != 0)
        {
            ds3231_interface_debug_print("ds3231: get temperature failed.\n");
            (void)ds3231_deinit(&gs_handle);
            
            return 1;
        }
        ds3231_interface_debug_print("ds3231: temperature is %0.2f.\n", s);
    }
    
    /* finish readwrite test */
    ds3231_interface_debug_print("ds3231: finish readwrite test.\n");
    (void)ds3231_deinit(&gs_handle);
    
    return 0;
}
