[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver DS3231

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/ds3231/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

DS3231是低成本、高精度IIC即時時鐘，具有集成的溫補晶體振盪器。 該器件包含電池輸入端，斷開主電源時仍可保持精確的及時。 集成晶體振盪器提高了器件的長期精確度，並减少了生產線的元件數量。 DS3231提供商用級和工業級溫度範圍，採用16引脚、300mil的SO封裝。

LibDriver DS3231是LibDriver推出的DS3231全功能驅動，該驅動提供時間讀取、鬧鐘觸發、溫度讀取和方波輸出等功能並且它符合MISRA標準。

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

/misra目錄包含了LibDriver MISRA程式碼掃描結果。

### 安裝

參考/interface目錄下與平台無關的IIC總線模板，完成指定平台的IIC總線驅動。

將/src目錄，您使用平臺的介面驅動和您開發的驅動加入工程，如果您想要使用默認的範例驅動，可以將/example目錄加入您的工程。

### 使用

您可以參考/example目錄下的程式設計範例完成適合您的驅動，如果您想要使用默認的程式設計範例，以下是它們的使用方法。

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

### 文檔

在線文檔: [https://www.libdriver.com/docs/ds3231/index.html](https://www.libdriver.com/docs/ds3231/index.html)。

離線文檔: /doc/html/index.html。

### 貢獻

請參攷CONTRIBUTING.md。

### 版權

版權 (c) 2015 - 現在 LibDriver 版權所有

MIT 許可證（MIT）

特此免費授予任何獲得本軟件副本和相關文檔文件（下稱“軟件”）的人不受限制地處置該軟件的權利，包括不受限制地使用、複製、修改、合併、發布、分發、轉授許可和/或出售該軟件副本，以及再授權被配發了本軟件的人如上的權利，須在下列條件下：

上述版權聲明和本許可聲明應包含在該軟件的所有副本或實質成分中。

本軟件是“如此”提供的，沒有任何形式的明示或暗示的保證，包括但不限於對適銷性、特定用途的適用性和不侵權的保證。在任何情況下，作者或版權持有人都不對任何索賠、損害或其他責任負責，無論這些追責來自合同、侵權或其它行為中，還是產生於、源於或有關於本軟件以及本軟件的使用或其它處置。

### 聯繫我們

請聯繫lishifenging@outlook.com。
