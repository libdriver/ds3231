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
 * @file      main.c
 * @brief     main source file
 * @version   2.0.0
 * @author    Shifeng Li
 * @date      2021-03-15
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2021/03/15  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "driver_ds3231_register_test.h"
#include "driver_ds3231_readwrite_test.h"
#include "driver_ds3231_alarm_test.h"
#include "driver_ds3231_output_test.h"
#include "driver_ds3231_basic.h"
#include "driver_ds3231_alarm.h"
#include "driver_ds3231_output.h"
#include "gpio.h"
#include <stdlib.h>

/**
 * @brief global var definition
 */
uint8_t (*g_gpio_irq)(void) = NULL;        /**< gpio irq function address */

/**
 * @brief     interface receive callback
 * @param[in] type is the interrupt type
 * @note      none
 */
static void a_alarm_receive_callback(uint8_t type)
{
    switch (type)
    {
        case DS3231_STATUS_ALARM_2 :
        {
            (void)ds3231_alarm_clear_flag(DS3231_ALARM_2);
            ds3231_interface_debug_print("ds3231: irq alarm2.\n");
            
            break;
        }
        case DS3231_STATUS_ALARM_1 :
        {
            (void)ds3231_alarm_clear_flag(DS3231_ALARM_1);
            ds3231_interface_debug_print("ds3231: irq alarm1.\n");
            
            break;
        }
        default :
        {
            break;
        }
    }
}

/**
 * @brief     ds3231 full function
 * @param[in] argc is arg numbers
 * @param[in] **argv is the arg address
 * @return    status code
 *             - 0 success
 *             - 1 run failed
 *             - 5 param is invalid
 * @note      none
 */
