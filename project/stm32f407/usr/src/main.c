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
 * @version   1.0.0
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
#include "shell.h"
#include "clock.h"
#include "delay.h"
#include "gpio.h"
#include "uart.h"
#include "getopt.h"
#include <stdlib.h>

/**
 * @brief global var definition
 */
uint8_t g_buf[256];                        /**< uart buffer */
volatile uint16_t g_len;                   /**< uart buffer length */
uint8_t (*g_gpio_irq)(void) = NULL;        /**< gpio irq function address */

/**
 * @brief exti 0 irq
 * @note  none
 */
void EXTI0_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

/**
 * @brief     gpio exti callback
 * @param[in] pin is the gpio pin
 * @note      none
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if(GPIO_Pin == GPIO_PIN_0)
    {
        if (g_gpio_irq != NULL)
        {
            g_gpio_irq();
        }
    }
}

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
 *            - 0 success
 *            - 1 run failed
 *            - 5 param is invalid
 * @note      none
 */
uint8_t ds3231(uint8_t argc, char **argv)
{
    int c;
    int longindex = 0;
    const char short_options[] = "hipe:t:";
    const struct option long_options[] =
    {
        {"help", no_argument, NULL, 'h'},
        {"information", no_argument, NULL, 'i'},
        {"port", no_argument, NULL, 'p'},
        {"example", required_argument, NULL, 'e'},
        {"test", required_argument, NULL, 't'},
        {"enable", required_argument, NULL, 1},
        {"mode1", required_argument, NULL, 2},
        {"mode2", required_argument, NULL, 3},
        {"times", required_argument, NULL, 4},
        {"timestamp", required_argument, NULL, 5},
        {NULL, 0, NULL, 0},
    };
    char type[33] = "unknown";
    char mode_name[32] = {0};
    uint32_t times = 3;
    uint32_t t = 0;
    uint8_t time_flag = 0;
    ds3231_bool_t enable = DS3231_BOOL_FALSE;
    uint8_t enable_flag = 0;
    ds3231_alarm1_mode_t mode1 = DS3231_ALARM1_MODE_ONCE_A_SECOND;
    uint8_t mode1_flag = 0;
    ds3231_alarm2_mode_t mode2 = DS3231_ALARM2_MODE_ONCE_A_MINUTE;
    uint8_t mode2_flag = 0;

    /* if no params */
    if (argc == 1)
    {
        /* goto the help */
        goto help;
    }

    /* init 0 */
    optind = 0;

    /* parse */
    do
    {
        /* parse the args */
        c = getopt_long(argc, argv, short_options, long_options, &longindex);

        /* judge the result */
        switch (c)
        {
            /* help */
            case 'h' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "h");

                break;
            }

            /* information */
            case 'i' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "i");

                break;
            }

            /* port */
            case 'p' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "p");

                break;
            }

            /* example */
            case 'e' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "e_%s", optarg);

                break;
            }

            /* test */
            case 't' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "t_%s", optarg);

                break;
            }

            /* enable */
            case 1 :
            {
                /* set enable */
                if (strcmp("true", optarg) == 0)
                {
                    enable = DS3231_BOOL_TRUE;
                    enable_flag = 1;
                }
                else if (strcmp("false", optarg) == 0)
                {
                    enable = DS3231_BOOL_FALSE;
                    enable_flag = 1;
                }
                else
                {
                    return 5;
                }

                break;
            }

            /* mode1 */
            case 2 :
            {
                /* set the mode1 */
                if (strcmp("ONCE_A_SECOND", optarg) == 0)
                {
                    mode1 = DS3231_ALARM1_MODE_ONCE_A_SECOND;
                    strcpy(mode_name, "ONCE_A_SECOND");
                    mode1_flag = 1;
                }
                else if (strcmp("SECOND_MATCH", optarg) == 0)
                {
                    mode1 = DS3231_ALARM1_MODE_SECOND_MATCH;
                    strcpy(mode_name, "SECOND_MATCH");
                    mode1_flag = 1;
                }
                else if (strcmp("MINUTE_SECOND_MATCH", optarg) == 0)
                {
                    mode1 = DS3231_ALARM1_MODE_MINUTE_SECOND_MATCH;
                    strcpy(mode_name, "MINUTE_SECOND_MATCH");
                    mode1_flag = 1;
                }
                else if (strcmp("HOUR_MINUTE_SECOND_MATCH", optarg) == 0)
                {
                    mode1 = DS3231_ALARM1_MODE_HOUR_MINUTE_SECOND_MATCH;
                    strcpy(mode_name, "HOUR_MINUTE_SECOND_MATCH");
                    mode1_flag = 1;
                }
                else if (strcmp("DATE_HOUR_MINUTE_SECOND_MATCH", optarg) == 0)
                {
                    mode1 = DS3231_ALARM1_MODE_DATE_HOUR_MINUTE_SECOND_MATCH;
                    strcpy(mode_name, "DATE_HOUR_MINUTE_SECOND_MATCH");
                    mode1_flag = 1;
                }
                else if (strcmp("WEEK_HOUR_MINUTE_SECOND_MATCH", optarg) == 0)
                {
                    mode1 = DS3231_ALARM1_MODE_WEEK_HOUR_MINUTE_SECOND_MATCH;
                    strcpy(mode_name, "WEEK_HOUR_MINUTE_SECOND_MATCH");
                    mode1_flag = 1;
                }
                else
                {
                    return 5;
                }

                break;
            }

            /* mode2 */
            case 3 :
            {
                /* set the mode2 */
                if (strcmp("ONCE_A_MINUTE", optarg) == 0)
                {
                    mode2 = DS3231_ALARM2_MODE_ONCE_A_MINUTE;
                    strcpy(mode_name, "ONCE_A_MINUTE");
                    mode2_flag = 1;
                }
                else if (strcmp("MINUTE_MATCH", optarg) == 0)
                {
                    mode2 = DS3231_ALARM2_MODE_MINUTE_MATCH;
                    strcpy(mode_name, "MINUTE_MATCH");
                    mode2_flag = 1;
                }
                else if (strcmp("HOUR_MINUTE_MATCH", optarg) == 0)
                {
                    mode2 = DS3231_ALARM2_MODE_HOUR_MINUTE_MATCH;
                    strcpy(mode_name, "HOUR_MINUTE_MATCH");
                    mode2_flag = 1;
                }
                else if (strcmp("DATE_HOUR_MINUTE_MATCH", optarg) == 0)
                {
                    mode2 = DS3231_ALARM2_MODE_DATE_HOUR_MINUTE_MATCH;
                    strcpy(mode_name, "DATE_HOUR_MINUTE_MATCH");
                    mode2_flag = 1;
                }
                else if (strcmp("WEEK_HOUR_MINUTE_MATCH", optarg) == 0)
                {
                    mode2 = DS3231_ALARM2_MODE_WEEK_HOUR_MINUTE_MATCH;
                    strcpy(mode_name, "WEEK_HOUR_MINUTE_MATCH");
                    mode2_flag = 1;
                }
                else
                {
                    return 5;
                }

                break;
            }

            /* running times */
            case 4 :
            {
                /* set the times */
                times = atol(optarg);

                break;
            }

            /* timestamp */
            case 5 :
            {
                /* set the timestamp */
                t = atoi(optarg);
                time_flag = 1;

                break;
            }

            /* the end */
            case -1 :
            {
                break;
            }

            /* others */
            default :
            {
                return 5;
            }
        }
    } while (c != -1);

    /* run the function */
    if (strcmp("t_reg", type) == 0)
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
    else if (strcmp("t_alarm", type) == 0)
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
    else if (strcmp("t_readwrite", type) == 0)
    {
        /* run readwrite test */
        if (ds3231_readwrite_test(times) != 0)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else if (strcmp("t_output", type) == 0)
    {
        /* run output test */
        if (ds3231_output_test(times) != 0)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else if (strcmp("e_basic-set-time", type) == 0)
    {
        uint8_t res;

        /* check the flag */
        if (time_flag != 1)
        {
            return 5;
        }

        /* basic init */
        res = ds3231_basic_init();
        if (res != 0)
        {
            return 1;
        }

        /* set the zone */
        res = ds3231_basic_set_timestamp_time_zone(8);
        if (res != 0)
        {
            (void)ds3231_basic_deinit();

            return 1;
        }

        /* set time */
        res = ds3231_basic_set_timestamp((time_t)t);
        if (res != 0)
        {
            (void)ds3231_basic_deinit();

            return 1;
        }

        /* output */
        ds3231_interface_debug_print("ds3231: set timestamp %d.\n", (time_t)t);

        /* basic deinit */
        (void)ds3231_basic_deinit();

        return 0;
    }
    else if (strcmp("e_basic-get-time", type) == 0)
    {
        uint8_t res;
        ds3231_time_t time;

        /* basic init */
        res = ds3231_basic_init();
        if (res != 0)
        {
            return 1;
        }

        /* basic get time */
        res = ds3231_basic_get_time(&time);
        if (res != 0)
        {
            (void)ds3231_basic_deinit();

            return 1;
        }

        /* output */
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

        /* basic deinit */
        (void)ds3231_basic_deinit();

        return 0;
    }
    else if (strcmp("e_basic-get-temperature", type) == 0)
    {
        uint8_t res;
        int16_t raw;
        float s;

        /* basic init */
        res = ds3231_basic_init();
        if (res != 0)
        {
            return 1;
        }

        /* read temperature */
        res = ds3231_basic_get_temperature((int16_t *)&raw, (float *)&s);
        if (res != 0)
        {
            (void)ds3231_basic_deinit();

            return 1;
        }

        /* output */
        ds3231_interface_debug_print("ds3231: temperature is %0.2fC.\n", s);

        /* basic deinit */
        (void)ds3231_basic_deinit();

        return 0;
    }
    else if (strcmp("e_alarm-set-time", type) == 0)
    {
        uint8_t res;

        /* check the flag */
        if (time_flag != 1)
        {
            return 5;
        }

        /* alarm init */
        res = ds3231_alarm_init(ds3231_interface_receive_callback);
        if (res != 0)
        {
            return 1;
        }

        /* set zone */
        res = ds3231_alarm_set_timestamp_time_zone(8);
        if (res != 0)
        {
            (void)ds3231_alarm_deinit();

            return 1;
        }

        /* set the timestamp */
        res = ds3231_alarm_set_timestamp((time_t)t);
        if (res != 0)
        {
            (void)ds3231_alarm_deinit();

            return 1;
        }

        /* output */
        ds3231_interface_debug_print("ds3231: set timestamp %d.\n", (time_t)t);

        /* alarm deinit */
        (void)ds3231_alarm_deinit();

        return 0;
    }
    else if (strcmp("e_alarm-get-time", type) == 0)
    {
        uint8_t res;
        ds3231_time_t time;

        /* alarm init */
        res = ds3231_alarm_init(ds3231_interface_receive_callback);
        if (res != 0)
        {
            return 1;
        }

        /* get time */
        res = ds3231_alarm_get_time(&time);
        if (res != 0)
        {
            (void)ds3231_alarm_deinit();

            return 1;
        }

        /* output */
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

        /* alarm deinit */
        (void)ds3231_alarm_deinit();

        return 0;
    }
    else if (strcmp("e_alarm-get-temperature", type) == 0)
    {
        uint8_t res;
        int16_t raw;
        float s;

        /* alarm init */
        res = ds3231_alarm_init(ds3231_interface_receive_callback);
        if (res != 0)
        {
            return 1;
        }

        /* get the temperature */
        res = ds3231_alarm_get_temperature((int16_t *)&raw, (float *)&s);
        if (res != 0)
        {
            (void)ds3231_alarm_deinit();

            return 1;
        }

        /* output */
        ds3231_interface_debug_print("ds3231: temperature is %0.2fC.\n", s);

        /* alarm deinit */
        (void)ds3231_alarm_deinit();

        return 0;
    }
    else if (strcmp("e_alarm-get-alarm1", type) == 0)
    {
        uint8_t res;
        ds3231_time_t time;

        /* alarm init */
        res = ds3231_alarm_init(ds3231_interface_receive_callback);
        if (res != 0)
        {
            return 1;
        }

        /* get alarm1 */
        res = ds3231_alarm_get_alarm1(&time, &mode1);
        if (res != 0)
        {
            (void)ds3231_alarm_deinit();

            return 1;
        }

        /* output */
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

        /* alarm deinit */
        (void)ds3231_alarm_deinit();

        return 0;
    }
    else if (strcmp("e_alarm-set-alarm1-mode", type) == 0)
    {
        uint8_t res;
        ds3231_time_t time;
        struct tm *timeptr;

        /* check the flag */
        if (time_flag != 1)
        {
            return 5;
        }

        /* check the flag */
        if (mode1_flag != 1)
        {
            return 5;
        }

        /* set the timestamp */
        t += 8 * 3600;
        timeptr = localtime((const time_t *)&t);
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

        /* alarm init */
        res = ds3231_alarm_init(a_alarm_receive_callback);
        if (res != 0)
        {
            return 1;
        }

        /* set the mode */
        res = ds3231_alarm_set_alarm1(&time, mode1);
        if (res != 0)
        {
            (void)ds3231_alarm_deinit();

            return 1;
        }

        /* output */
        ds3231_interface_debug_print("ds3231: set alarm1 timestamp %d mode %s.\n", t, mode_name);

        /* alarm deinit */
        (void)ds3231_alarm_deinit();

        return 0;
    }
    else if (strcmp("e_alarm-set-alarm1", type) == 0)
    {
        uint8_t res;

        /* check the flag */
        if (enable_flag != 1)
        {
            return 5;
        }

        /* alarm init */
        res = ds3231_alarm_init(a_alarm_receive_callback);
        if (res != 0)
        {
            return 1;
        }

        /* set enable */
        if (enable == DS3231_BOOL_TRUE)
        {
            /* gpio init */
            res = gpio_interrupt_init();
            if (res != 0)
            {
                (void)ds3231_alarm_deinit();

                return 1;
            }

            /* set gpio callback */
            g_gpio_irq = ds3231_alarm_irq_handler;

            /* enable */
            res = ds3231_alarm_enable(DS3231_ALARM_1);
            if (res != 0)
            {
                (void)ds3231_alarm_deinit();

                return 1;
            }
        }
        else
        {
            /* disable */
            res = ds3231_alarm_disable(DS3231_ALARM_1);
            if (res != 0)
            {
                (void)ds3231_alarm_deinit();

                return 1;
            }

            /* gpio deinit */
            res = gpio_interrupt_deinit();
            if (res != 0)
            {
                (void)ds3231_alarm_deinit();

                return 1;
            }
            g_gpio_irq = NULL;

            /* alarm deinit */
            (void)ds3231_alarm_deinit();
        }

        /* output */
        ds3231_interface_debug_print("ds3231: set alarm1 %s.\n", (enable == DS3231_BOOL_TRUE) ? "enable" : "disable");

        return 0;
    }
    else if (strcmp("e_alarm-get-alarm2", type) == 0)
    {
        uint8_t res;
        ds3231_time_t time;

        /* alarm init */
        res = ds3231_alarm_init(ds3231_interface_receive_callback);
        if (res != 0)
        {
            return 1;
        }

        /* get alarm2 */
        res = ds3231_alarm_get_alarm2(&time, &mode2);
        if (res != 0)
        {
            (void)ds3231_alarm_deinit();

            return 1;
        }

        /* output */
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

        /* alarm deinit */
        (void)ds3231_alarm_deinit();

        return 0;
    }
    else if (strcmp("e_alarm-set-alarm2-mode", type) == 0)
    {
        uint8_t res;
        ds3231_time_t time;
        struct tm *timeptr;

        /* check the flag */
        if (time_flag != 1)
        {
            return 5;
        }

        /* check the flag */
        if (mode2_flag != 1)
        {
            return 5;
        }

        /* set the timestamp */
        t += 8 * 3600;
        timeptr = localtime((const time_t *)&t);
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

        /* alarm init */
        res = ds3231_alarm_init(a_alarm_receive_callback);
        if (res != 0)
        {
            return 1;
        }

        /* set the mode */
        res = ds3231_alarm_set_alarm2(&time, mode2);
        if (res != 0)
        {
            (void)ds3231_alarm_deinit();

            return 1;
        }

        /* output */
        ds3231_interface_debug_print("ds3231: set alarm2 timestamp %d mode %s.\n", t, mode_name);

        /* alarm deinit */
        (void)ds3231_alarm_deinit();

        return 0;
    }
    else if (strcmp("e_alarm-set-alarm2", type) == 0)
    {
        uint8_t res;

        /* check the flag */
        if (enable_flag != 1)
        {
            return 5;
        }

        /* alarm init */
        res = ds3231_alarm_init(a_alarm_receive_callback);
        if (res != 0)
        {
            return 1;
        }

        /* set enable */
        if (enable == DS3231_BOOL_TRUE)
        {
            /* gpio init */
            res = gpio_interrupt_init();
            if (res != 0)
            {
                (void)ds3231_alarm_deinit();

                return 1;
            }

            /* set the gpio callback */
            g_gpio_irq = ds3231_alarm_irq_handler;

            /* enable */
            res = ds3231_alarm_enable(DS3231_ALARM_2);
            if (res != 0)
            {
                (void)ds3231_alarm_deinit();

                return 1;
            }
        }
        else
        {
            /* disable */
            res = ds3231_alarm_disable(DS3231_ALARM_2);
            if (res != 0)
            {
                (void)ds3231_alarm_deinit();

                return 1;
            }

            /* gpio deinit */
            res = gpio_interrupt_deinit();
            if (res != 0)
            {
                (void)ds3231_alarm_deinit();

                return 1;
            }
            g_gpio_irq = NULL;

            /* alarm deinit */
            (void)ds3231_alarm_deinit();
        }

        /* output */
        ds3231_interface_debug_print("ds3231: set alarm2 %s.\n", (enable == DS3231_BOOL_TRUE) ? "enable" : "disable");

        return 0;
    }
    else if (strcmp("e_output-set-time", type) == 0)
    {
        uint8_t res;

        /* check the flag */
        if (time_flag != 1)
        {
            return 5;
        }

        /* output init */
        res = ds3231_output_init();
        if (res != 0)
        {
            return 1;
        }

        /* set time zone */
        res = ds3231_output_set_timestamp_time_zone(8);
        if (res != 0)
        {
            (void)ds3231_output_deinit();

            return 1;
        }

        /* set the timestamp */
        res = ds3231_output_set_timestamp((time_t)t);
        if (res != 0)
        {
            (void)ds3231_output_deinit();

            return 1;
        }

        /* output */
        ds3231_interface_debug_print("ds3231: set timestamp %d.\n", (time_t)t);

        /* output deinit */
        (void)ds3231_output_deinit();

        return 0;
    }
    else if (strcmp("e_output-get-time", type) == 0)
    {
        uint8_t res;
        ds3231_time_t time;

        /* output init */
        res = ds3231_output_init();
        if (res != 0)
        {
            return 1;
        }

        /* get time */
        res = ds3231_output_get_time(&time);
        if (res != 0)
        {
            (void)ds3231_output_deinit();

            return 1;
        }

        /* output */
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

        /* output deinit */
        (void)ds3231_output_deinit();

        return 0;
    }
    else if (strcmp("e_output-get-temperature", type) == 0)
    {
        uint8_t res;
        int16_t raw;
        float s;

        /* output init */
        res = ds3231_output_init();
        if (res != 0)
        {
            return 1;
        }

        /* get temperature */
        res = ds3231_output_get_temperature((int16_t *)&raw, (float *)&s);
        if (res != 0)
        {
            (void)ds3231_output_deinit();

            return 1;
        }

        /* output */
        ds3231_interface_debug_print("ds3231: temperature is %0.2fC.\n", s);

        /* output deinit */
        (void)ds3231_output_deinit();

        return 0;
    }
    else if (strcmp("e_output-set-1Hz", type) == 0)
    {
        uint8_t res;

        /* check the flag */
        if (enable_flag != 1)
        {
            return 5;
        }

        /* output init */
        res = ds3231_output_init();
        if (res != 0)
        {
            return 1;
        }

        /* set enable */
        res = ds3231_output_set_square_wave(enable);
        if (res != 0)
        {
            (void)ds3231_output_deinit();

            return 1;
        }

        /* output */
        ds3231_interface_debug_print("ds3231: set 1Hz output %s.\n", (enable == DS3231_BOOL_TRUE) ? "enable" : "disable");

        /* output deinit */
        (void)ds3231_output_deinit();

        return 0;
    }
    else if (strcmp("e_output-get-1Hz", type) == 0)
    {
        uint8_t res;

        /* output init */
        res = ds3231_output_init();
        if (res != 0)
        {
            return 1;
        }

        /* get 1Hz */
        res = ds3231_output_get_square_wave(&enable);
        if (res != 0)
        {
            (void)ds3231_output_deinit();

            return 1;
        }

        /* output */
        ds3231_interface_debug_print("ds3231: get 1Hz output %s.\n", (enable == DS3231_BOOL_TRUE) ? "enable" : "disable");

        /* output deinit */
        (void)ds3231_output_deinit();

        return 0;
    }
    else if (strcmp("e_output-set-32KHz", type) == 0)
    {
        uint8_t res;

        /* check the flag */
        if (enable_flag != 1)
        {
            return 5;
        }

        /* output init */
        res = ds3231_output_init();
        if (res != 0)
        {
            return 1;
        }

        /* set enable */
        res = ds3231_output_set_32khz_output(enable);
        if (res != 0)
        {
            (void)ds3231_output_deinit();

            return 1;
        }

        /* output */
        ds3231_interface_debug_print("ds3231: set 32KHz output %s.\n", (enable == DS3231_BOOL_TRUE) ? "enable" : "disable");

        /* output deinit */
        (void)ds3231_output_deinit();

        return 0;
    }
    else if (strcmp("e_output-get-32KHz", type) == 0)
    {
        uint8_t res;

        /* output init */
        res = ds3231_output_init();
        if (res != 0)
        {
            return 1;
        }

        /* get 32KHz */
        res = ds3231_output_get_32khz_output(&enable);
        if (res != 0)
        {
            (void)ds3231_output_deinit();

            return 1;
        }

        /* output */
        ds3231_interface_debug_print("ds3231: get 32KHz output %s.\n", (enable == DS3231_BOOL_TRUE) ? "enable" : "disable");

        /* output deinit */
        (void)ds3231_output_deinit();

        return 0;
    }
    else if (strcmp("h", type) == 0)
    {
        help:
        ds3231_interface_debug_print("Usage:\n");
        ds3231_interface_debug_print("  ds3231 (-i | --information)\n");
        ds3231_interface_debug_print("  ds3231 (-h | --help)\n");
        ds3231_interface_debug_print("  ds3231 (-p | --port)\n");
        ds3231_interface_debug_print("  ds3231 (-t reg | --test=reg)\n");
        ds3231_interface_debug_print("  ds3231 (-t alarm | --test=alarm)\n");
        ds3231_interface_debug_print("  ds3231 (-t readwrite | --test=readwrite) [--times=<num>]\n");
        ds3231_interface_debug_print("  ds3231 (-t output | --test=output) [--times=<num>]\n");
        ds3231_interface_debug_print("  ds3231 (-e basic-set-time | --example=basic-set-time) --timestamp=<time>\n");
        ds3231_interface_debug_print("  ds3231 (-e basic-get-time | --example=basic-get-time)\n");
        ds3231_interface_debug_print("  ds3231 (-e basic-get-temperature | --example=basic-get-temperature)\n");
        ds3231_interface_debug_print("  ds3231 (-e alarm-set-time | --example=alarm-set-time) --timestamp=<time>\n");
        ds3231_interface_debug_print("  ds3231 (-e alarm-set-alarm1 | --example=alarm-set-alarm1) --enable=<true | false>\n");
        ds3231_interface_debug_print("  ds3231 (-e alarm-set-alarm2 | --example=alarm-set-alarm2) --enable=<true | false>\n");
        ds3231_interface_debug_print("  ds3231 (-e alarm-set-alarm1-mode | --example=alarm-set-alarm1-mode) --timestamp=<time>\n");
        ds3231_interface_debug_print("         --mode1=<ONCE_A_SECOND | SECOND_MATCH | MINUTE_SECOND_MATCH | HOUR_MINUTE_SECOND_MATCH | DATE_HOUR_MINUTE_SECOND_MATCH | WEEK_HOUR_MINUTE_SECOND_MATCH>\n");
        ds3231_interface_debug_print("  ds3231 (-e alarm-set-alarm2-mode | --example=alarm-set-alarm2-mode) --timestamp=<time>\n");
        ds3231_interface_debug_print("         --mode2=<ONCE_A_MINUTE | MINUTE_MATCH | HOUR_MINUTE_MATCH | DATE_HOUR_MINUTE_MATCH | WEEK_HOUR_MINUTE_MATCH>\n");
        ds3231_interface_debug_print("  ds3231 (-e alarm-get-time | --example=alarm-get-time)\n");
        ds3231_interface_debug_print("  ds3231 (-e alarm-get-temperature | --example=alarm-get-temperature)\n");
        ds3231_interface_debug_print("  ds3231 (-e alarm-get-alarm1 | --example=alarm-get-alarm1)\n");
        ds3231_interface_debug_print("  ds3231 (-e alarm-get-alarm2 | --example=alarm-get-alarm2)\n");
        ds3231_interface_debug_print("  ds3231 (-e output-set-time | --example=output-set-time) --timestamp=<time>\n");
        ds3231_interface_debug_print("  ds3231 (-e output-set-1Hz | --example=output-set-1Hz) --enable=<true | false>\n");
        ds3231_interface_debug_print("  ds3231 (-e output-set-32KHz | --example=output-set-32KHz) --enable=<true | false>\n");
        ds3231_interface_debug_print("  ds3231 (-e output-get-time | --example=output-get-time)\n");
        ds3231_interface_debug_print("  ds3231 (-e output-get-1Hz | --example=output-get-1Hz)\n");
        ds3231_interface_debug_print("  ds3231 (-e output-get-32KHz | --example=output-get-32KHz)\n");
        ds3231_interface_debug_print("  ds3231 (-e output-get-temperature | --example=output-get-temperature)\n");
        ds3231_interface_debug_print("\n");
        ds3231_interface_debug_print("Options:\n");
        ds3231_interface_debug_print("  -e <basic-set-time | basic-get-time | basic-get-temperature | alarm-set-time | alarm-set-alarm1 | alarm-set-alarm2 | alarm-set-alarm1-mode\n");
        ds3231_interface_debug_print("     | alarm-set-alarm2-mode | alarm-get-time | alarm-get-temperature | alarm-get-alarm1 | alarm-get-alarm2 | output-set-time | output-set-1Hz\n");
        ds3231_interface_debug_print("     | output-set-32KHz | output-get-time | output-get-1Hz | output-get-32KHz | output-get-temperature>, --example=<basic-set-time | basic-get-time \n");
        ds3231_interface_debug_print("     | basic-get-temperature | alarm-set-time | alarm-set-alarm1 | alarm-set-alarm2 | alarm-set-alarm1-mode | alarm-set-alarm2-mode | alarm-get-time \n");
        ds3231_interface_debug_print("     | alarm-get-temperature | alarm-get-alarm1 | alarm-get-alarm2 | output-set-time | output-set-1Hz | output-set-32KHz | output-get-time \n");
        ds3231_interface_debug_print("     | output-get-1Hz | output-get-32KHz | output-get-temperature>\n");
        ds3231_interface_debug_print("                                  Run the driver example.\n");
        ds3231_interface_debug_print("      --enable=<true | false>     Enable or disable the function.\n");
        ds3231_interface_debug_print("  -h, --help                      Show the help.\n");
        ds3231_interface_debug_print("  -i, --information               Show the chip information.\n");
        ds3231_interface_debug_print("      --mode1=<ONCE_A_SECOND | SECOND_MATCH | MINUTE_SECOND_MATCH | HOUR_MINUTE_SECOND_MATCH\n");
        ds3231_interface_debug_print("              | DATE_HOUR_MINUTE_SECOND_MATCH | WEEK_HOUR_MINUTE_SECOND_MATCH>\n");
        ds3231_interface_debug_print("                                  Set the alarm1 mode.\n");
        ds3231_interface_debug_print("      --mode2=<ONCE_A_MINUTE | MINUTE_MATCH | HOUR_MINUTE_MATCH | DATE_HOUR_MINUTE_MATCH | WEEK_HOUR_MINUTE_MATCH>\n");
        ds3231_interface_debug_print("                                  Set the alarm2 mode.\n");
        ds3231_interface_debug_print("  -p, --port                      Display the pin connections of the current board.\n");
        ds3231_interface_debug_print("  -t <reg | alarm | readwrite | output>, --test=<reg | alarm | readwrite | output>\n");
        ds3231_interface_debug_print("                                  Run the driver test.\n");
        ds3231_interface_debug_print("      --times=<num>               Set the running times.([default: 3])\n");
        ds3231_interface_debug_print("      --timestamp=<time>          Set the the unix timestamp.\n");

        return 0;
    }
    else if (strcmp("i", type) == 0)
    {
        ds3231_info_t info;

        /* print ds3231 info */
        ds3231_info(&info);
        ds3231_interface_debug_print("ds3231: chip is %s.\n", info.chip_name);
        ds3231_interface_debug_print("ds3231: manufacturer is %s.\n", info.manufacturer_name);
        ds3231_interface_debug_print("ds3231: interface is %s.\n", info.interface);
        ds3231_interface_debug_print("ds3231: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        ds3231_interface_debug_print("ds3231: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        ds3231_interface_debug_print("ds3231: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        ds3231_interface_debug_print("ds3231: max current is %0.2fmA.\n", info.max_current_ma);
        ds3231_interface_debug_print("ds3231: max temperature is %0.1fC.\n", info.temperature_max);
        ds3231_interface_debug_print("ds3231: min temperature is %0.1fC.\n", info.temperature_min);

        return 0;
    }
    else if (strcmp("p", type) == 0)
    {
        /* print pin connection */
        ds3231_interface_debug_print("ds3231: SCL connected to GPIOB PIN8.\n");
        ds3231_interface_debug_print("ds3231: SDA connected to GPIOB PIN9.\n");
        ds3231_interface_debug_print("ds3231: INT connected to GPIOB PIN0.\n");

        return 0;
    }
    else
    {
        return 5;
    }
}

/**
 * @brief main function
 * @note  none
 */
int main(void)
{
    uint8_t res;

    /* stm32f407 clock init and hal init */
    clock_init();

    /* delay init */
    delay_init();

    /* uart init */
    uart_init(115200);

    /* shell init && register ds3231 function */
    shell_init();
    shell_register("ds3231", ds3231);
    uart_print("ds3231: welcome to libdriver ds3231.\n");

    while (1)
    {
        /* read uart */
        g_len = uart_read(g_buf, 256);
        if (g_len != 0)
        {
            /* run shell */
            res = shell_parse((char *)g_buf, g_len);
            if (res == 0)
            {
                /* run success */
            }
            else if (res == 1)
            {
                uart_print("ds3231: run failed.\n");
            }
            else if (res == 2)
            {
                uart_print("ds3231: unknown command.\n");
            }
            else if (res == 3)
            {
                uart_print("ds3231: length is too long.\n");
            }
            else if (res == 4)
            {
                uart_print("ds3231: pretreat failed.\n");
            }
            else if (res == 5)
            {
                uart_print("ds3231: param is invalid.\n");
            }
            else
            {
                uart_print("ds3231: unknown status code.\n");
            }
            uart_flush();
        }
        delay_ms(100);
    }
}
