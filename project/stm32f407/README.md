### 1. Chip

#### 1.1 Chip Info

Chip Name: STM32F407ZGT6.

Extern Oscillator: 8MHz.

UART Pin: TX/RX PA9/PA10.

IIC Pin: SCL/SDA PB8/PB9.

GPIO Pin: INT PB0.

### 2. Development and Debugging

#### 2.1 Integrated Development Environment

LibDriver provides both Keil and IAR integrated development environment projects.

MDK is the Keil ARM project and your Keil version must be 5 or higher.Keil ARM project needs STMicroelectronics STM32F4 Series Device Family Pack and you can download from https://www.keil.com/dd2/stmicroelectronics/stm32f407zgtx.

EW is the IAR ARM project and your IAR version must be 9 or higher.

#### 2.2 Serial Port Parameter

Baud Rate: 115200.

Data Bits : 8.

Stop Bits: 1.

Parity: None.

Flow Control: None.

#### 2.3 Serial Port Assistant

We use '\n' to wrap lines.If your serial port assistant displays exceptions (e.g. the displayed content does not divide lines), please modify the configuration of your serial port assistant or replace one that supports '\n' parsing.

### 3. DS3231

#### 3.1 Command Instruction

1. Show ds3231 chip and driver information.

   ```shell
   ds3231 (-i | --information)
   ```

2. Show ds3231 help.

   ```shell
   ds3231 (-h | --help)
   ```

3. Show ds3231 pin connections of the current board.

   ```shell
   ds3231 (-p | --port)
   ```

4. Run ds3231 register test.

   ```shell
   ds3231 (-t reg | --test=reg)
   ```

5. Run ds3231 alarm test.

   ```shell
   ds3231 (-t alarm | --test=alarm)
   ```

6. Run ds3231 readwrite test, num means test times.

   ```shell
   ds3231 (-t readwrite | --test=readwrite) [--times=<num>]
   ```

7. Run ds3231 output test, num means test times.

   ```shell
   ds3231 (-t output | --test=output) [--times=<num>]
   ```

8. Run ds3231 basic set time function, time is the unix timestamp.

   ```shell
   ds3231 (-e basic-set-time | --example=basic-set-time) --timestamp=<time>
   ```

9. Run ds3231 basic get time function.

   ```shell
   ds3231 (-e basic-get-time | --example=basic-get-time)
   ```

10. Run ds3231 basic get temperature function. 

    ```shell
    ds3231 (-e basic-get-temperature | --example=basic-get-temperature)
    ```

11. Run ds3231 alarm set time function, time is the unix timestamp.

    ```shell
    ds3231 (-e alarm-set-time | --example=alarm-set-time) --timestamp=<time>
    ```

12. Run ds3231 alarm set alarm1 function.

    ```shell
    ds3231 (-e alarm-set-alarm1 | --example=alarm-set-alarm1) --enable=<true | false>
    ```

13. Run ds3231 alarm set alarm2 function.

    ```shell
    ds3231 (-e alarm-set-alarm2 | --example=alarm-set-alarm2) --enable=<true | false>
    ```

14. Run ds3231 alarm set alarm1 function, time is the unix timestamp.

    ```shell
    ds3231 (-e alarm-set-alarm1-mode | --example=alarm-set-alarm1-mode)  --timestamp=<time> --mode1=<ONCE_A_SECOND | SECOND_MATCH | MINUTE_SECOND_MATCH | HOUR_MINUTE_SECOND_MATCH | DATE_HOUR_MINUTE_SECOND_MATCH | WEEK_HOUR_MINUTE_SECOND_MATCH>
    ```

15. Run ds3231 alarm set alarm2 function, time is the unix timestamp.

    ```shell
    ds3231 (-e alarm-set-alarm2-mode | --example=alarm-set-alarm2-mode)  --timestamp=<time> --mode2=<ONCE_A_MINUTE | MINUTE_MATCH | HOUR_MINUTE_MATCH | DATE_HOUR_MINUTE_MATCH | WEEK_HOUR_MINUTE_MATCH>
    ```