uint8_t ds3231(uint8_t argc, char **argv)
{
    if (argc == 1)
    {
        goto help;
    }
    else if (argc == 2)
    {
        if (strcmp("-i", argv[1]) == 0)
        {
            ds3231_info_t info;
            
            /* print ds3231 info */
            ds3231_info(&info);
            ds3231_interface_debug_print("ds3231: chip is %s.\n", info.chip_name);
            ds3231_interface_debug_print("ds3231: manufacturer is %s.\n", info.manufacturer_name);
            ds3231_interface_debug_print("ds3231: interface is %s.\n", info.interface);
            ds3231_interface_debug_print("ds3231: driver version is %d.%d.\n", info.driver_version/1000, (info.driver_version%1000)/100);
            ds3231_interface_debug_print("ds3231: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
            ds3231_interface_debug_print("ds3231: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
            ds3231_interface_debug_print("ds3231: max current is %0.2fmA.\n", info.max_current_ma);
            ds3231_interface_debug_print("ds3231: max temperature is %0.1fC.\n", info.temperature_max);
            ds3231_interface_debug_print("ds3231: min temperature is %0.1fC.\n", info.temperature_min);
            
            return 0;
        }
        else if (strcmp("-p", argv[1]) == 0)
        {
            /* print pin connection */
            ds3231_interface_debug_print("ds3231: SCL connected to GPIO3(BCM).\n");
            ds3231_interface_debug_print("ds3231: SDA connected to GPIO2(BCM).\n");
            ds3231_interface_debug_print("ds3231: INT connected to GPIO17(BCM).\n");
            
            return 0;
        }
        else if (strcmp("-h", argv[1]) == 0)
        {
            /* show ds3231 help */
            
            help:
            
            ds3231_interface_debug_print("ds3231 -i\n\tshow ds3231 chip and driver information.\n");
            ds3231_interface_debug_print("ds3231 -h\n\tshow ds3231 help.\n");
            ds3231_interface_debug_print("ds3231 -p\n\tshow ds3231 pin connections of the current board.\n");
            ds3231_interface_debug_print("ds3231 -t reg\n\trun ds3231 register test.\n");
            ds3231_interface_debug_print("ds3231 -t readwrite <times>\n\trun ds3231 readwrite test.times means test times.\n");
            ds3231_interface_debug_print("ds3231 -t output <times>\n\trun ds3231 output test.times means test times.\n");
            ds3231_interface_debug_print("ds3231 -c basic -set -time <timestamp>\n\trun ds3231 basic set time function."
                                         "timestamp is the unix timestamp.\n");
            ds3231_interface_debug_print("ds3231 -c basic -get -time\n\trun ds3231 basic get time function.\n");
            ds3231_interface_debug_print("ds3231 -c basic -get -temperature\n\trun ds3231 basic get temperature function.\n");
            ds3231_interface_debug_print("ds3231 -c alarm -set -time <timestamp>\n\trun ds3231 alarm set time function."
                                         "timestamp is the unix timestamp.\n");
            ds3231_interface_debug_print("ds3231 -c alarm -set -alarm1 <timestamp>\n\trun ds3231 alarm set alarm1 function."
                                         "timestamp is the unix timestamp.\n");
            ds3231_interface_debug_print("ds3231 -c alarm -set -alarm2 <timestamp>\n\trun ds3231 alarm set alarm2 function."
                                         "timestamp is the unix timestamp.\n");
            ds3231_interface_debug_print("ds3231 -c alarm -set -alarm1 -m (ONCE_A_SECOND | SECOND_MATCH | MINUTE_SECOND_MATCH | "
                                         "HOUR_MINUTE_SECOND_MATCH | DATE_HOUR_MINUTE_SECOND_MATCH | WEEK_HOUR_MINUTE_SECOND_MATCH)\n\t");
            ds3231_interface_debug_print("run ds3231 alarm set alarm1 function.\n");
            ds3231_interface_debug_print("ds3231 -c alarm -set -alarm2 -m (ONCE_A_MINUTE | MINUTE_MATCH | HOUR_MINUTE_MATCH | "
                                         "DATE_HOUR_MINUTE_MATCH | WEEK_HOUR_MINUTE_MATCH)\n\t");
            ds3231_interface_debug_print("run ds3231 alarm set alarm2 function.\n");
            ds3231_interface_debug_print("ds3231 -c alarm -get -time\n\trun ds3231 alarm get time function.\n");
            ds3231_interface_debug_print("ds3231 -c alarm -get -temperature\n\trun ds3231 alarm get temperature function.\n");
            ds3231_interface_debug_print("ds3231 -c alarm -get -alarm1\n\trun ds3231 alarm get alarm1 function.\n");
            ds3231_interface_debug_print("ds3231 -c alarm -get -alarm2\n\trun ds3231 alarm get alarm2 function.\n");
            ds3231_interface_debug_print("ds3231 -c output -set -time <timestamp>\n\trun ds3231 output set time function."
                                         "timestamp is the unix timestamp.\n");
            ds3231_interface_debug_print("ds3231 -c output -set -1Hz (enable | disable)\n\trun ds3231 output set 1Hz function.\n");
            ds3231_interface_debug_print("ds3231 -c output -set -32KHz (enable | disable)\n\trun ds3231 output set 32KHz function.\n");
            ds3231_interface_debug_print("ds3231 -c output -get -time\n\trun ds3231 output get time function.\n");
            ds3231_interface_debug_print("ds3231 -c output -get -1Hz\n\trun ds3231 output get 1Hz function.\n");
            ds3231_interface_debug_print("ds3231 -c output -get -32KHz\n\trun ds3231 output get 32KHz function.\n");
            ds3231_interface_debug_print("ds3231 -c output -get -temperature\n\trun ds3231 output get temperature function.\n");
            
            return 0;
        }
        else
        {
            return 5;
        }
    }
    else if (argc == 3)
    {
        /* run test */
        if (strcmp("-t", argv[1]) == 0)
        {
             /* reg test */
            if (strcmp("reg", argv[2]) == 0)
            {
                /* run reg test */
                if (ds3231_register_test() != 0)
                {
                    return 1;
                }
                else
                {
                    return 0;
                }
            }
             /* alarm test */
            else if (strcmp("alarm", argv[2]) == 0)
            {
                /* run alarm test */
                if (ds3231_alarm_test() != 0)
                {
                    return 1;
                }
                else
                {
                    return 0;
                }
            }
            /* param is invalid */
            else
            {
                return 5;
            }
        }
        /* param is invalid */
        else
        {
            return 5;
        }
    }
    else if (argc == 4)
    {
        /* run test */
        if (strcmp("-t", argv[1]) == 0)
        {
             /* readwrite test */
            if (strcmp("readwrite", argv[2]) == 0)
            {
                /* run readwrite test */
                if (ds3231_readwrite_test(atoi(argv[3])) != 0)
                {
                    return 1;
                }
                else
                {
                    return 0;
                }
            }
             /* output test */
            else if (strcmp("output", argv[2]) == 0)
            {
                /* run output test */
                if (ds3231_output_test(atoi(argv[3])) != 0)
                {
                    return 1;
                }
                else
                {
                    return 0;
                }
            }
            /* param is invalid */
            else
            {
                return 5;
            }
        }
        /* param is invalid */
        else
        {
            return 5;
        }
    }
    else if (argc == 5)
    {
        /* run function */
        if (strcmp("-c", argv[1]) == 0)
        {
            uint8_t res;
            
            /* basic function */
            if (strcmp("basic", argv[2]) == 0)
            {
                if (strcmp("-get", argv[3]) == 0)
                {
                    if (strcmp("-time", argv[4]) == 0)
                    {
                        ds3231_time_t time;
                        
                        res = ds3231_basic_init();
                        if (res != 0)
                        {
                            return 1;
                        }
                        res = ds3231_basic_get_time(&time);
                        if (res != 0)
                        {
                            (void)ds3231_basic_deinit();
                            
                            return 1;
                        }
                        if (time.format == DS3231_FORMAT_24H)
                        {
                            ds3231_interface_debug_print("ds3231: %04d-%02d-%02d %02d:%02d:%02d %d.\n",
                                                         time.year, time.month, time.date,
                                                         time.hour, time.minute, time.second, time.week
                                                        );
                        }
                        else
                        {
                            ds3231_interface_debug_print("ds3231: %04d-%02d-%02d %s %02d:%02d:%02d %d.\n",
                                                         time.year, time.month, time.date, (time.am_pm == DS3231_AM) ? "AM" : "PM",
                                                         time.hour, time.minute, time.second, time.week
                                                        );
                        }
                        (void)ds3231_basic_deinit();
                        
                        return 0;
                    }
                    else if (strcmp("-temperature", argv[4]) == 0)
                    {
                        int16_t raw;
                        float s;
                        
                        res = ds3231_basic_init();
                        if (res != 0)
                        {
                            return 1;
                        }
                        res = ds3231_basic_get_temperature((int16_t *)&raw, (float *)&s);
                        if (res != 0)
                        {
                            (void)ds3231_basic_deinit();
                            
                            return 1;
                        }
                        ds3231_interface_debug_print("ds3231: temperature is %0.2fC.\n", s);
                        (void)ds3231_basic_deinit();
                        
                        return 0;
                    }
                    /* param is invalid */
                    else
                    {
                        return 5;
                    }
                }
                /* param is invalid */
                else
                {
                    return 5;
                }
            }
            /* alarm function */
            else if (strcmp("alarm", argv[2]) == 0)
            {
                if (strcmp("-get", argv[3]) == 0)
                {
                    if (strcmp("-time", argv[4]) == 0)
                    {
                        ds3231_time_t time;
                        
                        res = ds3231_alarm_init(ds3231_interface_receive_callback);
                        if (res != 0)
                        {
                            return 1;
                        }
                        res = ds3231_alarm_get_time(&time);
                        if (res != 0)
                        {
                            (void)ds3231_alarm_deinit();
                            
                            return 1;
                        }
                        if (time.format == DS3231_FORMAT_24H)
                        {
                            ds3231_interface_debug_print("ds3231: %04d-%02d-%02d %02d:%02d:%02d %d.\n",
                                                         time.year, time.month, time.date,
                                                         time.hour, time.minute, time.second, time.week
                                                        );
                        }
                        else
                        {
                            ds3231_interface_debug_print("ds3231: %04d-%02d-%02d %s %02d:%02d:%02d %d.\n",
                                                         time.year, time.month, time.date, (time.am_pm == DS3231_AM) ? "AM" : "PM",
                                                         time.hour, time.minute, time.second, time.week
                                                        );
                        }
                        (void)ds3231_alarm_deinit();
                        
                        return 0;
                    }
                    else if (strcmp("-temperature", argv[4]) == 0)
                    {
                        int16_t raw;
                        float s;
                        
                        res = ds3231_alarm_init(ds3231_interface_receive_callback);
                        if (res != 0)
                        {
                            return 1;
                        }
                        res = ds3231_alarm_get_temperature((int16_t *)&raw, (float *)&s);
                        if (res != 0)
                        {
                            (void)ds3231_alarm_deinit();
                            
                            return 1;
                        }
                        ds3231_interface_debug_print("ds3231: temperature is %0.2fC.\n", s);
                        (void)ds3231_alarm_deinit();
                        
                        return 0;
                    }
                    else if (strcmp("-alarm1", argv[4]) == 0)
                    {
                        ds3231_time_t time;
                        ds3231_alarm1_mode_t mode;
                        
                        res = ds3231_alarm_init(ds3231_interface_receive_callback);
                        if (res != 0)
                        {
                            return 1;
                        }
                        res = ds3231_alarm_get_alarm1(&time, &mode);
                        if (res != 0)
                        {
                            (void)ds3231_alarm_deinit();
                            
                            return 1;
                        }
                        if (time.format == DS3231_FORMAT_24H)
                        {
                            ds3231_interface_debug_print("ds3231: %04d-%02d-%02d %02d:%02d:%02d %d.\n",
                                                         time.year, time.month, time.date,
                                                         time.hour, time.minute, time.second, time.week
                                                        );
                        }
                        else
                        {
                            ds3231_interface_debug_print("ds3231: %04d-%02d-%02d %s %02d:%02d:%02d %d.\n",
                                                         time.year, time.month, time.date, (time.am_pm == DS3231_AM) ? "AM" : "PM",
                                                         time.hour, time.minute, time.second, time.week
                                                        );
                        }
                        (void)ds3231_alarm_deinit();
                        
                        return 0;
                    }
                    else if (strcmp("-alarm2", argv[4]) == 0)
                    {
                        ds3231_time_t time;
                        ds3231_alarm2_mode_t mode;
                        
                        res = ds3231_alarm_init(ds3231_interface_receive_callback);
                        if (res != 0)
                        {
                            return 1;
                        }
                        res = ds3231_alarm_get_alarm2(&time, &mode);
                        if (res != 0)
                        {
                            (void)ds3231_alarm_deinit();
                            
                            return 1;
                        }
                        if (time.format == DS3231_FORMAT_24H)
                        {
                            ds3231_interface_debug_print("ds3231: %04d-%02d-%02d %02d:%02d:%02d %d.\n",
                                                         time.year, time.month, time.date,
                                                         time.hour, time.minute, time.second, time.week
                                                        );
                        }
                        else
                        {
                            ds3231_interface_debug_print("ds3231: %04d-%02d-%02d %s %02d:%02d:%02d %d.\n",
                                                         time.year, time.month, time.date, (time.am_pm == DS3231_AM) ? "AM" : "PM",
                                                         time.hour, time.minute, time.second, time.week
                                                        );
                        }
                        (void)ds3231_alarm_deinit();
                        
                        return 0;
                    }
                    /* param is invalid */
                    else
                    {
                        return 5;
                    }
                }
                /* param is invalid */
                else
                {
                    return 5;
                }
            }
            /* output function */
            else if (strcmp("output", argv[2]) == 0)
            {
                if (strcmp("-get", argv[3]) == 0)
                {
                    if (strcmp("-time", argv[4]) == 0)
                    {
                        ds3231_time_t time;
                        
                        res = ds3231_output_init();
                        if (res != 0)
                        {
                            return 1;
                        }
                        res = ds3231_output_get_time(&time);
                        if (res != 0)
                        {
                            (void)ds3231_output_deinit();
                            
                            return 1;
                        }
                        if (time.format == DS3231_FORMAT_24H)
                        {
                            ds3231_interface_debug_print("ds3231: %04d-%02d-%02d %02d:%02d:%02d %d.\n",
                                                         time.year, time.month, time.date,
                                                         time.hour, time.minute, time.second, time.week
                                                        );
                        }
                        else
                        {
                            ds3231_interface_debug_print("ds3231: %04d-%02d-%02d %s %02d:%02d:%02d %d.\n",
                                                         time.year, time.month, time.date, (time.am_pm == DS3231_AM) ? "AM" : "PM",
                                                         time.hour, time.minute, time.second, time.week
                                                        );
                        }
                        (void)ds3231_output_deinit();
                        
                        return 0;
                    }
                    else if (strcmp("-temperature", argv[4]) == 0)
                    {
                        int16_t raw;
                        float s;
                        
                        res = ds3231_output_init();
                        if (res != 0)
                        {
                            return 1;
                        }
                        res = ds3231_output_get_temperature((int16_t *)&raw, (float *)&s);
                        if (res != 0)
                        {
                            (void)ds3231_output_deinit();
                            
                            return 1;
                        }
                        ds3231_interface_debug_print("ds3231: temperature is %0.2fC.\n", s);
                        (void)ds3231_output_deinit();
                        
                        return 0;
                    }
                    else if (strcmp("-1Hz", argv[4]) == 0)
                    {
                        ds3231_bool_t enable;
                        
                        res = ds3231_output_init();
                        if (res != 0)
                        {
                            return 1;
                        }
                        res = ds3231_output_get_square_wave(&enable);
                        if (res != 0)
                        {
                            (void)ds3231_output_deinit();
                            
                            return 1;
                        }
                        ds3231_interface_debug_print("ds3231: get 1Hz output %s.\n", (enable == DS3231_BOOL_TRUE) ? "enable" : "disable");
                        (void)ds3231_output_deinit();
                        
                        return 0;
                    }
                    else if (strcmp("-32KHz", argv[4]) == 0)
                    {
                        ds3231_bool_t enable;
                        
                        res = ds3231_output_init();
                        if (res != 0)
                        {
                            return 1;
                        }
                        res = ds3231_output_get_32khz_output(&enable);
                        if (res != 0)
                        {
                            (void)ds3231_output_deinit();
                            
                            return 1;
                        }
                        ds3231_interface_debug_print("ds3231: get 32KHz output %s.\n", (enable == DS3231_BOOL_TRUE) ? "enable" : "disable");
                        (void)ds3231_output_deinit();
                        
                        return 0;
                    }
                    /* param is invalid */
                    else
                    {
                        return 5;
                    }
                }
                /* param is invalid */
                else
                {
                    return 5;
                }
            }
            /* param is invalid */
            else
            {
                return 5;
            }
        }
        /* param is invalid */
        else
        {
            return 5;
        }
    }
    else if (argc == 6)
    {
        /* run function */
        if (strcmp("-c", argv[1]) == 0)
        {
            uint8_t res;
            
            /* basic function */
            if (strcmp("basic", argv[2]) == 0)
            {
                if (strcmp("-set", argv[3]) == 0)
                {
                    if (strcmp("-time", argv[4]) == 0)
                    {
                        res = ds3231_basic_init();
                        if (res != 0)
                        {
                            return 1;
                        }
                        res = ds3231_basic_set_timestamp_time_zone(0);
                        if (res != 0)
                        {
                            (void)ds3231_basic_deinit();
                            
                            return 1;
                        }
                        res = ds3231_basic_set_timestamp((time_t)(atoi(argv[5])));
                        if (res != 0)
                        {
                            (void)ds3231_basic_deinit();
                            
                            return 1;
                        }
                        ds3231_interface_debug_print("ds3231: set timestamp %d.\n", (time_t)(atoi(argv[5])));
                        (void)ds3231_basic_deinit();
                        
                        return 0;
                    }
                    /* param is invalid */
                    else
                    {
                        return 5;
                    }
                }
                /* param is invalid */
                else
                {
                    return 5;
                }
            }
            /* alarm function */
            else if (strcmp("alarm", argv[2]) == 0)
            {
                if (strcmp("-set", argv[3]) == 0)
                {
                    if (strcmp("-time", argv[4]) == 0)
                    {
                        res = ds3231_alarm_init(ds3231_interface_receive_callback);
                        if (res != 0)
                        {
                            return 1;
                        }
                        res = ds3231_alarm_set_timestamp_time_zone(0);
                        if (res != 0)
                        {
                            (void)ds3231_alarm_deinit();
                            
                            return 1;
                        }
                        res = ds3231_alarm_set_timestamp((time_t)(atoi(argv[5])));
                        if (res != 0)
                        {
                            (void)ds3231_alarm_deinit();
                            
                            return 1;
                        }
                        ds3231_interface_debug_print("ds3231: set timestamp %d.\n", (time_t)(atoi(argv[5])));
                        (void)ds3231_alarm_deinit();
                        
                        return 0;
                    }
                    else if (strcmp("-alarm1", argv[4]) == 0)
                    {
                        ds3231_bool_t enable;
                        
                        if (strcmp("enable", argv[5]) == 0)
                        {
                            enable = DS3231_BOOL_TRUE;
                        }
                        else if (strcmp("disable", argv[5]) == 0)
                        {
                            enable = DS3231_BOOL_FALSE;
                        }
                        else
                        {
                            ds3231_interface_debug_print("ds3231: param is invalid.\n");
                            
                            return 5;
                        }
                        res = ds3231_alarm_init(a_alarm_receive_callback);
                        if (res != 0)
                        {
                            return 1;
                        }
                        if (enable == DS3231_BOOL_TRUE)
                        {
                            res = gpio_interrupt_init();
                            if (res != 0)
                            {
                                (void)ds3231_alarm_deinit();
                                
                                return 1;
                            }
                            g_gpio_irq = ds3231_alarm_irq_handler;
                            res = ds3231_alarm_enable(DS3231_ALARM_1);
                            if (res != 0)
                            {
                                (void)ds3231_alarm_deinit();
                                
                                return 1;
                            }
                        }
                        else
                        {
                            res = ds3231_alarm_disable(DS3231_ALARM_1);
                            if (res != 0)
                            {
                                (void)ds3231_alarm_deinit();
                                
                                return 1;
                            }
                            res = gpio_interrupt_deinit();
                            if (res != 0)
                            {
                                (void)ds3231_alarm_deinit();
                                
                                return 1;
                            }
                            g_gpio_irq = NULL;
                            (void)ds3231_alarm_deinit();
                        }
                        ds3231_interface_debug_print("ds3231: set alarm1 %s.\n", (enable == DS3231_BOOL_TRUE) ? "enable" : "disable");

                        return 0;
                    }
                    else if (strcmp("-alarm2", argv[4]) == 0)
                    {
                        ds3231_bool_t enable;
                        
                        if (strcmp("enable", argv[5]) == 0)
                        {
                            enable = DS3231_BOOL_TRUE;
                        }
                        else if (strcmp("disable", argv[5]) == 0)
                        {
                            enable = DS3231_BOOL_FALSE;
                        }
                        else
                        {
                            ds3231_interface_debug_print("ds3231: param is invalid.\n");
                            
                            return 5;
                        }
                        res = ds3231_alarm_init(a_alarm_receive_callback);
                        if (res != 0)
                        {
                            return 1;
                        }
                        if (enable == DS3231_BOOL_TRUE)
                        {
                            res = gpio_interrupt_init();
                            if (res != 0)
                            {
                                (void)ds3231_alarm_deinit();
                                
                                return 1;
                            }
                            g_gpio_irq = ds3231_alarm_irq_handler;
                            res = ds3231_alarm_enable(DS3231_ALARM_2);
                            if (res != 0)
                            {
                                (void)ds3231_alarm_deinit();
                                
                                return 1;
                            }
                        }
                        else
                        {
                            res = ds3231_alarm_disable(DS3231_ALARM_2);
                            if (res != 0)
                            {
                                (void)ds3231_alarm_deinit();
                                
                                return 1;
                            }
                            res = gpio_interrupt_deinit();
                            if (res != 0)
                            {
                                (void)ds3231_alarm_deinit();
                                
                                return 1;
                            }
                            g_gpio_irq = NULL;
                            (void)ds3231_alarm_deinit();
                        }
                        ds3231_interface_debug_print("ds3231: set alarm2 %s.\n", (enable == DS3231_BOOL_TRUE) ? "enable" : "disable");

                        return 0;
                    }
                    /* param is invalid */
                    else
                    {
                        return 5;
                    }
                }
                /* param is invalid */
                else
                {
                    return 5;
                }
            }
            /* output function */
            else if (strcmp("output", argv[2]) == 0)
            {
                if (strcmp("-set", argv[3]) == 0)
                {
                    if (strcmp("-time", argv[4]) == 0)
                    {
                        res = ds3231_output_init();
                        if (res != 0)
                        {
                            return 1;
                        }
                        res = ds3231_output_set_timestamp_time_zone(0);
                        if (res != 0)
                        {
                            (void)ds3231_output_deinit();
                            
                            return 1;
                        }
                        res = ds3231_output_set_timestamp((time_t)(atoi(argv[5])));
                        if (res != 0)
                        {
                            (void)ds3231_output_deinit();
                            
                            return 1;
                        }
                        ds3231_interface_debug_print("ds3231: set timestamp %d.\n", (time_t)(atoi(argv[5])));
                        (void)ds3231_output_deinit();
                        
                        return 0;
                    }
                    else if (strcmp("-1Hz", argv[4]) == 0)
                    {
                        ds3231_bool_t enable;
                        
                        if (strcmp("enable", argv[5]) == 0)
                        {
                            enable = DS3231_BOOL_TRUE;
                        }
                        else if (strcmp("disable", argv[5]) == 0)
                        {
                            enable = DS3231_BOOL_FALSE;
                        }
                        else
                        {
                            ds3231_interface_debug_print("ds3231: param is invalid.\n");
                            
                            return 5;
                        }
                        res = ds3231_output_init();
                        if (res != 0)
                        {
                            return 1;
                        }
                        res = ds3231_output_set_square_wave(enable);
                        if (res != 0)
                        {
                            (void)ds3231_output_deinit();
                            
                            return 1;
                        }
                        ds3231_interface_debug_print("ds3231: set 1Hz output %s.\n", (enable == DS3231_BOOL_TRUE) ? "enable" : "disable");
                        (void)ds3231_output_deinit();
                        
                        return 0;
                    }
                    else if (strcmp("-32KHz", argv[4]) == 0)
                    {
                        ds3231_bool_t enable;
                        
                        if (strcmp("enable", argv[5]) == 0)
                        {
                            enable = DS3231_BOOL_TRUE;
                        }
                        else if (strcmp("disable", argv[5]) == 0)
                        {
                            enable = DS3231_BOOL_FALSE;
                        }
                        else
                        {
                            ds3231_interface_debug_print("ds3231: param is invalid.\n");
                            
                            return 5;
                        }
                        res = ds3231_output_init();
                        if (res != 0)
                        {
                            return 1;
                        }
                        res = ds3231_output_set_32khz_output(enable);
                        if (res != 0)
                        {
                            (void)ds3231_output_deinit();
                            
                            return 1;
                        }
                        ds3231_interface_debug_print("ds3231: set 32KHz output %s.\n", (enable == DS3231_BOOL_TRUE) ? "enable" : "disable");
                        (void)ds3231_output_deinit();
                        
                        return 0;
                    }
                    /* param is invalid */
                    else
                    {
                        return 5;
                    }
                }
                /* param is invalid */
                else
                {
                    return 5;
                }
            }
            /* param is invalid */
            else
            {
                return 5;
            }
        }
        /* param is invalid */
        else
        {
            return 5;
        }
    }
    else if (argc == 8)
    {
        /* run function */
        if (strcmp("-c", argv[1]) == 0)
        {
            uint8_t res;
            
            /* alarm function */
            if (strcmp("alarm", argv[2]) == 0)
            {
                if (strcmp("-set", argv[3]) == 0)
                {
                    if (strcmp("-alarm1", argv[4]) == 0)
                    {
                        ds3231_alarm1_mode_t mode;
                        ds3231_time_t time;
                        struct tm *timeptr;
                        time_t timestamp;
                        
                        if (strcmp("-m", argv[6]) != 0)
                        {
                            return 5;
                        }
                        if (strcmp("ONCE_A_SECOND", argv[7]) == 0)
                        {
                            mode = DS3231_ALARM1_MODE_ONCE_A_SECOND;
                        }
                        else if (strcmp("SECOND_MATCH", argv[7]) == 0)
                        {
                            mode = DS3231_ALARM1_MODE_SECOND_MATCH;
                        }
                        else if (strcmp("MINUTE_SECOND_MATCH", argv[7]) == 0)
                        {
                            mode = DS3231_ALARM1_MODE_MINUTE_SECOND_MATCH;
                        }
                        else if (strcmp("HOUR_MINUTE_SECOND_MATCH", argv[7]) == 0)
                        {
                            mode = DS3231_ALARM1_MODE_HOUR_MINUTE_SECOND_MATCH;
                        }
                        else if (strcmp("DATE_HOUR_MINUTE_SECOND_MATCH", argv[7]) == 0)
                        {
                            mode = DS3231_ALARM1_MODE_DATE_HOUR_MINUTE_SECOND_MATCH;
                        }
                        else if (strcmp("WEEK_HOUR_MINUTE_SECOND_MATCH", argv[7]) == 0)
                        {
                            mode = DS3231_ALARM1_MODE_WEEK_HOUR_MINUTE_SECOND_MATCH;
                        }
                        else
                        {
                            ds3231_interface_debug_print("ds3231: mode is invalid.\n");
                            
                            return 5;
                        }
                        /* convert times */
                        timestamp = atoi(argv[5]);
                        timestamp += 8 * 3600;
                        timeptr = localtime(&timestamp);
                        time.am_pm = DS3231_AM;
                        time.date = (uint8_t)timeptr->tm_mday;
                        time.format = DS3231_FORMAT_24H;
                        time.hour = (uint8_t)timeptr->tm_hour;
                        time.minute = (uint8_t)timeptr->tm_min;
                        time.month = (uint8_t)(timeptr->tm_mon + 1);
                        time.second = (uint8_t)timeptr->tm_sec;
                        if (timeptr->tm_wday == 0)
                        {
                            time.week  = 7;
                        }
                        else
                        {
                            time.week = (uint8_t)timeptr->tm_wday;
                        }
                        time.year = (uint16_t)(timeptr->tm_year + 1900);
                        res = ds3231_alarm_init(a_alarm_receive_callback);
                        if (res != 0)
                        {
                            return 1;
                        }
                        res = ds3231_alarm_set_alarm1(&time, mode);
                        if (res != 0)
                        {
                            (void)ds3231_alarm_deinit();
                            
                            return 1;
                        }
                        ds3231_interface_debug_print("ds3231: set alarm1 timestamp %d mode %s.\n", timestamp, argv[7]);
                        (void)ds3231_alarm_deinit();
                        
                        return 0;
                    }
                    else if (strcmp("-alarm2", argv[4]) == 0)
                    {
                        ds3231_alarm2_mode_t mode;
                        ds3231_time_t time;
                        struct tm *timeptr;
                        time_t timestamp;
                        
                        if (strcmp("-m", argv[6]) != 0)
                        {
                            return 5;
                        }
                        if (strcmp("ONCE_A_MINUTE", argv[7]) == 0)
                        {
                            mode = DS3231_ALARM2_MODE_ONCE_A_MINUTE;
                        }
                        else if (strcmp("MINUTE_MATCH", argv[7]) == 0)
                        {
                            mode = DS3231_ALARM2_MODE_MINUTE_MATCH;
                        }
                        else if (strcmp("HOUR_MINUTE_MATCH", argv[7]) == 0)
                        {
                            mode = DS3231_ALARM2_MODE_HOUR_MINUTE_MATCH;
                        }
                        else if (strcmp("HOUR_MINUTE_MATCH", argv[7]) == 0)
                        {
                            mode = DS3231_ALARM2_MODE_DATE_HOUR_MINUTE_MATCH;
                        }
                        else if (strcmp("WEEK_HOUR_MINUTE_MATCH", argv[7]) == 0)
                        {
                            mode = DS3231_ALARM2_MODE_WEEK_HOUR_MINUTE_MATCH;
                        }
                        else
                        {
                            ds3231_interface_debug_print("ds3231: mode is invalid.\n");
                            
                            return 5;
                        }
                        /* convert times */
                        timestamp = atoi(argv[5]);
                        timestamp += 8 * 3600;
                        timeptr = localtime(&timestamp);
                        time.am_pm = DS3231_AM;
                        time.date = (uint8_t)timeptr->tm_mday;
                        time.format = DS3231_FORMAT_24H;
                        time.hour = (uint8_t)timeptr->tm_hour;
                        time.minute = (uint8_t)timeptr->tm_min;
                        time.month = (uint8_t)(timeptr->tm_mon + 1);
                        time.second = (uint8_t)timeptr->tm_sec;
                        if (timeptr->tm_wday == 0)
                        {
                            time.week  = 7;
                        }
                        else
                        {
                            time.week = (uint8_t)timeptr->tm_wday;
                        }
                        time.year = (uint16_t)(timeptr->tm_year + 1900);
                        res = ds3231_alarm_init(a_alarm_receive_callback);
                        if (res != 0)
                        {
                            return 1;
                        }
                        res = ds3231_alarm_set_alarm2(&time, mode);
                        if (res != 0)
                        {
                            (void)ds3231_alarm_deinit();
                            
                            return 1;
                        }
                        ds3231_interface_debug_print("ds3231: set alarm2 timestamp %d mode %s.\n", timestamp, argv[7]);
                        (void)ds3231_alarm_deinit();
                        
                        return 0;
                    }
                    /* param is invalid */
                    else
                    {
                        return 5;
                    }
                }
                /* param is invalid */
                else
                {
                    return 5;
                }
            }
            /* param is invalid */
            else
            {
                return 5;
            }
        }
        /* param is invalid */
        else
        {
            return 5;
        }
    }
    /* param is invalid */
    else
    {
        return 5;
    }
}

/**
 * @brief     main function
 * @param[in] argc is arg numbers
 * @param[in] **argv is the arg address
 * @return    status code
 *             - 0 success
 * @note      none
 */
int main(uint8_t argc, char **argv)
{
    uint8_t res;

    res = ds3231(argc, argv);
    if (res == 0)
    {
        /* run success */
    }
    else if (res == 1)
    {
        ds3231_interface_debug_print("ds3231: run failed.\n");
    }
    else if (res == 5)
    {
        ds3231_interface_debug_print("ds3231: param is invalid.\n");
    }
    else
    {
        ds3231_interface_debug_print("ds3231: unknow status code.\n");
    }

    return 0;
}