[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver DS3231

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/ds3231/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

The DS3231 is a low-cost, extremely accurate I2C real-time clock (RTC) with an integrated temperature compensated crystal oscillator (TCXO) and crystal. The device incorporates a battery input, and maintains accurate timekeeping when main power to the device is interrupted. The integration of the crystal resonator enhances the long-term accuracy of the device as well as reduces the piece-part count in a manufacturing line. The DS3231 is available in commercial and industrial temperature ranges, and is offered in a 16-pin, 300-mil SO package.

LibDriver DS3231 is a full function driver of ds3231 launched by LibDriver.It provides time reading, alarm clock triggering, temperature reading, square wave output and other functions. LibDriver is MISRA compliant.

### Table of Contents

  - [Instruction](#Instruction)
  - [Install](#Install)
  - [Usage](#Usage)
    - [example basic](#example-basic)
    - [example alarm](#example-alarm)
    - [example output](#example-output)
  - [Document](#Document)
  - [Contributing](#Contributing)
  - [License](#License)
  - [Contact Us](#Contact-Us)

### Instruction

/src includes LibDriver DS3231 source files.

/interface includes LibDriver DS3231 IIC platform independent template.

/test includes LibDriver DS3231 driver test code and this code can test the chip necessary function simply.

/example includes LibDriver DS3231 sample code.

/doc includes LibDriver DS3231 offline document.

/datasheet includes DS3231 datasheet.

/project includes the common Linux and MCU development board sample code. All projects use the shell script to debug the driver and the detail instruction can be found in each project's README.md.

/misra includes the LibDriver MISRA code scanning results.

### Install

Reference /interface IIC platform independent template and finish your platform IIC driver.

Add the /src directory, the interface driver for your platform, and your own drivers to your project, if you want to use the default example drivers, add the /example directory to your project.

### Usage

You can refer to the examples in the /example directory to complete your own driver. If you want to use the default programming examples, here's how to use them.

#### example basic

```C
#include "driver_ds3231_basic.h"

uint8_t res;
ds3231_time_t time;
int16_t raw;
float s;

res = ds3231_basic_init();
if (res != 0)
{
    return 1;
}

...

res = ds3231_basic_get_temperature((int16_t *)&raw, (float *)&s);
if (res != 0)
{
    (void)ds3231_basic_deinit();

    return 1;
}
ds3231_interface_debug_print("ds3231: temperature is %0.2fC.\n", s);    

...
mode = DS3231_ALARM1_MODE_ONCE_A_SECOND;
time.am_pm = DS3231_AM;
time.date = data;
time.format = DS3231_FORMAT_24H;
time.hour = hour;
time.minute = minute;
time.month = month;
time.second = second;
time.week  = week;
time.year = year;
res = ds3231_basic_set_time(&time);
if (res != 0)
{
    (void)ds3231_basic_deinit();

    return 1;
}

...
    
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

...

(void)ds3231_basic_deinit();

return 0;
```

#### example alarm

```C
#include "driver_ds3231_alarm.h"

uint8_t res;
ds3231_time_t time;
ds3231_alarm1_mode_t mode;

void alarm_receive_callback(uint8_t type)
{
    switch (type)
    {
        case DS3231_STATUS_ALARM_2 :
        {
            ds3231_alarm_clear_flag(DS3231_ALARM_2);
            ds3231_interface_debug_print("ds3231: irq alarm2.\n");
            
            ...
            
            break;
        }
        case DS3231_STATUS_ALARM_1 :
        {
            ds3231_alarm_clear_flag(DS3231_ALARM_1);
            ds3231_interface_debug_print("ds3231: irq alarm1.\n");
            
            ...
            
            break;
        }
        default :
        {
            ...
            
            break;
        }
    }
}


res = ds3231_alarm_init(alarm_receive_callback);
if (res != 0)
{
    return 1;
}

...

res = gpio_interrupt_init();
if (res != 0)
{
    (void)ds3231_alarm_deinit();

    return 1;
}

...

mode = DS3231_ALARM1_MODE_ONCE_A_SECOND;
time.am_pm = DS3231_AM;
time.date = data;
time.format = DS3231_FORMAT_24H;
time.hour = hour;
time.minute = minute;
time.month = month;
time.second = second;
time.week  = week;
time.year = year;
res = ds3231_alarm_set_alarm1(&time, mode);
if (res != 0)
{
    (void)ds3231_alarm_deinit();

    return 1;
}

...

res = ds3231_alarm_enable(DS3231_ALARM_1);
if (res != 0)
{
    (void)ds3231_alarm_deinit();

    return 1;
}

...

(void)ds3231_alarm_deinit();

return 0;
```

#### example output

```C
#include "driver_ds3231_output.h"

uint8_t res;

res = ds3231_output_init();
if (res != 0)
{
    return 1;
}

...

res = ds3231_output_set_square_wave(enable);
if (res != 0)
{
    (void)ds3231_output_deinit();

    return 1;
}

...

res = ds3231_output_set_32khz_output(enable);
if (res != 0)
{
    (void)ds3231_output_deinit();

    return 1;
}

...

(void)ds3231_output_deinit();

return 0;
```

### Document

Online documents: [https://www.libdriver.com/docs/ds3231/index.html](https://www.libdriver.com/docs/ds3231/index.html).

Offline documents: /doc/html/index.html.

### Contributing

Please refer to CONTRIBUTING.md.

### License

Copyright (c) 2015 - present LibDriver All rights reserved



The MIT License (MIT) 



Permission is hereby granted, free of charge, to any person obtaining a copy

of this software and associated documentation files (the "Software"), to deal

in the Software without restriction, including without limitation the rights

to use, copy, modify, merge, publish, distribute, sublicense, and/or sell

copies of the Software, and to permit persons to whom the Software is

furnished to do so, subject to the following conditions: 



The above copyright notice and this permission notice shall be included in all

copies or substantial portions of the Software. 



THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR

IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,

FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE

AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER

LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,

OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE

SOFTWARE. 

### Contact Us

Please send an e-mail to lishifenging@outlook.com.