16. Run ds3231 alarm get time function.

    ```shell
    ds3231 (-e alarm-get-time | --example=alarm-get-time)
    ```

17. Run ds3231 alarm get temperature function.

    ```shell
    ds3231 (-e alarm-get-temperature | --example=alarm-get-temperature)
    ```

18. Run ds3231 alarm get alarm1 function.

    ```shell
    ds3231 (-e alarm-get-alarm1 | --example=alarm-get-alarm1)
    ```

19. Run ds3231 alarm get alarm2 function.

    ```shell
    ds3231 (-e alarm-get-alarm2 | --example=alarm-get-alarm2)
    ```

20. Run ds3231 output set time function, time is the unix timestamp.

    ```shell
    ds3231 (-e output-set-time | --example=output-set-time) --timestamp=<time>
    ```

21. Run ds3231 output set 1Hz function.

    ```shell
    ds3231 (-e output-set-1Hz | --example=output-set-1Hz) --enable=<true | false>
    ```

22. Run ds3231 output set 32KHz function.

    ```shell
    ds3231 (-e output-set-32KHz | --example=output-set-32KHz) --enable=<true | false>
    ```

23. Run ds3231 output get time function.

    ```shell
    ds3231 (-e output-get-time | --example=output-get-time)
    ```

24. Run ds3231 output get 1Hz function.

    ```shell
    ds3231 (-e output-get-1Hz | --example=output-get-1Hz)
    ```

25. Run ds3231 output get 32KHz function.

    ```shell
    ds3231 (-e output-get-32KHz | --example=output-get-32KHz)
    ```

26. Run ds3231 output get temperature function.

    ```shell
    ds3231 (-e output-get-temperature | --example=output-get-temperature)
    ```

#### 3.2 Command Example

```shell
ds3231 -i

ds3231: chip is Maxim Integrated DS3231.
ds3231: manufacturer is Maxim Integrated.
ds3231: interface is IIC.
ds3231: driver version is 2.0.
ds3231: min supply voltage is 2.3V.
ds3231: max supply voltage is 5.5V.
ds3231: max current is 0.65mA.
ds3231: max temperature is 85.0C.
ds3231: min temperature is -40.0C.
```

```shell
ds3231 -p

ds3231: SCL connected to GPIOB PIN8.
ds3231: SDA connected to GPIOB PIN9.
ds3231: INT connected to GPIOB PIN0.
```

