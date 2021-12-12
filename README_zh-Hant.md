<div align=center>
<img src="/doc/image/logo.png"/>
</div>

## LibDriver DS3231

[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md)

DS3231是低成本、高精度IIC實時時鐘，具有集成的溫補晶體振盪器和晶體。該器件包含電池輸入端，斷開主電源時仍可保持精確的及時。集成晶體振盪器提高了器件的長期精確度，並減少了生產線的元件數量。 DS3231提供商用級和工業級溫度範圍，採用16引腳、300mil的SO封裝。

LibDriver DS3231是LibDriver推出的DS3231全功能驅動，該驅動提供時間讀取、鬧鐘觸發、溫度讀取和方波輸出等功能。

### 目錄

  - [說明](#說明)
  - [安裝](#安裝)
  - [使用](#使用)
    - [example basic](#example-basic)
    - [example alarm](#example-alarm)
    - [example output](#example-output)
  - [文檔](#文檔)
  - [貢獻](#貢獻)
  - [版權](#版權)
  - [聯繫我們](#聯繫我們)

### 說明

/src目錄包含了LibDriver DS3231的源文件。

/interface目錄包含了LibDriver DS3231與平台無關的IIC總線模板。

/test目錄包含了LibDriver DS3231驅動測試程序，該程序可以簡單的測試芯片必要功能。

/example目錄包含了LibDriver DS3231編程範例。

/doc目錄包含了LibDriver DS3231離線文檔。

/datasheet目錄包含了DS3231數據手冊。

/project目錄包含了常用Linux與單片機開發板的工程樣例。所有工程均採用shell腳本作為調試方法，詳細內容可參考每個工程裡面的README.md。

### 安裝

參考/interface目錄下與平台無關的IIC總線模板，完成指定平台的IIC總線驅動。

將/src目錄，/interface目錄和/example目錄加入工程。

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

### 文檔

在線文檔: https://www.libdriver.com/docs/ds3231/index.html

離線文檔: /doc/html/index.html

### 貢獻

請聯繫lishifenging@outlook.com

### 版權

版權 (c) 2015 - 現在 LibDriver 版權所有

MIT 許可證（MIT）

特此免費授予任何獲得本軟件副本和相關文檔文件（下稱“軟件”）的人不受限制地處置該軟件的權利，包括不受限制地使用、複製、修改、合併、發布、分發、轉授許可和/或出售該軟件副本，以及再授權被配發了本軟件的人如上的權利，須在下列條件下：

上述版權聲明和本許可聲明應包含在該軟件的所有副本或實質成分中。

本軟件是“如此”提供的，沒有任何形式的明示或暗示的保證，包括但不限於對適銷性、特定用途的適用性和不侵權的保證。在任何情況下，作者或版權持有人都不對任何索賠、損害或其他責任負責，無論這些追責來自合同、侵權或其它行為中，還是產生於、源於或有關於本軟件以及本軟件的使用或其它處置。

### 聯繫我們

請聯繫lishifenging@outlook.com
