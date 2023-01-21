### 1. Board

#### 1.1 Board Info

Board Name: Raspberry Pi 4B.

IIC Pin: SCL/SDA GPIO3/GPIO2.

GPIO Pin: INT GPIO17.

### 2. Install

#### 2.1 Dependencies

Install the necessary dependencies.

```shell
sudo apt-get install libgpiod-dev pkg-config cmake -y
```

#### 2.2 Makefile

Build the project.

```shell
make
```

Install the project and this is optional.

```shell
sudo make install
```

Uninstall the project and this is optional.

```shell
sudo make uninstall
```

#### 2.3 CMake

Build the project.

```shell
mkdir build && cd build 
cmake .. 
make
```

Install the project and this is optional.

```shell
sudo make install
```

Uninstall the project and this is optional.

```shell
sudo make uninstall
```

Test the project and this is optional.

```shell
make test
```

Find the compiled library in CMake. 

```cmake
find_package(ds3231 REQUIRED)
```

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
./ds3231 -i

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
./ds3231 -p

ds3231: SCL connected to GPIO3(BCM).
ds3231: SDA connected to GPIO2(BCM).
ds3231: INT connected to GPIO17(BCM).
```

```shell
./ds3231 -t reg

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
ds3231: set time 2073-11-16 PM 05:46:12 2.
ds3231: check time ok.
ds3231: set time 2139-02-03 02:19:23 6.
ds3231: check time ok.
ds3231: ds3231_set_alarm1/ds3231_get_alarm1 test.
ds3231: set alarm1 time 0000-00-01 PM 10:06:52 1.
ds3231: check alarm1 time ok.
ds3231: set alarm1 time 0000-00-01 11:08:27 3.
ds3231: check alarm1 time ok.
ds3231: set alarm1 time 0000-00-03 02:02:43 3.
ds3231: set alarm1 mode ALARM1_MODE_ONCE_A_SECOND.
ds3231: check alarm1 mode ok.
ds3231: set alarm1 time 0000-00-16 17:42:42 2.
ds3231: set alarm1 mode ALARM1_MODE_SECOND_MATCH.
ds3231: check alarm1 mode ok.
ds3231: set alarm1 time 0000-00-10 15:13:16 6.
ds3231: set alarm1 mode ALARM1_MODE_MINUTE_SECOND_MATCH.
ds3231: check alarm1 mode ok.
ds3231: set alarm1 time 0000-00-03 21:33:21 3.
ds3231: set alarm1 mode ALARM1_MODE_HOUR_MINUTE_SECOND_MATCH.
ds3231: check alarm1 mode ok.
ds3231: set alarm1 time 0000-00-05 01:38:44 1.
ds3231: set alarm1 mode ALARM1_MODE_DATE_HOUR_MINUTE_SECOND_MATCH.
ds3231: check alarm1 mode ok.
ds3231: set alarm1 time 0000-00-11 05:46:31 3.
ds3231: set alarm1 mode ALARM1_MODE_WEEK_HOUR_MINUTE_SECOND_MATCH.
ds3231: check alarm1 mode ok.
ds3231: ds3231_set_alarm2/ds3231_get_alarm2 test.
ds3231: set alarm2 time 0000-00-17 PM 07:02:00 2.
ds3231: check alarm2 time ok.
ds3231: set alarm2 time 0000-00-17 01:45:00 5.
ds3231: check alarm2 time ok.
ds3231: set alarm2 time 0000-00-05 15:56:00 7.
ds3231: set mode ALARM2_MODE_ONCE_A_MINUTE.
ds3231: check alarm2 time ok.
ds3231: set alarm2 time 0000-00-07 09:13:00 6.
ds3231: set mode ALARM2_MODE_MINUTE_MATCH.
ds3231: check alarm2 time ok.
ds3231: set alarm2 time 0000-00-05 15:42:00 1.
ds3231: set mode ALARM2_MODE_HOUR_MINUTE_MATCH.
ds3231: check alarm2 time ok.
ds3231: set alarm2 time 0000-00-15 15:14:00 4.
ds3231: set mode ALARM2_MODE_DATE_HOUR_MINUTE_MATCH.
ds3231: check alarm2 time ok.
ds3231: set alarm2 time 0000-00-04 14:07:00 1.
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
ds3231: set offset -56.
ds3231: check offset ok.
ds3231: ds3231_get_status test.
ds3231: check status 0x03.
ds3231: ds3231_aging_offset_convert_to_register/ds3231_aging_offset_convert_to_data test.
ds3231: offset is 6.
ds3231: offset is 0.760.
ds3231: convert offset is 0.720.
ds3231: finish register test.
```

```shell
./ds3231 -t alarm

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
ds3231: set time 2173-11-18 17:55:46 6 in 24 format.
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
./ds3231 -t readwrite --times=3

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
ds3231: set time 2073-11-16 PM 05:46:12 2 in 12 format.
ds3231: time is 2073-11-16 PM 05:46:13 2.
ds3231: time is 2073-11-16 PM 05:46:14 2.
ds3231: time is 2073-11-16 PM 05:46:15 2.
ds3231: set time 2139-02-03 14:19:23 6 in 24 format.
ds3231: time is 2139-02-03 14:19:24 6.
ds3231: time is 2139-02-03 14:19:25 6.
ds3231: time is 2139-02-03 14:19:26 6.
ds3231: read temperature.
ds3231: temperature is 26.50.
ds3231: temperature is 26.50.
ds3231: temperature is 26.50.
ds3231: finish readwrite test.
```

```shell
./ds3231 -t output --times=3

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
./ds3231 -e basic-set-time --timestamp=1612686500