```shell
ds3231 -t reg

ds3231: chip is Maxim Integrated DS3231.
ds3231: manufacturer is Maxim Integrated.
ds3231: interface is IIC.
ds3231: driver version is 2.0.
ds3231: min supply voltage is 2.3V.
ds3231: max supply voltage is 5.5V.
ds3231: max current is 0.65mA.
ds3231: max temperature is 85.0C.
ds3231: min temperature is -40.0C.
ds3231: start register test.
ds3231: ds3231_set_time/ds3231_get_time test.
ds3231: set time 2058-04-01 PM 04:00:27 2.
ds3231: check time ok.
ds3231: set time 2097-05-09 18:33:57 3.
ds3231: check time ok.
ds3231: ds3231_set_alarm1/ds3231_get_alarm1 test.
ds3231: set alarm1 time 0000-00-01 PM 07:56:09 1.
ds3231: check alarm1 time ok.
ds3231: set alarm1 time 0000-00-01 06:22:25 4.
ds3231: check alarm1 time ok.
ds3231: set alarm1 time 0000-00-10 00:32:57 5.
ds3231: set alarm1 mode ALARM1_MODE_ONCE_A_SECOND.
ds3231: check alarm1 mode ok.
ds3231: set alarm1 time 0000-00-13 07:48:41 3.
ds3231: set alarm1 mode ALARM1_MODE_SECOND_MATCH.
ds3231: check alarm1 mode ok.
ds3231: set alarm1 time 0000-00-19 17:31:09 5.
ds3231: set alarm1 mode ALARM1_MODE_MINUTE_SECOND_MATCH.
ds3231: check alarm1 mode ok.
ds3231: set alarm1 time 0000-00-20 17:35:19 1.
ds3231: set alarm1 mode ALARM1_MODE_HOUR_MINUTE_SECOND_MATCH.
ds3231: check alarm1 mode ok.
ds3231: set alarm1 time 0000-00-09 21:45:58 1.
ds3231: set alarm1 mode ALARM1_MODE_DATE_HOUR_MINUTE_SECOND_MATCH.
ds3231: check alarm1 mode ok.
ds3231: set alarm1 time 0000-00-16 03:42:15 4.
ds3231: set alarm1 mode ALARM1_MODE_WEEK_HOUR_MINUTE_SECOND_MATCH.
ds3231: check alarm1 mode ok.
ds3231: ds3231_set_alarm2/ds3231_get_alarm2 test.
ds3231: set alarm2 time 0000-00-17 PM 08:08:00 1.
ds3231: check alarm2 time ok.
ds3231: set alarm2 time 0000-00-11 00:04:00 7.
ds3231: check alarm2 time ok.
ds3231: set alarm2 time 0000-00-08 06:37:00 5.
ds3231: set mode ALARM2_MODE_ONCE_A_MINUTE.
ds3231: check alarm2 time ok.
ds3231: set alarm2 time 0000-00-19 23:01:00 6.
ds3231: set mode ALARM2_MODE_MINUTE_MATCH.
ds3231: check alarm2 time ok.
ds3231: set alarm2 time 0000-00-19 16:54:00 2.
ds3231: set mode ALARM2_MODE_HOUR_MINUTE_MATCH.
ds3231: check alarm2 time ok.
ds3231: set alarm2 time 0000-00-05 12:37:00 6.
ds3231: set mode ALARM2_MODE_DATE_HOUR_MINUTE_MATCH.
ds3231: check alarm2 time ok.
ds3231: set alarm2 time 0000-00-19 01:24:00 1.
ds3231: set mode ALARM2_MODE_WEEK_HOUR_MINUTE_MATCH.
ds3231: check alarm2 time ok.
ds3231: ds3231_set_oscillator/ds3231_get_oscillator test.
ds3231: enable.
ds3231: check bool ok.
ds3231: disable.
ds3231: check bool ok.
ds3231: ds3231_set_alarm_interrupt/ds3231_get_alarm_interrupt test.
ds3231: alarm1 enable.
ds3231: check alarm1 bool ok.
ds3231: alarm1 disable.
ds3231: check alarm1 bool ok.
ds3231: alarm2 enable.
ds3231: check alarm2 bool ok.
ds3231: alarm2 disable.
ds3231: check alarm2 bool ok.
ds3231: ds3231_set_pin/ds3231_get_pin test.
ds3231: set square wave pin.
ds3231: check pin ok.
ds3231: set interrupt pin.
ds3231: check pin ok.
ds3231: ds3231_set_square_wave/ds3231_get_square_wave test.
ds3231: set enable.
ds3231: check bool ok.
ds3231: set disable.
ds3231: check bool ok.
ds3231: ds3231_set_32khz_output/ds3231_get_32khz_output test.
ds3231: enable.
ds3231: check bool ok.
ds3231: disable.
ds3231: check bool ok.
ds3231: ds3231_set_aging_offset/ds3231_get_aging_offset test.
ds3231: set offset -9.
ds3231: check offset ok.
ds3231: ds3231_get_status test.
ds3231: check status 0x03.
ds3231: ds3231_aging_offset_convert_to_register/ds3231_aging_offset_convert_to_data test.
ds3231: offset is 5.
ds3231: offset is 0.670.
ds3231: convert offset is 0.600.
ds3231: finish register test.
```

