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
 * @file      driver_ds3231.c
 * @brief     driver ds3231 source file
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

#include "driver_ds3231.h"

/**
 * @brief chip information definition
 */
#define CHIP_NAME                 "Maxim Integrated DS3231"        /**< chip name */
#define MANUFACTURER_NAME         "Maxim Integrated"               /**< manufacturer name */
#define SUPPLY_VOLTAGE_MIN        2.3f                             /**< chip min supply voltage */
#define SUPPLY_VOLTAGE_MAX        5.5f                             /**< chip max supply voltage */
#define MAX_CURRENT               0.65f                            /**< chip max current */
#define TEMPERATURE_MIN           -40.0f                           /**< chip min operating temperature */
#define TEMPERATURE_MAX           85.0f                            /**< chip max operating temperature */
#define DRIVER_VERSION            2000                             /**< driver version */

/**
 * @brief chip register definition
 */
#define DS3231_REG_SECOND               0x00        /**< second register */
#define DS3231_REG_MINUTE               0x01        /**< minute register */
#define DS3231_REG_HOUR                 0x02        /**< hour register */
#define DS3231_REG_WEEK                 0x03        /**< week register */
#define DS3231_REG_DATE                 0x04        /**< date register */
#define DS3231_REG_MONTH                0x05        /**< month register */
#define DS3231_REG_YEAR                 0x06        /**< year register */
#define DS3231_REG_ALARM1_SECOND        0x07        /**< alarm1 second register */
#define DS3231_REG_ALARM1_MINUTE        0x08        /**< alarm1 minute register */
#define DS3231_REG_ALARM1_HOUR          0x09        /**< alarm1 hour register */
#define DS3231_REG_ALARM1_WEEK          0x0A        /**< alarm1 week register */
#define DS3231_REG_ALARM2_MINUTE        0x0B        /**< alarm2 minute register */
#define DS3231_REG_ALARM2_HOUR          0x0C        /**< alarm2 hour register */
#define DS3231_REG_ALARM2_WEEK          0x0D        /**< alarm2 week register */
#define DS3231_REG_CONTROL              0x0E        /**< control register */
#define DS3231_REG_STATUS               0x0F        /**< status register */
#define DS3231_REG_XTAL                 0x10        /**< xtal register */
#define DS3231_REG_TEMPERATUREH         0x11        /**< temperature high register */
#define DS3231_REG_TEMPERATUREL         0x12        /**< temperature low register */

/**
 * @brief chip address definition
 */
#define DS3231_ADDRESS        0xD0        /**< iic device address */ 

/**
 * @brief     write one byte
 * @param[in] *handle pointer to a ds3231 handle structure
 * @param[in] reg iic register address
 * @param[in] data written data
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
static uint8_t a_ds3231_iic_write(ds3231_handle_t *handle, uint8_t reg, uint8_t data)
{
    if (handle->iic_write(DS3231_ADDRESS, reg, &data, 1) != 0)        /* write data */
    {
        return 1;                                                     /* return error */
    }
    else
    {
        return 0;                                                     /* success return 0 */
    }
}

/**
 * @brief      read multiple bytes
 * @param[in]  *handle pointer to a ds3231 handle structure
 * @param[in]  reg iic register address
 * @param[out] *buf pointer to a data buffer
 * @param[in]  len data buffer length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
static uint8_t a_ds3231_iic_multiple_read(ds3231_handle_t *handle, uint8_t reg, uint8_t *buf, uint8_t len)
{
    if (handle->iic_read(DS3231_ADDRESS, reg, buf, len) != 0)        /* read data */
    {
        return 1;                                                    /* return error */
    }
    else
    {                                                                /* success return 0 */
        return 0;
    }
}

/**
 * @brief     hex to bcd
 * @param[in] val hex data
 * @return    bcd data
 * @note      none
 */
static uint8_t a_ds3231_hex2bcd(uint8_t val)
{
    uint8_t i, j, k;
    
    i = val / 10;            /* get tens place */
    j = val % 10;            /* get ones place */
    k = j + (i << 4);        /* set bcd */
    
    return k;                /* return bcd */
}

/**
 * @brief     bcd to hex
 * @param[in] val bcd data
 * @return    hex data
 * @note      none
 */
