<div align=center>
<img src="/doc/image/logo.png"/>
</div>

## LibDriver DS3231

[English](/README.md) | [ 简体中文](/README_CH.md)

DS3231是低成本、高精度IIC实时时钟，具有集成的温补晶体振荡器和晶体。该器件包含电池输入端，断开主电源时仍可保持精确的及时。集成晶体振荡器提高了器件的长期精确度，并减少了生产线的元件数量。DS3231提供商用级和工业级温度范围，采用16引脚、300mil的SO封装。

LibDriver DS3231是LibDriver推出的DS3231全功能驱动，该驱动提供时间读取、闹钟触发、温度读取和方波输出等功能。

### 目录

  - [说明](#说明)
  - [安装](#安装)
  - [使用](#使用)
    - [example basic](#example-basic)
    - [example alarm](#example-alarm)
    - [example output](#example-output)
  - [文档](#文档)
  - [贡献](#贡献)
  - [版权](#版权)
  - [联系我们](#联系我们)

### 说明

/src目录包含了LibDriver DS3231的源文件。

/interface目录包含了LibDriver DS3231与平台无关的IIC总线模板。

/test目录包含了LibDriver DS3231驱动测试程序，该程序可以简单的测试芯片必要功能。

/example目录包含了LibDriver DS3231编程范例。

/doc目录包含了LibDriver DS3231离线文档。

/datasheet目录包含了DS3231数据手册。

/project目录包含了常用Linux与单片机开发板的工程样例。所有工程均采用shell脚本作为调试方法，详细内容可参考每个工程里面的README.md。

### 安装

参考/interface目录下与平台无关的IIC总线模板，完成指定平台的IIC总线驱动。

将/src目录，/interface目录和/example目录加入工程。

### 使用

#### example basic

```C
uint8_t res;
ds3231_time_t time;
int16_t raw;
float s;

res = ds3231_basic_init();
if (res)
{
    return 1;
}

...

res = ds3231_basic_get_temperature((int16_t *)&raw, (float *)&s);
if (res)
{
    ds3231_basic_deinit();

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
if (res)
{
    ds3231_basic_deinit();

    return 1;
}

...
    
res = ds3231_basic_get_time(&time);
if (res)
{
    ds3231_basic_deinit();

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

ds3231_basic_deinit();

return 0;
```

#### example alarm

```C
uint8_t res;
ds3231_time_t time;
ds3231_alarm1_mode_t mode;

uint8_t alarm_receive_callback(uint8_t type)
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
        
    return 0;
}


res = ds3231_alarm_init(alarm_receive_callback);
if (res)
{
    return 1;
}

...

res = gpio_interrupt_init();
if (res)
{
    ds3231_alarm_deinit();

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
if (res)
{
    ds3231_alarm_deinit();

    return 1;
}

...

res = ds3231_alarm_enable(DS3231_ALARM_1);
if (res)
{
    ds3231_alarm_deinit();

    return 1;
}

...

ds3231_alarm_deinit();

return 0;
```

#### example output

```C
uint8_t res;

res = ds3231_output_init();
if (res)
{
    return 1;
}

...

res = ds3231_output_set_square_wave(enable);
if (res)
{
    ds3231_output_deinit();

    return 1;
}

...

res = ds3231_output_set_32khz_output(enable);
if (res)
{
    ds3231_output_deinit();

    return 1;
}

...

ds3231_output_deinit();

return 0;
```

### 文档

在线文档: https://www.libdriver.com/docs/ds3231/index.html

离线文档: /doc/html/index.html

### 贡献

请联系lishifenging@outlook.com

### 版权

版权(C) LibDriver 2015-2021 版权所有

MIT 许可证（MIT）

特此免费授予任何获得本软件副本和相关文档文件（下称“软件”）的人不受限制地处置该软件的权利，包括不受限制地使用、复制、修改、合并、发布、分发、转授许可和/或出售该软件副本，以及再授权被配发了本软件的人如上的权利，须在下列条件下：

上述版权声明和本许可声明应包含在该软件的所有副本或实质成分中。

本软件是“如此”提供的，没有任何形式的明示或暗示的保证，包括但不限于对适销性、特定用途的适用性和不侵权的保证。在任何情况下，作者或版权持有人都不对任何索赔、损害或其他责任负责，无论这些追责来自合同、侵权或其它行为中，还是产生于、源于或有关于本软件以及本软件的使用或其它处置。

### 联系我们

请联系lishifenging@outlook.com