```shell
ds3231 -t alarm

ds3231: chip is Maxim Integrated DS3231.
ds3231: manufacturer is Maxim Integrated.
ds3231: interface is IIC.
ds3231: driver version is 2.0.
ds3231: min supply voltage is 2.3V.
ds3231: max supply voltage is 5.5V.
ds3231: max current is 0.65mA.
ds3231: max temperature is 85.0C.
ds3231: min temperature is -40.0C.
ds3231: start alarm test.
ds3231: set time 2158-04-10 15:41:00 7 in 24 format.
ds3231: check alarm1 ok.
ds3231: time left 60s.
ds3231: time left 59s.
ds3231: time left 58s.
ds3231: time left 57s.
ds3231: time left 56s.
ds3231: time left 55s.
ds3231: time left 54s.
ds3231: time left 53s.
ds3231: time left 52s.
ds3231: time left 51s.
ds3231: time left 50s.
ds3231: time left 49s.
ds3231: time left 48s.
ds3231: time left 47s.
ds3231: time left 46s.
ds3231: time left 45s.
ds3231: time left 44s.
ds3231: time left 43s.
ds3231: time left 42s.
ds3231: time left 41s.
ds3231: time left 40s.
ds3231: time left 39s.
ds3231: time left 38s.
ds3231: time left 37s.
ds3231: time left 36s.
ds3231: time left 35s.
ds3231: time left 34s.
ds3231: time left 33s.
ds3231: time left 32s.
ds3231: time left 31s.
ds3231: time left 30s.
ds3231: time left 29s.
ds3231: time left 28s.
ds3231: time left 27s.
ds3231: time left 26s.
ds3231: time left 25s.
ds3231: time left 24s.
ds3231: time left 23s.
ds3231: time left 22s.
ds3231: time left 21s.
ds3231: time left 20s.
ds3231: time left 19s.
ds3231: time left 18s.
ds3231: time left 17s.
ds3231: time left 16s.
ds3231: time left 15s.
ds3231: time left 14s.
ds3231: time left 13s.
ds3231: time left 12s.
ds3231: time left 11s.
ds3231: time left 10s.
ds3231: time left 9s.
ds3231: time left 8s.
ds3231: time left 7s.
ds3231: time left 6s.
ds3231: time left 5s.
ds3231: time left 4s.
ds3231: time left 3s.
ds3231: time left 2s.
ds3231: time left 1s.
ds3231: time left 0s.
ds3231: check alarm2 ok.
ds3231: finish alarm test.
```

```shell
ds3231 -t readwrite --times=3

ds3231: chip is Maxim Integrated DS3231.
ds3231: manufacturer is Maxim Integrated.
ds3231: interface is IIC.
ds3231: driver version is 2.0.
ds3231: min supply voltage is 2.3V.
ds3231: max supply voltage is 5.5V.
ds3231: max current is 0.65mA.
ds3231: max temperature is 85.0C.
ds3231: min temperature is -40.0C.
ds3231: start readwrite test.
ds3231: ds3231_set_time/ds3231_get_time test.
ds3231: set time 2039-05-02 PM 04:29:28 3 in 12 format.
ds3231: time is 2039-05-02 PM 04:29:29 3.
ds3231: time is 2039-05-02 PM 04:29:30 3.
ds3231: time is 2039-05-02 PM 04:29:31 3.
ds3231: set time 2123-10-19 23:39:42 4 in 24 format.
ds3231: time is 2123-10-19 23:39:43 4.
ds3231: time is 2123-10-19 23:39:44 4.
ds3231: time is 2123-10-19 23:39:45 4.
ds3231: read temperature.
ds3231: temperature is 30.00.
ds3231: temperature is 30.00.
ds3231: temperature is 30.00.
ds3231: finish readwrite test.
```