ds3231: set timestamp 1612686500.
```

```shell
./ds3231 -e basic-get-time

ds3231: 2021-02-07 16:28:34 7.
```

```shell
./ds3231 -e basic-get-temperature

ds3231: temperature is 27.25C.
```

```shell
./ds3231 -e alarm-set-time --timestamp=1612686500

ds3231: set timestamp 1612686500.
```

```shell
./ds3231 -e alarm-get-time

ds3231: 2021-02-07 16:28:33 7.
```

```shell
./ds3231 -e alarm-get-temperature

ds3231: temperature is 27.25C.
```

```shell
./ds3231 -e alarm-get-alarm1

ds3231: 0000-00-18 17:55:46 0.
```

```shell
./ds3231 -e alarm-set-alarm1-mode --timestamp=1612686500 --mode1=ONCE_A_SECOND

ds3231: set alarm1 timestamp 1612715300 mode ONCE_A_SECOND.
```

```shell
./ds3231 -e alarm-set-alarm1 --enable=true

ds3231: irq alarm1.
ds3231: set alarm1 enable.
```

```shell
./ds3231 -e alarm-get-alarm2

ds3231: 0000-00-18 17:55:00 0.
```

```shell
./ds3231 -e alarm-set-alarm2-mode --timestamp=1612686500 --mode2=ONCE_A_MINUTE

ds3231: set alarm2 timestamp 1612715300 mode ONCE_A_MINUTE.
```

```shell
./ds3231 -e alarm-set-alarm2 --enable=true

ds3231: irq alarm2.
ds3231: set alarm2 enable.
```

```shell
./ds3231 -e output-set-time --timestamp=1612686500

ds3231: set timestamp 1612686500.
```

```shell
./ds3231 -e output-get-time

ds3231: 2021-02-07 16:29:51 7.
```

```shell
./ds3231 -e output-get-temperature

ds3231: temperature is 27.50C.
```

```shell
./ds3231 -e output-set-1Hz --enable=true

ds3231: set 1Hz output enable.
```

```shell
./ds3231 -e output-get-1Hz

ds3231: get 1Hz output enable.
```

```shell
./ds3231 -e output-set-32KHz --enable=true

ds3231: set 32KHz output enable.
```

```shell
./ds3231 -e output-get-32KHz

ds3231: get 32KHz output enable.
```

```shell
./ds3231 -h

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

