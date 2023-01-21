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
 * @file      driver_ds3231_alarm_test.c
 * @brief     driver ds3231 alarm test source file
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

#include "driver_ds3231_alarm_test.h"
#include <stdlib.h>

static ds3231_handle_t gs_handle;        /**< ds3231 handle */

/**
 * @brief  alarm test
 * @return status code
 *         - 0 success
 *         - 1 test failed
 * @note   none
 */
uint8_t ds3231_alarm_test(void)
{
    uint8_t res;
    int8_t reg;
    uint8_t times;
    uint8_t status;
    ds3231_info_t info;
    ds3231_time_t time_in;
    
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
    
    /* start alarm test */
    ds3231_interface_debug_print("ds3231: start alarm test.\n");
    
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
    
    /* enable alarm1 */
    res = ds3231_set_alarm_interrupt(&gs_handle, DS3231_ALARM_1, DS3231_BOOL_TRUE);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: set alarm1 interrupt failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable alarm2 */
    res = ds3231_set_alarm_interrupt(&gs_handle, DS3231_ALARM_2, DS3231_BOOL_TRUE);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: set alarm2 interrupt failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set interrupt */
    res = ds3231_set_pin(&gs_handle, DS3231_PIN_INTERRUPT);
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
    
    /* clear alarm 1 */
    res = ds3231_alarm_clear(&gs_handle, DS3231_ALARM_1);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: alarm clear failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    res = ds3231_set_alarm1(&gs_handle, &time_in, DS3231_ALARM1_MODE_ONCE_A_SECOND);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: set alarm1 failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    ds3231_interface_delay_ms(2000);
    res = ds3231_get_status(&gs_handle, (uint8_t *)&status);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: get status failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    if ((status & DS3231_STATUS_ALARM_1) != 0)
    {
        ds3231_interface_debug_print("ds3231: check alarm1 %s.\n", "ok"); 
    }
    else
    {
        ds3231_interface_debug_print("ds3231: check alarm1 %s.\n", "error");
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
    
    /* clear alarm 2 */
    res = ds3231_alarm_clear(&gs_handle, DS3231_ALARM_2);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: alarm clear failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    res = ds3231_set_alarm2(&gs_handle, &time_in, DS3231_ALARM2_MODE_ONCE_A_MINUTE);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: set alarm2 failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    times = 61;
    while (times != 0)
    {
        ds3231_interface_delay_ms(1000);
        times--;
        ds3231_interface_debug_print("ds3231: time left %ds.\n", times);
    }
    res = ds3231_get_status(&gs_handle, (uint8_t *)&status);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: get status failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    if ((status & DS3231_STATUS_ALARM_2) != 0)
    {
        ds3231_interface_debug_print("ds3231: check alarm2 %s.\n", "ok"); 
    }
    else
    {
        ds3231_interface_debug_print("ds3231: check alarm2 %s.\n", "error");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable alarm2 */
    res = ds3231_set_alarm_interrupt(&gs_handle, DS3231_ALARM_2, DS3231_BOOL_FALSE);
    if (res != 0)
    {
        ds3231_interface_debug_print("ds3231: set alarm1 interrupt failed.\n");
        (void)ds3231_deinit(&gs_handle);
        
        return 1;
    }
    
    /* finish alarm test */
    ds3231_interface_debug_print("ds3231: finish alarm test.\n");
    (void)ds3231_deinit(&gs_handle);
    
    return 0;
}