```shell
ds3231 -t output --times=3

ds3231: chip is Maxim Integrated DS3231.
ds3231: manufacturer is Maxim Integrated.
ds3231: interface is IIC.
ds3231: driver version is 2.0.
ds3231: min supply voltage is 2.3V.
ds3231: max supply voltage is 5.5V.
ds3231: max current is 0.65mA.
ds3231: max temperature is 85.0C.
ds3231: min temperature is -40.0C.
ds3231: start output test.
ds3231: enable square wave.
ds3231: run 1s.
ds3231: run 2s.
ds3231: run 3s.
ds3231: disable square wave.
ds3231: enable 32khz output.
ds3231: run 1s.
ds3231: run 2s.
ds3231: run 3s.
ds3231: disable 32khz output.
ds3231: finish output test.
```

```shell
ds3231 -e basic-set-time --timestamp=1612686500

ds3231: set timestamp 1612686500.
```

```shell
ds3231 -e basic-get-time

ds3231: 2021-02-07 16:28:31 7.
```

```shell
ds3231 -e basic-get-temperature

ds3231: temperature is 30.00C.
```

```shell
ds3231 -e alarm-set-time --timestamp=1612686500

ds3231: set timestamp 1612686500.
```

```shell
ds3231 -e alarm-get-time

ds3231: 2021-02-07 16:28:29 7.
```

```shell
ds3231 -e alarm-get-temperature

ds3231: temperature is 30.00C.
```

```shell
ds3231 -e alarm-get-alarm1

ds3231: 0000-00-10 15:41:00 0.
```

```shell
ds3231 -e alarm-set-alarm1-mode --timestamp=1612686500 --mode1=ONCE_A_SECOND

ds3231: set alarm1 timestamp 1612715300 mode ONCE_A_SECOND.
```

```shell
ds3231 -e alarm-set-alarm1 --enable=true

ds3231: set alarm1 enable.
ds3231: irq alarm1.
ds3231: irq alarm1.
```

```shell
ds3231 -e alarm-get-alarm2

ds3231: 0000-00-10 15:41:00 0.
```

```shell
ds3231 -e alarm-set-alarm2-mode --timestamp=1612686500 --mode2=ONCE_A_MINUTE

ds3231: set alarm2 timestamp 1612715300 mode ONCE_A_MINUTE.
```

```shell
ds3231 -e alarm-set-alarm2 --enable=true

ds3231: set alarm2 enable.
ds3231: irq alarm2.
```

```shell
ds3231 -e output-set-time --timestamp=1612686500

ds3231: set timestamp 1612686500.
```

```shell
ds3231 -e output-get-time

ds3231: 2021-02-07 16:28:31 7.
```

```shell
ds3231 -e output-get-temperature

ds3231: temperature is 30.25C.
```

```shell
ds3231 -e output-set-1Hz --enable=true

ds3231: set 1Hz output enable.
```

```shell
ds3231 -e output-get-1Hz

ds3231: get 1Hz output enable.
```

```shell
ds3231 -e output-set-32KHz --enable=true

ds3231: set 32KHz output enable.
```

```shell
ds3231 -e output-get-32KHz

ds3231: get 32KHz output enable.
```