static uint8_t a_ds3231_bcd2hex(uint8_t val)
{
    uint8_t temp;
    
    temp = val & 0x0F;              /* get ones place */
    val = (val >> 4) & 0x0F;        /* get tens place */
    val = val * 10;                 /* set tens place */
    temp = temp + val;              /* get hex */
    
    return temp;                    /* return hex */
}

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
uint8_t ds3231_set_time(ds3231_handle_t *handle, ds3231_time_t *t)
{
    uint8_t res;
    uint8_t reg;
    uint8_t century;
    uint16_t year;
    
    if (handle == NULL)                                                                                      /* check handle */
    {
        return 2;                                                                                            /* return error */
    }
    if (handle->inited != 1)                                                                                 /* check handle initialization */
    {
        return 3;                                                                                            /* return error */
    }
    if (t == NULL)                                                                                           /* check time */
    {
        handle->debug_print("ds3231: time is null.\n");                                                      /* time is null */
        
        return 2;                                                                                            /* return error */
    }
    if (t->format == DS3231_FORMAT_12H)                                                                      /* if 12H */
    {
        if ((t->year < 2000) || (t->year > 2200))                                                            /* check year */
        {
            handle->debug_print("ds3231: year can't be over 2200 or less than 2000.\n");                     /* year can't be over 2200 or less than 2000 */
            
            return 4;                                                                                        /* return error */
        }
        if ((t->month == 0) || (t->month > 12))                                                              /* check month */
        {
            handle->debug_print("ds3231: month can't be zero or over than 12.\n");                           /* month can't be zero or over than 12 */
            
            return 4;                                                                                        /* return error */
        }
        if ((t->week == 0) || (t->week > 7))                                                                 /* check week */
        {
            handle->debug_print("ds3231: week can't be zero or over than 7.\n");                             /* week can't be zero or over than 7 */
            
            return 4;                                                                                        /* return error */
        }
        if ((t->date == 0) || (t->date > 31))                                                                /* check data */
        {
            handle->debug_print("ds3231: date can't be zero or over than 31.\n");                            /* date can't be zero or over than 31 */
            
            return 4;                                                                                        /* return error */
        }
        if ((t->hour < 1) || (t->hour > 12))                                                                 /* check hour */
        {
            handle->debug_print("ds3231: hour can't be over than 12 or less 1.\n");                          /* hour can't be over than 12 or less 1 */
            
            return 4;                                                                                        /* return error */
        }
        if (t->minute > 59)                                                                                  /* check minute */
        {
            handle->debug_print("ds3231: minute can't be over than 59.\n");                                  /* minute can't be over than 59 */
            
            return 4;                                                                                        /* return error */
        }
        if (t->second > 59)                                                                                  /* check second */
        {
            handle->debug_print("ds3231: second can't be over than 59.\n");                                  /* second can't be over than 59 */
            
            return 4;                                                                                        /* return error */
        }
    }
    else if (t->format == DS3231_FORMAT_24H)                                                                 /* if 24H */
    {
        if ((t->year < 2000) || (t->year > 2200))                                                            /* check year */
        {
            handle->debug_print("ds3231: year can't be over 2200 or less than 2000.\n");                     /* year can't be over 2200 or less than 2000 */
            
            return 4;                                                                                        /* return error */
        }
        if ((t->month == 0) || (t->month > 12))                                                              /* check month */
        {
            handle->debug_print("ds3231: month can't be zero or over than 12.\n");                           /* month can't be zero or over than 12 */
            
            return 4;                                                                                        /* return error */
        }
        if ((t->week == 0) || (t->week > 7))                                                                 /* check week */
        {
            handle->debug_print("ds3231: week can't be zero or over than 7.\n");                             /* week can't be zero or over than 7 */
            
            return 4;                                                                                        /* return error */
        }
        if ((t->date == 0) || (t->date > 31))                                                                /* check data */
        {
            handle->debug_print("ds3231: date can't be zero or over than 31.\n");                            /* date can't be zero or over than 31 */
            
            return 4;                                                                                        /* return error */
        }
        if (t->hour > 23)                                                                                    /* check hour */
        {
            handle->debug_print("ds3231: hour can't be over than 23.\n");                                    /* hour can't be over than 23 */
            
            return 4;                                                                                        /* return error */
        }
        if (t->minute > 59)                                                                                  /* check minute */
        {
            handle->debug_print("ds3231: minute can't be over than 59.\n");                                  /* minute can't be over than 59 */
            
            return 4;                                                                                        /* return error */
        }
        if (t->second > 59)                                                                                  /* check second */
        {
            handle->debug_print("ds3231: second can't be over than 59.\n");                                  /* second can't be over than 59 */
            
            return 4;                                                                                        /* return error */
        }
    }
    else
    {
        handle->debug_print("ds3231: format is invalid.\n");                                                 /* format is invalid */
        
        return 4;                                                                                            /* return error */
    }
    
    res = a_ds3231_iic_write(handle, DS3231_REG_SECOND, a_ds3231_hex2bcd(t->second));                        /* write second */
    if (res != 0)                                                                                            /* check result */
    {
        handle->debug_print("ds3231: write second failed.\n");                                               /* write second failed */
        
        return 1;                                                                                            /* return error */
    }
    res = a_ds3231_iic_write(handle, DS3231_REG_MINUTE, a_ds3231_hex2bcd(t->minute));                        /* write minute */
    if (res != 0)                                                                                            /* check result */
    {
        handle->debug_print("ds3231: write minute failed.\n");                                               /* write minute failed */
        
        return 1;                                                                                            /* return error */
    }
    if (t->format == DS3231_FORMAT_12H)                                                                      /* if 12H */
    {
        reg = (uint8_t)((1 << 6) | (t->am_pm << 5) | a_ds3231_hex2bcd(t->hour));                             /* set hour in 12H */
    }
    else                                                                                                     /* if 24H */
    {
        reg = (0 << 6) | a_ds3231_hex2bcd(t->hour);                                                          /* set hour in 24H */
    }
    res = a_ds3231_iic_write(handle, DS3231_REG_HOUR, reg);                                                  /* write hour */
    if (res != 0)                                                                                            /* check result */
    {
        handle->debug_print("ds3231: write hour failed.\n");                                                 /* write hour failed */
        
        return 1;                                                                                            /* return error */
    }
    res = a_ds3231_iic_write(handle, DS3231_REG_WEEK, a_ds3231_hex2bcd(t->week));                            /* write week */
    if (res != 0)                                                                                            /* check result */
    {
        handle->debug_print("ds3231: write week failed.\n");                                                 /* write week failed */
        
        return 1;                                                                                            /* return error */
    }
    res = a_ds3231_iic_write(handle, DS3231_REG_DATE, a_ds3231_hex2bcd(t->date));                            /* write data */
    if (res != 0)                                                                                            /* check result */
    {
        handle->debug_print("ds3231: write date failed.\n");                                                 /* write date failed */
        
        return 1;                                                                                            /* return error */
    }
    year = t->year - 2000;                                                                                   /* year - 2000 */
    if (year >= 100)                                                                                         /* check year */
    {
        century = 1;                                                                                         /* set century */
        year -= 100;                                                                                         /* year -= 100 */
    }
    else
    {
        century = 0;                                                                                         /* set century 0 */
    }
    res = a_ds3231_iic_write(handle, DS3231_REG_MONTH, a_ds3231_hex2bcd(t->month) | (century << 7));         /* write month and century */
    if (res != 0)                                                                                            /* check result */
    {
        handle->debug_print("ds3231: write century and month failed.\n");                                    /* write century and month failed */
        
        return 1;                                                                                            /* return error */
    }
    res = a_ds3231_iic_write(handle, DS3231_REG_YEAR, a_ds3231_hex2bcd((uint8_t)year));                      /* write year */
    if (res != 0)                                                                                            /* check result */
    {
        handle->debug_print("ds3231: write year failed.\n");                                                 /* write year failed */
        
        return 1;                                                                                            /* return error */
    }
    
    return 0;                                                                                                /* success return 0 */
}

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
uint8_t ds3231_get_time(ds3231_handle_t *handle, ds3231_time_t *t)
{
    uint8_t res;
    uint8_t buf[7];
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    if (t == NULL)                                                                        /* check time */
    {
        handle->debug_print("ds3231: time is null.\n");                                   /* time is null */
        
        return 2;                                                                         /* return error */
    }
    
    memset(buf, 0, sizeof(uint8_t) * 7);                                                  /* clear the buffer */
    res = a_ds3231_iic_multiple_read(handle, DS3231_REG_SECOND, (uint8_t *)buf, 7);       /* multiple_read */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("ds3231: multiple read failed.\n");                           /* multiple read failed */
        
        return 1;                                                                         /* return error */
    }
    t->year = a_ds3231_bcd2hex(buf[6]) + 2000 + ((buf[5] >> 7) & 0x01) * 100;             /* get year */
    t->month = a_ds3231_bcd2hex(buf[5]&0x1F);                                             /* get month */
    t->week = a_ds3231_bcd2hex(buf[3]);                                                   /* get week */
    t->date = a_ds3231_bcd2hex(buf[4]);                                                   /* get date */
    t->am_pm = (ds3231_am_pm_t)((buf[2] >> 5) & 0x01);                                    /* get am pm */
    t->format = (ds3231_format_t)((buf[2] >> 6) & 0x01);                                  /* get format */
    if (t->format == DS3231_FORMAT_12H)                                                   /* if 12H */
    {
        t->hour = a_ds3231_bcd2hex(buf[2] & 0x1F);                                        /* get hour */
    }
    else
    {
        t->hour = a_ds3231_bcd2hex(buf[2] & 0x3F);                                        /* get hour */
    }
    t->minute = a_ds3231_bcd2hex(buf[1]);                                                 /* get minute */
    t->second = a_ds3231_bcd2hex(buf[0]);                                                 /* get second */
    
    return 0;                                                                             /* success return 0 */
}

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
uint8_t ds3231_set_alarm1(ds3231_handle_t *handle, ds3231_time_t *t, ds3231_alarm1_mode_t mode)
{
    uint8_t res;
    uint8_t reg;
    
    if (handle == NULL)                                                                                                            /* check handle */
    {
        return 2;                                                                                                                  /* return error */
    }
    if (handle->inited != 1)                                                                                                       /* check handle initialization */
    {
        return 3;                                                                                                                  /* return error */
    }
    if (t == NULL)                                                                                                                 /* check time */
    {
        handle->debug_print("ds3231: time is null.\n");                                                                            /* time is null */
        
        return 2;                                                                                                                  /* return error */
    }
    if (t->format == DS3231_FORMAT_12H)                                                                                            /* if 12H */
    {
        if ((t->week == 0) || (t->week > 7))                                                                                       /* check week */
        {
            handle->debug_print("ds3231: week can't be zero or over than 7.\n");                                                   /* week can't be zero or over than 7 */
            
            return 1;                                                                                                              /* return error */
        }
        if ((t->date == 0) || (t->date > 31))                                                                                      /* check data */
        {
            handle->debug_print("ds3231: date can't be zero or over than 31.\n");                                                  /* date can't be zero or over than 31 */
            
            return 1;                                                                                                              /* return error */
        }
        if ((t->hour < 1) || (t->hour > 12))                                                                                       /* check hour */
        {
            handle->debug_print("ds3231: hour can't be over than 12 or less 1.\n");                                                /* hour can't be over than 12 or less 1 */
            
            return 1;                                                                                                              /* return error */
        }
        if (t->minute > 59)                                                                                                        /* check minute */
        {
            handle->debug_print("ds3231: minute can't be over than 59.\n");                                                        /* minute can't be over than 59 */
            
            return 1;                                                                                                              /* return error */
        }
        if (t->second > 59)                                                                                                        /* check second  */
        {
            handle->debug_print("ds3231: second can't be over than 59.\n");                                                        /* second can't be over than 59 */
            
            return 1;                                                                                                              /* return error */
        }
    }
    else if (t->format == DS3231_FORMAT_24H)                                                                                       /* if 24H */
    {
        if ((t->week == 0) || (t->week > 7))                                                                                       /* check week */
        {
            handle->debug_print("ds3231: week can't be zero or over than 7.\n");                                                   /* week can't be zero or over than 7 */
            
            return 1;                                                                                                              /* return error */
        }
        if ((t->date == 0) || (t->date > 31))                                                                                      /* check data */
        {
            handle->debug_print("ds3231: date can't be zero or over than 31.\n");                                                  /* date can't be zero or over than 31 */
            
            return 1;                                                                                                              /* return error */
        }
        if (t->hour > 23)                                                                                                          /* check hour */
        {
            handle->debug_print("ds3231: hour can't be over than 23.\n");                                                          /* hour can't be over than 23 */
            
            return 1;                                                                                                              /* return error */
        }
        if (t->minute > 59)                                                                                                        /* check minute */
        {
            handle->debug_print("ds3231: minute can't be over than 59.\n");                                                        /* minute can't be over than 59 */
            
            return 1;                                                                                                              /* return error */
        }
        if (t->second > 59)                                                                                                        /* check second */
        {
            handle->debug_print("ds3231: second can't be over than 59.\n");                                                        /* second can't be over than 59 */
            
            return 1;                                                                                                              /* return error */
        }
    }
    else
    {
        handle->debug_print("ds3231: format is invalid.\n");                                                                       /* format is invalid */
        
        return 1;                                                                                                                  /* return error */
    }
    
    res = a_ds3231_iic_write(handle, DS3231_REG_ALARM1_SECOND, a_ds3231_hex2bcd(t->second) | ((mode & 0x01) << 7));                /* write second */
    if (res != 0)                                                                                                                  /* check result */
    {
        handle->debug_print("ds3231: write alarm1 second failed.\n");                                                              /* write alarm1 second failed */
        
        return 1;                                                                                                                  /* return error */
    }
    res = a_ds3231_iic_write(handle, DS3231_REG_ALARM1_MINUTE, a_ds3231_hex2bcd(t->minute) | (((mode >> 1) & 0x01) << 7));         /* write minute */
    if (res != 0)                                                                                                                  /* check result */
    {
        handle->debug_print("ds3231: write alarm1 minute failed.\n");                                                              /* write alarm1 minute failed */
        
        return 1;                                                                                                                  /* return error */
    }
    if (t->format == DS3231_FORMAT_12H)                                                                                            /* if 12H */
    {
        reg = (uint8_t)((((mode >> 2) & 0x01) << 7) | (1 << 6) | (t->am_pm << 5) | a_ds3231_hex2bcd(t->hour));                     /* set hour in 12H */
    }
    else                                                                                                                           /* if 24H */
    {
        reg = (((mode >> 2) & 0x01) << 7) | a_ds3231_hex2bcd(t->hour);                                                             /* set hour in 24H */
    }
    res = a_ds3231_iic_write(handle, DS3231_REG_ALARM1_HOUR, reg);                                                                 /* write hour */
    if (res != 0)                                                                                                                  /* check result */
    {
        handle->debug_print("ds3231: write alarm1 hour failed.\n");                                                                /* write alarm1 hour failed */
        
        return 1;                                                                                                                  /* return error */
    }
    if (mode >= DS3231_ALARM1_MODE_WEEK_HOUR_MINUTE_SECOND_MATCH)                                                                  /* if week */
    {
        reg = (((mode >> 3) & 0x01) << 7) | (1 << 6) | a_ds3231_hex2bcd(t->week);                                                  /* set data in week */
    }
    else                                                                                                                           /* if day */
    {
        reg = (((mode >> 3) & 0x01) << 7) | a_ds3231_hex2bcd(t->date);                                                             /* set data in date */
    }
    res = a_ds3231_iic_write(handle, DS3231_REG_ALARM1_WEEK, reg);                                                                 /* write week */
    if (res != 0)                                                                                                                  /* check result */
    {
        handle->debug_print("ds3231: write alarm1 week failed.\n");                                                                /* write alarm1 week failed */
        
        return 1;                                                                                                                  /* return error */
    }
    
    return 0;                                                                                                                      /* success return 0 */
}

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
uint8_t ds3231_get_alarm1(ds3231_handle_t *handle, ds3231_time_t *t, ds3231_alarm1_mode_t *mode)
{
    uint8_t res;
    uint8_t buf[4];
    
    if (handle == NULL)                                                                                                                   /* check handle */
    {
        return 2;                                                                                                                         /* return error */
    }
    if (handle->inited != 1)                                                                                                              /* check handle initialization */
    {
        return 3;                                                                                                                         /* return error */
    }
    if (t == NULL)                                                                                                                        /* check time */
    {
        handle->debug_print("ds3231: time is null.\n");                                                                                   /* time is null */
        
        return 2;                                                                                                                         /* return error */
    }
    
    memset(buf, 0, sizeof(uint8_t) * 4);                                                                                                  /* clear the buffer */
    res = a_ds3231_iic_multiple_read(handle, DS3231_REG_ALARM1_SECOND, (uint8_t *)buf, 4);                                                /* multiple_read */
    if (res != 0)                                                                                                                         /* check result */
    {
        handle->debug_print("ds3231: multiple read failed.\n");                                                                           /* multiple read failed */
        
        return 1;                                                                                                                         /* return error */
    }
    t->year = 0;                                                                                                                          /* get year */
    t->month = 0;                                                                                                                         /* get month */
    if (((buf[3] >> 6) & 0x01) != 0)                                                                                                      /* if week */
    {
        t->week = a_ds3231_bcd2hex(buf[3] & 0x0F);                                                                                        /* get week */
        t->date = 0;                                                                                                                      /* get data */
    }
    else                                                                                                                                  /* if data */
    {
        t->week = 0;                                                                                                                      /* get week */
        t->date = a_ds3231_bcd2hex(buf[3] & 0x3F);                                                                                        /* get data */
    }
    t->am_pm = (ds3231_am_pm_t)((buf[2] >> 5) & 0x01);                                                                                    /* get am pm */
    t->format = (ds3231_format_t)((buf[2] >> 6) & 0x01);                                                                                  /* get format */
    if (t->format == DS3231_FORMAT_12H)                                                                                                   /* if 12H */
    {
        t->hour = a_ds3231_bcd2hex(buf[2]&0x1F);                                                                                          /* get hour */
    }
    else                                                                                                                                  /* if 24H */
    {
        t->hour = a_ds3231_bcd2hex(buf[2]&0x3F);                                                                                          /* get hour */
    }
    t->minute = a_ds3231_bcd2hex(buf[1] & 0x7F);                                                                                          /* get minute */
    t->second = a_ds3231_bcd2hex(buf[0] & 0x7F);                                                                                          /* get second */
    *mode = (ds3231_alarm1_mode_t)(((buf[0]>>7)&0x01)<<0 | ((buf[1]>>7)&0x01)<<1 | ((buf[2]>>7)&0x01)<<2 | ((buf[3]>>7)&0x01)<<3 |
                                   ((buf[3] >> 6)&0x01)<<4
                                  );                                                                                                      /* get mode */
    
    return 0;                                                                                                                             /* success return 0 */
}

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
uint8_t ds3231_set_alarm2(ds3231_handle_t *handle, ds3231_time_t *t, ds3231_alarm2_mode_t mode)
{
    uint8_t res;
    uint8_t reg;
    
    if (handle == NULL)                                                                                                            /* check handle */
    {
        return 2;                                                                                                                  /* return error */
    }
    if (handle->inited != 1)                                                                                                       /* check handle initialization */
    {
        return 3;                                                                                                                  /* return error */
    }
    if (t == NULL)                                                                                                                 /* check time */
    {
        handle->debug_print("ds3231: time is null.\n");                                                                            /* time is null */
        
        return 2;                                                                                                                  /* return error */
    }
    if (t->format == DS3231_FORMAT_12H)                                                                                            /* if 12H */
    {
        if ((t->week == 0) || (t->week > 7))                                                                                       /* check week */
        {
            handle->debug_print("ds3231: week can't be zero or over than 7.\n");                                                   /* week can't be zero or over than 7 */
            
            return 1;                                                                                                              /* return error */
        }
        if ((t->date == 0) || (t->date > 31))                                                                                      /* check data */
        {
            handle->debug_print("ds3231: date can't be zero or over than 31.\n");                                                  /* date can't be zero or over than 31 */
            
            return 1;                                                                                                              /* return error */
        }
        if ((t->hour < 1) || (t->hour > 12))                                                                                       /* check hour */
        {
            handle->debug_print("ds3231: hour can't be over than 12 or less 1.\n");                                                /* hour can't be over than 12 or less 1 */
            
            return 1;                                                                                                              /* return error */
        }
        if (t->minute > 59)                                                                                                        /* check minute */
        {
            handle->debug_print("ds3231: minute can't be over than 59.\n");                                                        /* minute can't be over than 59 */
            
            return 1;                                                                                                              /* return error */
        }
    }
    else if (t->format == DS3231_FORMAT_24H)                                                                                       /* if 24H */
    {
        if ((t->week == 0) || (t->week > 7))                                                                                       /* check week */
        {
            handle->debug_print("ds3231: week can't be zero or over than 7.\n");                                                   /* week can't be zero or over than 7 */
            
            return 1;                                                                                                              /* return error */
        }
        if ((t->date == 0) || (t->date > 31))                                                                                      /* check data */
        {
            handle->debug_print("ds3231: date can't be zero or over than 31.\n");                                                  /* date can't be zero or over than 31 */
            
            return 1;                                                                                                              /* return error */
        }
        if (t->hour > 23)                                                                                                          /* check hour */
        {
            handle->debug_print("ds3231: hour can't be over than 23.\n");                                                          /* hour can't be over than 23 */
            
            return 1;                                                                                                              /* return error */
        }
        if (t->minute > 59)                                                                                                        /* check minute */
        {
            handle->debug_print("ds3231: minute can't be over than 59.\n");                                                        /* minute can't be over than 59 */
            
            return 1;                                                                                                              /* return error */
        }
    }
    else
    {
        handle->debug_print("ds3231: format is invalid.\n");                                                                       /* format is invalid */
        
        return 1;                                                                                                                  /* return error */
    }
    
    res = a_ds3231_iic_write(handle, DS3231_REG_ALARM2_MINUTE, a_ds3231_hex2bcd(t->minute) | (((mode >> 0) & 0x01) << 7));         /* write minute */
    if (res != 0)                                                                                                                  /* check result */
    {
        handle->debug_print("ds3231: write alarm2 minute failed.\n");                                                              /* write alarm2 minute failed */
        
        return 1;                                                                                                                  /* return error */
    }
    if (t->format == DS3231_FORMAT_12H)                                                                                            /* if 12H */
    {
        reg = (uint8_t)((((mode >> 1) & 0x01) << 7) | (1 << 6) | (t->am_pm << 5) | a_ds3231_hex2bcd(t->hour));                     /* set hour in 12H */
    }
    else                                                                                                                           /* if 24H */
    {
        reg = (((mode >> 1) & 0x01) << 7) | a_ds3231_hex2bcd(t->hour);                                                             /* set hour in 24H */
    }
    res = a_ds3231_iic_write(handle, DS3231_REG_ALARM2_HOUR, reg);                                                                 /* write hour */
    if (res != 0)                                                                                                                  /* check result */
    {
        handle->debug_print("ds3231: write alarm2 hour failed.\n");                                                                /* write alarm2 hour failed */
        
        return 1;                                                                                                                  /* return error */
    }
    if (mode >= (uint8_t)DS3231_ALARM1_MODE_WEEK_HOUR_MINUTE_SECOND_MATCH)                                                         /* if week */
    {
        reg = (((mode >> 2) & 0x01) << 7) | (1 << 6) | a_ds3231_hex2bcd(t->week);                                                  /* set data in week */
    }
    else                                                                                                                           /* if day */
    {
        reg = (((mode >> 2) & 0x01) << 7) | a_ds3231_hex2bcd(t->date);                                                             /* set data in date */
    }
    res = a_ds3231_iic_write(handle, DS3231_REG_ALARM2_WEEK, reg);                                                                 /* write week */
    if (res != 0)                                                                                                                  /* check result */
    {
        handle->debug_print("ds3231: write alarm2 week failed.\n");                                                                /* write alarm2 week failed */
        
        return 1;                                                                                                                  /* return error */
    }
    
    return 0;                                                                                                                      /* success return 0 */
}

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
uint8_t ds3231_get_alarm2(ds3231_handle_t *handle, ds3231_time_t *t, ds3231_alarm2_mode_t *mode)
{
    uint8_t res;
    uint8_t buf[3];
    
    if (handle == NULL)                                                                                                                   /* check handle */
    {
        return 2;                                                                                                                         /* return error */
    }
    if (handle->inited != 1)                                                                                                              /* check handle initialization */
    {
        return 3;                                                                                                                         /* return error */
    }
    if (t == NULL)                                                                                                                        /* check time */
    {
        handle->debug_print("ds3231: time is null.\n");                                                                                   /* time is null */
        
        return 2;                                                                                                                         /* return error */
    }
    
    memset(buf, 0, sizeof(uint8_t) * 3);                                                                                                  /* clear the buffer */
    res = a_ds3231_iic_multiple_read(handle, DS3231_REG_ALARM2_MINUTE, (uint8_t *)buf, 3);                                                /* multiple read */
    if (res != 0)                                                                                                                         /* check result */
    {
        handle->debug_print("ds3231: multiple read failed.\n");                                                                           /* multiple read failed */
        
        return 1;                                                                                                                         /* return error */
    }
    t->year = 0;                                                                                                                          /* get year */
    t->month = 0;                                                                                                                         /* get month */
    if (((buf[2] >> 6) & 0x01) != 0)                                                                                                      /* if week */
    {
        t->week = a_ds3231_bcd2hex(buf[2] & 0x0F);                                                                                        /* get week */
        t->date = 0;                                                                                                                      /* get data */
    }
    else                                                                                                                                  /* if data */
    {
        t->week = 0;                                                                                                                      /* get week */
        t->date = a_ds3231_bcd2hex(buf[2] & 0x3F);                                                                                        /* get data */
    }
    t->am_pm = (ds3231_am_pm_t)((buf[1] >> 5) & 0x01);                                                                                    /* get am pm */
    t->format = (ds3231_format_t)((buf[1] >> 6) & 0x01);                                                                                  /* get format */
    if (t->format == DS3231_FORMAT_12H)                                                                                                   /* if 12H */
    {
        t->hour = a_ds3231_bcd2hex(buf[1]&0x1F);                                                                                          /* get hour */
    }
    else                                                                                                                                  /* if 24H */
    {
        t->hour = a_ds3231_bcd2hex(buf[1]&0x3F);                                                                                          /* get hour */
    }
    t->minute = a_ds3231_bcd2hex(buf[0] & 0x7F);                                                                                          /* get minute */
    t->second = 0;                                                                                                                        /* get second */
    *mode = (ds3231_alarm2_mode_t)(((buf[0]>>7)&0x01)<<0 | ((buf[1]>>7)&0x01)<<1 | ((buf[2]>>7)&0x01)<<2 | ((buf[2]>>6)&0x01)<<4);        /* get mode */
    
    return 0;                                                                                                                             /* success return 0 */
}

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
uint8_t ds3231_set_oscillator(ds3231_handle_t *handle, ds3231_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                      /* check handle */
    {
        return 2;                                                                            /* return error */
    }
    if (handle->inited != 1)                                                                 /* check handle initialization */
    {
        return 3;                                                                            /* return error */
    }
    
    res = a_ds3231_iic_multiple_read(handle, DS3231_REG_CONTROL, (uint8_t *)&prev, 1);       /* multiple read */
    if (res != 0)                                                                            /* check result */
    {
        handle->debug_print("ds3231: read control failed.\n");                               /* read control failed */
        
        return 1;                                                                            /* return error */
    }
    prev &= ~ (1 << 7);                                                                      /* clear config */
    prev |= (!enable) << 7;                                                                  /* set enable */
    res = a_ds3231_iic_write(handle, DS3231_REG_CONTROL, prev);                              /* write control */
    if (res != 0)                                                                            /* check result */
    {
        handle->debug_print("ds3231: write control failed.\n");                              /* write control failed */
        
        return 1;                                                                            /* return error */
    }
    
    return 0;                                                                                /* success return 0 */
}

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
uint8_t ds3231_get_oscillator(ds3231_handle_t *handle, ds3231_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                      /* check handle */
    {
        return 2;                                                                            /* return error */
    }
    if (handle->inited != 1)                                                                 /* check handle initialization */
    {
        return 3;                                                                            /* return error */
    }
    
    res = a_ds3231_iic_multiple_read(handle, DS3231_REG_CONTROL, (uint8_t *)&prev, 1);       /* multiple read */
    if (res != 0)                                                                            /* check result */
    {
        handle->debug_print("ds3231: read control failed.\n");                               /* read control failed */
        
        return 1;                                                                            /* return error */
    }
    *enable = (ds3231_bool_t)(!((prev >> 7) & 0x01));                                        /* get enable */
    
    return 0;                                                                                /* success return 0 */
}

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
uint8_t ds3231_set_alarm_interrupt(ds3231_handle_t *handle, ds3231_alarm_t alarm, ds3231_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                      /* check handle */
    {
        return 2;                                                                            /* return error */
    }
    if (handle->inited != 1)                                                                 /* check handle initialization */
    {
        return 3;                                                                            /* return error */
    }
    
    res = a_ds3231_iic_multiple_read(handle, DS3231_REG_CONTROL, (uint8_t *)&prev, 1);       /* multiple_read */
    if (res != 0)                                                                            /* check result */
    {
        handle->debug_print("ds3231: read control failed.\n");                               /* read control failed */
        
        return 1;                                                                            /* return error */
    }
    prev &= ~(1 << alarm);                                                                   /* clear config */
    prev |= enable << alarm;                                                                 /* set enable */
    res = a_ds3231_iic_write(handle, DS3231_REG_CONTROL, prev);                              /* write control */
    if (res != 0)                                                                            /* check result */
    {
        handle->debug_print("ds3231: write control failed.\n");                              /* write control failed */
        
        return 1;                                                                            /* return error */
    }
    
    return 0;                                                                                /* success return 0 */
}

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
uint8_t ds3231_get_alarm_interrupt(ds3231_handle_t *handle, ds3231_alarm_t alarm, ds3231_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                      /* check handle */
    {
        return 2;                                                                            /* return error */
    }
    if (handle->inited != 1)                                                                 /* check handle initialization */
    {
        return 3;                                                                            /* return error */
    }
    
    res = a_ds3231_iic_multiple_read(handle, DS3231_REG_CONTROL, (uint8_t *)&prev, 1);       /* multiple_read */
    if (res != 0)                                                                            /* check result */
    {
        handle->debug_print("ds3231: read control failed.\n");                               /* read control failed */
        
        return 1;                                                                            /* return error */
    }
    *enable = (ds3231_bool_t)((prev >> alarm) & 0x01);                                       /* get enable */
    
    return 0;                                                                                /* success return 0 */
}

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
uint8_t ds3231_set_pin(ds3231_handle_t *handle, ds3231_pin_t pin)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                      /* check handle */
    {
        return 2;                                                                            /* return error */
    }
    if (handle->inited != 1)                                                                 /* check handle initialization */
    {
        return 3;                                                                            /* return error */
    }
    
    res = a_ds3231_iic_multiple_read(handle, DS3231_REG_CONTROL, (uint8_t *)&prev, 1);       /* multiple_read */
    if (res != 0)                                                                            /* check result */
    {
        handle->debug_print("ds3231: read control failed.\n");                               /* read control failed */
        
        return 1;                                                                            /* return error */
    }
    prev &= ~(1 << 2);                                                                       /* clear config */
    prev |= pin << 2;                                                                        /* set pin */
    res = a_ds3231_iic_write(handle, DS3231_REG_CONTROL, prev);                              /* write control */
    if (res != 0)                                                                            /* check result */
    {
        handle->debug_print("ds3231: write control failed.\n");                              /* write control failed */
        
        return 1;                                                                            /* return error */
    }
    
    return 0;                                                                                /* success return 0 */
}

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
uint8_t ds3231_get_pin(ds3231_handle_t *handle, ds3231_pin_t *pin)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                      /* check handle */
    {
        return 2;                                                                            /* return error */
    }
    if (handle->inited != 1)                                                                 /* check handle initialization */
    {
        return 3;                                                                            /* return error */
    }
    
    res = a_ds3231_iic_multiple_read(handle, DS3231_REG_CONTROL, (uint8_t *)&prev, 1);       /* multiple_read */
    if (res != 0)                                                                            /* check result */
    {
        handle->debug_print("ds3231: read control failed.\n");                               /* read control failed */
        
        return 1;                                                                            /* return error */
    }
    *pin = (ds3231_pin_t)((prev >> 2) & 0x01);                                               /* get pin */
    
    return 0;                                                                                /* success return 0 */
}

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
uint8_t ds3231_set_square_wave(ds3231_handle_t *handle, ds3231_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                      /* check handle */
    {
        return 2;                                                                            /* return error */
    }
    if (handle->inited != 1)                                                                 /* check handle initialization */
    {
        return 3;                                                                            /* return error */
    }
    
    res = a_ds3231_iic_multiple_read(handle, DS3231_REG_CONTROL, (uint8_t *)&prev, 1);       /* multiple_read */
    if (res != 0)                                                                            /* check result */
    {
        handle->debug_print("ds3231: read control failed.\n");                               /* read control failed */
        
        return 1;                                                                            /* return error */
    }
    prev &= ~(1 << 6);                                                                       /* clear config */
    prev |= enable << 6;                                                                     /* set enable */
    res = a_ds3231_iic_write(handle, DS3231_REG_CONTROL, prev);                              /* write control */
    if (res != 0)                                                                            /* check result */
    {
        handle->debug_print("ds3231: write control failed.\n");                              /* write control failed */
        
        return 1;                                                                            /* return error */
    }
    
    return 0;                                                                                /* success return 0 */
}

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
uint8_t ds3231_get_square_wave(ds3231_handle_t *handle, ds3231_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                      /* check handle */
    {
        return 2;                                                                            /* return error */
    }
    if (handle->inited != 1)                                                                 /* check handle initialization */
    {
        return 3;                                                                            /* return error */
    }
    
    res = a_ds3231_iic_multiple_read(handle, DS3231_REG_CONTROL, (uint8_t *)&prev, 1);       /* multiple_read */
    if (res != 0)                                                                            /* check result */
    {
        handle->debug_print("ds3231: read control failed.\n");                               /* read control failed */
        
        return 1;                                                                            /* return error */
    }
    *enable = (ds3231_bool_t)((prev >> 6) & 0x01);                                           /* get enable */
    
    return 0;                                                                                /* success return 0 */
}

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
uint8_t ds3231_get_temperature(ds3231_handle_t *handle, int16_t *raw, float *s)
{
    uint8_t res;
    uint8_t prev;
    uint32_t times;
    uint8_t buf[2];
    
    if (handle == NULL)                                                                         /* check handle */
    {
        return 2;                                                                               /* return error */
    }
    if (handle->inited != 1)                                                                    /* check handle initialization */
    {
        return 3;                                                                               /* return error */
    }
    
    memset(buf, 0, sizeof(uint8_t) * 2);                                                        /* clear the buffer */
    res = a_ds3231_iic_multiple_read(handle, DS3231_REG_CONTROL, (uint8_t *)&prev, 1);          /* multiple read */
    if (res != 0)                                                                               /* check result */
    {
        handle->debug_print("ds3231: read control failed.\n");                                  /* read control failed */
        
        return 1;                                                                               /* return error */
    }
    prev &= ~(1 << 5);                                                                          /* clear config */
    prev |= 1 << 5;                                                                             /* set enable */
    res = a_ds3231_iic_write(handle, DS3231_REG_CONTROL, prev);                                 /* write control */
    if (res != 0)                                                                               /* check result */
    {
        handle->debug_print("ds3231: write control failed.\n");                                 /* write control failed */
        
        return 1;                                                                               /* return error */
    }
    times = 500;                                                                                /* set 5s */
    while (times != 0)                                                                          /* check times */
    {
        handle->delay_ms(10);                                                                   /* delay 10 ms */
        res = a_ds3231_iic_multiple_read(handle, DS3231_REG_STATUS, (uint8_t *)&prev, 1);       /* read status */
        if (res != 0)                                                                           /* check result */
        {
            handle->debug_print("ds3231: read status failed.\n");                               /* read status failed */
            
            return 1;                                                                           /* return error */
        }
        if (((prev >> 2) & 0x01) == 0)                                                          /* check result */
        {
            break;                                                                              /* break */
        }
        times--;                                                                                /* times-- */
    }
    if (times == 0)                                                                             /* if zero */
    {
        handle->debug_print("ds3231: read timeout.\n");                                         /* read timeout */
        
        return 1;                                                                               /* return error */
    }
    res = a_ds3231_iic_multiple_read(handle, DS3231_REG_TEMPERATUREH, (uint8_t *)buf, 2);       /* read temperature */
    if (res != 0)                                                                               /* check result */
    {
        handle->debug_print("ds3231: read temperature failed.\n");                              /* read temperature failed */
        
        return 1;                                                                               /* return error */
    }
    *raw = (int16_t)(((uint16_t)buf[0]) << 8) | buf[1];                                         /* set raw temperature */
    *raw = (*raw) >> 6;                                                                         /* right shift */
    if (((*raw) & 0x0200) != 0)                                                                 /* set negative value */
    {
        *raw = (*raw) | 0xFC00U;                                                                /* set negative part */
    }
    *s = (float)(*raw) * 0.25f;                                                                 /* set converted temperature */
    
    return 0;                                                                                   /* success return 0 */
}

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
uint8_t ds3231_get_status(ds3231_handle_t *handle, uint8_t *status)
{
    uint8_t res;
    
    if (handle == NULL)                                                                      /* check handle */
    {
        return 2;                                                                            /* return error */
    }
    if (handle->inited != 1)                                                                 /* check handle initialization */
    {
        return 3;                                                                            /* return error */
    }
    
    res = a_ds3231_iic_multiple_read(handle, DS3231_REG_STATUS, (uint8_t *)status, 1);       /* multiple read */
    if (res != 0)                                                                            /* check result */
    {
        handle->debug_print("ds3231: read status failed.\n");                                /* read status failed */
        
        return 1;                                                                            /* return error */
    }

    return 0;                                                                                /* success return 0 */
}

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
uint8_t ds3231_set_aging_offset(ds3231_handle_t *handle, int8_t offset)
{
    uint8_t res;
    
    if (handle == NULL)                                               /* check handle */
    {
        return 2;                                                     /* return error */
    }
    if (handle->inited != 1)                                          /* check handle initialization */
    {
        return 3;                                                     /* return error */
    }

    res = a_ds3231_iic_write(handle, DS3231_REG_XTAL, offset);        /* write offset */
    if (res != 0)                                                     /* check result */
    {
        handle->debug_print("ds3231: write offset failed.\n");        /* write offset failed */
        
        return 1;                                                     /* return error */
    }
    
    return 0;                                                         /* success return 0 */
}

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
uint8_t ds3231_get_aging_offset(ds3231_handle_t *handle, int8_t *offset)
{
    uint8_t res;
    
    if (handle == NULL)                                                                    /* check handle */
    {
        return 2;                                                                          /* return error */
    }
    if (handle->inited != 1)                                                               /* check handle initialization */
    {
        return 3;                                                                          /* return error */
    }

    res = a_ds3231_iic_multiple_read(handle, DS3231_REG_XTAL, (uint8_t *)offset, 1);       /* read offset */
    if (res != 0)                                                                          /* check result */
    {
        handle->debug_print("ds3231: read offset failed.\n");                              /* read offset failed */
        
        return 1;                                                                          /* return error */
    }
    
    return 0;                                                                              /* success return 0 */
}

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
uint8_t ds3231_aging_offset_convert_to_register(ds3231_handle_t *handle, float offset, int8_t *reg)
{
    if (handle == NULL)                     /* check handle */
    {
        return 2;                           /* return error */
    }
    if (handle->inited != 1)                /* check handle initialization */
    {
        return 3;                           /* return error */
    }
    
    *reg = (int8_t)(offset / 0.12f);        /* convert real data to register data */
    
    return 0;                               /* success return 0 */
}

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
uint8_t ds3231_aging_offset_convert_to_data(ds3231_handle_t *handle, int8_t reg, float *offset)
{
    if (handle == NULL)                    /* check handle */
    {
        return 2;                          /* return error */
    }
    if (handle->inited != 1)               /* check handle initialization */
    {
        return 3;                          /* return error */
    }
    
    *offset = (float)(reg) * 0.12f;        /* convert raw data to real data */
    
    return 0;                              /* success return 0 */
}

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
uint8_t ds3231_irq_handler(ds3231_handle_t *handle)
{
    uint8_t res, prev;
    
    if (handle == NULL)                                                                      /* check handle */
    {
        return 2;                                                                            /* return error */
    }
    if (handle->inited != 1)                                                                 /* check handle initialization */
    {
        return 3;                                                                            /* return error */
    }
    
    res = a_ds3231_iic_multiple_read(handle, DS3231_REG_STATUS, (uint8_t *)&prev, 1);        /* multiple read */
    if (res != 0)                                                                            /* check result */
    {
        handle->debug_print("ds3231: read status failed.\n");                                /* read status failed */
        
        return 1;                                                                            /* return error */
    }                                                                                        /* if oscillator stop */
    if ((prev & DS3231_STATUS_ALARM_2) != 0)                                                 /* if alarm 2 */
    {
        if (handle->receive_callback != NULL)                                                /* if receive callback */
        {
            handle->receive_callback(DS3231_STATUS_ALARM_2);                                 /* run callback */
        }
    }
    if ((prev & DS3231_STATUS_ALARM_1) != 0)                                                 /* if alarm 1 */
    {
        if (handle->receive_callback != NULL)                                                /* if receive callback */
        {
            handle->receive_callback(DS3231_STATUS_ALARM_1);                                 /* run callback */
        }
    }
    
    return 0;                                                                                /* success return 0 */
}

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
uint8_t ds3231_init(ds3231_handle_t *handle)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                      /* check handle */
    {
        return 2;                                                                            /* return error */
    }
    if (handle->debug_print == NULL)                                                         /* check debug_print */
    {
        return 3;                                                                            /* return error */
    }
    if (handle->iic_init == NULL)                                                            /* check iic_init */
    {
        handle->debug_print("ds3231: iic_init is null.\n");                                  /* iic_init is null */
       
        return 3;                                                                            /* return error */
    }
    if (handle->iic_deinit == NULL)                                                          /* check iic_deinit */
    {
        handle->debug_print("ds3231: iic_deinit is null.\n");                                /* iic_deinit is null */
       
        return 3;                                                                            /* return error */
    }
    if (handle->iic_write == NULL)                                                           /* check iic_write */
    {
        handle->debug_print("ds3231: iic_write is null.\n");                                 /* iic_write is null */
       
        return 3;                                                                            /* return error */
    }
    if (handle->iic_read == NULL)                                                            /* check iic_read */
    {
        handle->debug_print("ds3231: iic_read is null.\n");                                  /* iic_read is null */
       
        return 3;                                                                            /* return error */
    }
    if (handle->delay_ms == NULL)                                                            /* check delay_ms */
    {
        handle->debug_print("ds3231: delay_ms is null.\n");                                  /* delay_ms is null */
       
        return 3;                                                                            /* return error */
    }
    if (handle->receive_callback == NULL)                                                    /* check receive_callback */
    {
        handle->debug_print("ds3231: receive_callback is null.\n");                          /* receive_callback is null */
       
        return 3;                                                                            /* return error */
    }
    
    if (handle->iic_init() != 0)                                                             /* iic init */
    {
        handle->debug_print("ds3231: iic init failed.\n");                                   /* iic init failed */
       
        return 1;                                                                            /* return error */
    }
    res = a_ds3231_iic_multiple_read(handle, DS3231_REG_STATUS, (uint8_t *)&prev, 1);        /* multiple_read */
    if (res != 0)                                                                            /* check result */
    {
        handle->debug_print("ds3231: read status failed.\n");                                /* read status failed */
        (void)handle->iic_deinit();                                                          /* iic deinit */
        
        return 1;                                                                            /* return error */
    }
    prev &= ~(1 << 7);                                                                       /* clear config */
    res = a_ds3231_iic_write(handle, DS3231_REG_STATUS, prev);                               /* write status */
    if (res != 0)                                                                            /* check result */
    {
        handle->debug_print("ds3231: write status failed.\n");                               /* write status failed */
        (void)handle->iic_deinit();                                                          /* iic deinit */
        
        return 1;                                                                            /* return error */
    }
    handle->inited = 1;                                                                      /* flag finish initialization */
    
    return 0;                                                                                /* success return 0 */
}

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
uint8_t ds3231_deinit(ds3231_handle_t *handle)
{
    if (handle == NULL)                                             /* check handle */
    {
        return 2;                                                   /* return error */
    }
    if (handle->inited != 1)                                        /* check handle initialization */
    {
        return 3;                                                   /* return error */
    }
    
    if (handle->iic_deinit() != 0)                                  /* iic deinit */
    {
        handle->debug_print("ds3231: iic deinit failed.\n");        /* iic deinit failed */
       
        return 1;                                                   /* return error */
    }
    handle->inited = 0;                                             /* flag close */
    
    return 0;                                                       /* success return 0 */
}

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
uint8_t ds3231_alarm_clear(ds3231_handle_t *handle, ds3231_alarm_t alarm)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                      /* check handle */
    {
        return 2;                                                                            /* return error */
    }
    if (handle->inited != 1)                                                                 /* check handle initialization */
    {
        return 3;                                                                            /* return error */
    }
    
    res = a_ds3231_iic_multiple_read(handle, DS3231_REG_STATUS, (uint8_t *)&prev, 1);        /* multiple_read  */
    if (res != 0)                                                                            /* check result */
    {
        handle->debug_print("ds3231: read status failed.\n");                                /* read status failed */
        
        return 1;                                                                            /* return error */
    }
    prev &= ~(1 << alarm);                                                                   /* clear config */
    res = a_ds3231_iic_write(handle, DS3231_REG_STATUS, prev);                               /* write status */
    if (res != 0)                                                                            /* check result */
    {
        handle->debug_print("ds3231: write status failed.\n");                               /* write status failed */
        
        return 1;                                                                            /* return error */
    }
    
    return 0;                                                                                /* success return 0 */
}

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
uint8_t ds3231_set_32khz_output(ds3231_handle_t *handle, ds3231_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                      /* check handle */
    {
        return 2;                                                                            /* return error */
    }
    if (handle->inited != 1)                                                                 /* check handle initialization */
    {
        return 3;                                                                            /* return error */
    }
    
    res = a_ds3231_iic_multiple_read(handle, DS3231_REG_STATUS, (uint8_t *)&prev, 1);        /* multiple read */
    if (res != 0)                                                                            /* check result */
    {
        handle->debug_print("ds3231: read status failed.\n");                                /* read status failed */
        
        return 1;                                                                            /* return error */
    }
    prev &= ~(1 << 3);                                                                       /* clear config */
    prev |= enable << 3;                                                                     /* set enable */
    res = a_ds3231_iic_write(handle, DS3231_REG_STATUS, prev);                               /* write status */
    if (res != 0)                                                                            /* check result */
    {
        handle->debug_print("ds3231: write status failed.\n");                               /* write status failed */
        
        return 1;                                                                            /* return error */
    }
    
    return 0;                                                                                /* success return 0 */
}

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
uint8_t ds3231_get_32khz_output(ds3231_handle_t *handle, ds3231_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                      /* check handle */
    {
        return 2;                                                                            /* return error */
    }
    if (handle->inited != 1)                                                                 /* check handle initialization */
    {
        return 3;                                                                            /* return error */
    }
    
    res = a_ds3231_iic_multiple_read(handle, DS3231_REG_STATUS, (uint8_t *)&prev, 1);        /* multiple read */
    if (res != 0)                                                                            /* check result */
    {
        handle->debug_print("ds3231: read status failed.\n");                                /* read status failed */
        
        return 1;                                                                            /* return error */
    }
    *enable = (ds3231_bool_t)((prev >> 3) & 0x01);                                           /* get enable */
    
    return 0;                                                                                /* success return 0 */
}

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
uint8_t ds3231_set_reg(ds3231_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (handle == NULL)                                               /* check handle */
    {
        return 2;                                                     /* return error */
    }
    if (handle->inited != 1)                                          /* check handle initialization */
    {
        return 3;                                                     /* return error */
    }
    
    if (handle->iic_write(DS3231_ADDRESS, reg, buf, len) != 0)        /* write data */
    {
        return 1;                                                     /* return error */
    }
    else
    {
        return 0;                                                     /* success return 0 */
    }
}

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
uint8_t ds3231_get_reg(ds3231_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (handle == NULL)                                              /* check handle */
    {
        return 2;                                                    /* return error */
    }
    if (handle->inited != 1)                                         /* check handle initialization */
    {
        return 3;                                                    /* return error */
    }
    
    if (handle->iic_read(DS3231_ADDRESS, reg, buf, len) != 0)        /* read data */
    {
        return 1;                                                    /* return error */
    }
    else
    {
        return 0;                                                    /* success return 0 */
    }
}

/**
 * @brief      get chip's information
 * @param[out] *info pointer to a ds3231 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t ds3231_info(ds3231_info_t *info)
{
    if (info == NULL)                                               /* check handle */
    {
        return 2;                                                   /* return error */
    }
    
    memset(info, 0, sizeof(ds3231_info_t));                         /* initialize ds3231 info structure */
    strncpy(info->chip_name, CHIP_NAME, 32);                        /* copy chip name */
    strncpy(info->manufacturer_name, MANUFACTURER_NAME, 32);        /* copy manufacturer name */
    strncpy(info->interface, "IIC", 8);                             /* copy interface name */
    info->supply_voltage_min_v = SUPPLY_VOLTAGE_MIN;                /* set minimal supply voltage */
    info->supply_voltage_max_v = SUPPLY_VOLTAGE_MAX;                /* set maximum supply voltage */
    info->max_current_ma = MAX_CURRENT;                             /* set maximum current */
    info->temperature_max = TEMPERATURE_MAX;                        /* set minimal temperature */
    info->temperature_min = TEMPERATURE_MIN;                        /* set maximum temperature */
    info->driver_version = DRIVER_VERSION;                          /* set driver version */
    
    return 0;                                                       /* success return 0 */
}