```shell
ds3231 -h

Usage:
  ds3231 (-i | --information)
  ds3231 (-h | --help)
  ds3231 (-p | --port)
  ds3231 (-t reg | --test=reg)
  ds3231 (-t alarm | --test=alarm)
  ds3231 (-t readwrite | --test=readwrite) [--times=<num>]
  ds3231 (-t output | --test=output) [--times=<num>]
  ds3231 (-e basic-set-time | --example=basic-set-time) --timestamp=<time>
  ds3231 (-e basic-get-time | --example=basic-get-time)
  ds3231 (-e basic-get-temperature | --example=basic-get-temperature)
  ds3231 (-e alarm-set-time | --example=alarm-set-time) --timestamp=<time>
  ds3231 (-e alarm-set-alarm1 | --example=alarm-set-alarm1) --enable=<true | false>
  ds3231 (-e alarm-set-alarm2 | --example=alarm-set-alarm2) --enable=<true | false>
  ds3231 (-e alarm-set-alarm1-mode | --example=alarm-set-alarm1-mode) --timestamp=<time>
         --mode1=<ONCE_A_SECOND | SECOND_MATCH | MINUTE_SECOND_MATCH | HOUR_MINUTE_SECOND_MATCH | DATE_HOUR_MINUTE_SECOND_MATCH | WEEK_HOUR_MINUTE_SECOND_MATCH>
  ds3231 (-e alarm-set-alarm2-mode | --example=alarm-set-alarm2-mode) --timestamp=<time>
         --mode2=<ONCE_A_MINUTE | MINUTE_MATCH | HOUR_MINUTE_MATCH | DATE_HOUR_MINUTE_MATCH | WEEK_HOUR_MINUTE_MATCH>
  ds3231 (-e alarm-get-time | --example=alarm-get-time)
  ds3231 (-e alarm-get-temperature | --example=alarm-get-temperature)
  ds3231 (-e alarm-get-alarm1 | --example=alarm-get-alarm1)
  ds3231 (-e alarm-get-alarm2 | --example=alarm-get-alarm2)
  ds3231 (-e output-set-time | --example=output-set-time) --timestamp=<time>
  ds3231 (-e output-set-1Hz | --example=output-set-1Hz) --enable=<true | false>
  ds3231 (-e output-set-32KHz | --example=output-set-32KHz) --enable=<true | false>
  ds3231 (-e output-get-time | --example=output-get-time)
  ds3231 (-e output-get-1Hz | --example=output-get-1Hz)
  ds3231 (-e output-get-32KHz | --example=output-get-32KHz)
  ds3231 (-e output-get-temperature | --example=output-get-temperature)

Options:
  -e <basic-set-time | basic-get-time | basic-get-temperature | alarm-set-time | alarm-set-alarm1 | alarm-set-alarm2 | alarm-set-alarm1-mode
     | alarm-set-alarm2-mode | alarm-get-time | alarm-get-temperature | alarm-get-alarm1 | alarm-get-alarm2 | output-set-time | output-set-1Hz
     | output-set-32KHz | output-get-time | output-get-1Hz | output-get-32KHz | output-get-temperature>, --example=<basic-set-time | basic-get-time 
     | basic-get-temperature | alarm-set-time | alarm-set-alarm1 | alarm-set-alarm2 | alarm-set-alarm1-mode | alarm-set-alarm2-mode | alarm-get-time 
     | alarm-get-temperature | alarm-get-alarm1 | alarm-get-alarm2 | output-set-time | output-set-1Hz | output-set-32KHz | output-get-time 
     | output-get-1Hz | output-get-32KHz | output-get-temperature>
                                  Run the driver example.
      --enable=<true | false>     Enable or disable the function.
  -h, --help                      Show the help.
  -i, --information               Show the chip information.
      --mode1=<ONCE_A_SECOND | SECOND_MATCH | MINUTE_SECOND_MATCH | HOUR_MINUTE_SECOND_MATCH
              | DATE_HOUR_MINUTE_SECOND_MATCH | WEEK_HOUR_MINUTE_SECOND_MATCH>
                                  Set the alarm1 mode.
      --mode2=<ONCE_A_MINUTE | MINUTE_MATCH | HOUR_MINUTE_MATCH | DATE_HOUR_MINUTE_MATCH | WEEK_HOUR_MINUTE_MATCH>
                                  Set the alarm2 mode.
  -p, --port                      Display the pin connections of the current board.
  -t <reg | alarm | readwrite | output>, --test=<reg | alarm | readwrite | output>
                                  Run the driver test.
      --times=<num>               Set the running times.([default: 3])
      --timestamp=<time>          Set the the unix timestamp.
```

