### 1. chip

#### 1.1 chip info

chip name : STM32F407ZGT6.

extern oscillator : 8MHz.

uart pin: TX/RX PA9/PA10.

iic pin: SCL/SDA PB8/PB9.

gpio pin: INT PB0.

### 2. shell

#### 2.1 shell parameter

baud rate: 115200

data bits : 8

stop bits: 1

parity: none

flow control: none

### 3. ds3231

#### 3.1 command Instruction

​          ds3231 is a basic command which can test all ds3231 driver function:

​           -i        show ds3231 chip and driver information.

​           -h       show ds3231 help.

​           -p       show ds3231 pin connections of the current board.

​           -t (reg | readwrite <times>| output <times>)

​           -t reg        run ds3231 register test.

​           -t readwrite <times>        run ds3231 readwrite test. times means test times.

​           -t output <times>            run ds3231 output test. times means test times.

​           -c (basic (-set -time <timestamp> | -get (-time | -temperature)) | alarm (-set (-time <timestamp> | -alarm1 <timestamp> | -alarm2 <timestamp> | -alarm1 -m (ONCE_A_SECOND | SECOND_MATCH | MINUTE_SECOND_MATCH | HOUR_MINUTE_SECOND_MATCH | DATE_HOUR_MINUTE_SECOND_MATCH | WEEK_HOUR_MINUTE_SECOND_MATCH)) | -alarm2 -m (ONCE_A_MINUTE | MINUTE_MATCH | HOUR_MINUTE_MATCH | DATE_HOUR_MINUTE_MATCH | WEEK_HOUR_MINUTE_MATCH) | -get (-time | -temperature | -alarm1 | -alarm2))| output (-set (-time <timestamp> | -1Hz (enable | disable) | -32KHz (enable | disable))| -get ( -1Hz | -32KHz | -time | -temperature)))

​            -c basic -set -time <timestamp>        run ds3231 basic set time function. timestamp is the unix timestamp.

​            -c basic -get -time        run ds3231 basic get time function.

​            -c basic -get -temperature        run ds3231 basic get temperature function. 

​            -c alarm -set -time <timestamp>        run ds3231 alarm set time function. timestamp is the unix timestamp.

​            -c alarm -set -alarm1 <timestamp>        run ds3231 alarm set alarm1 function. timestamp is the unix timestamp.

​            -c alarm -set -alarm2 <timestamp>        run ds3231 alarm set alarm2 function. timestamp is the unix timestamp.

​            -c  alarm -set -alarm1 -m (ONCE_A_SECOND | SECOND_MATCH | MINUTE_SECOND_MATCH | HOUR_MINUTE_SECOND_MATCH | DATE_HOUR_MINUTE_SECOND_MATCH | WEEK_HOUR_MINUTE_SECOND_MATCH)        run ds3231 alarm set alarm1 function.

​             -c  alarm -set -alarm2 -m (ONCE_A_MINUTE | MINUTE_MATCH | HOUR_MINUTE_MATCH | DATE_HOUR_MINUTE_MATCH | WEEK_HOUR_MINUTE_MATCH)         run ds3231 alarm set alarm2 function.

​             -c alarm -get -time        run ds3231 alarm get time function.

​             -c alarm -get -temperature        run ds3231 alarm get temperature function.

​             -c alarm -get -alarm1        run ds3231 alarm get alarm1 function.

​             -c alarm -get -alarm2        run ds3231 alarm get alarm2 function.

​             -c output -set -time <timestamp>        run ds3231 output set time function. timestamp is the unix timestamp.

​             -c output -set -1Hz (enable | disable)        run ds3231 output set 1Hz function.

​             -c output -set -32KHz (enable | disable)        run ds3231 output set 32KHz function.

​             -c output -get -time        run ds3231 output get time function.

​             -c output -get -1Hz        run ds3231 output get 1Hz function.

​             -c output -get -32KHz        run ds3231 output get 32KHz function.

​             -c output -get -temperature        run ds3231 output get temperature function.

#### 3.2 command example

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
ds3231 -t readwrite 3

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
ds3231: set time 2058-04-01 PM 04:00:27 2 in 12 format.
ds3231: time is 2058-04-01 PM 04:00:28 2.
ds3231: time is 2058-04-01 PM 04:00:29 2.
ds3231: time is 2058-04-01 PM 04:00:30 2.
ds3231: set time 2097-05-09 18:33:57 3 in 24 format.
ds3231: time is 2097-05-09 18:33:58 3.
ds3231: time is 2097-05-09 18:33:59 3.
ds3231: time is 2097-05-09 18:34:00 3.
ds3231: read temperature.
ds3231: temperature is 28.25.
ds3231: temperature is 28.00.
ds3231: temperature is 28.25.
ds3231: finish readwrite test.
```

```shell
ds3231 -t output 3

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
ds3231 -c basic -set -time 1612686500

ds3231: set timestamp 1612686500.
```

```shell
ds3231 -c basic -get -time 

ds3231: 2021-02-07 16:27:27 7.
```

```shell
ds3231 -c basic -get -temperature

ds3231: temperature is 26.50C.
```

```shell
ds3231 -c alarm -set -time 1612686500

ds3231: set timestamp 1612686500.
```

```shell
ds3231 -c alarm -get -time 

ds3231: 2021-02-07 16:28:38 7.
```

```shell
ds3231 -c alarm -get -temperature

ds3231: temperature is 26.50C.
```

```shell
ds3231 -c alarm -get -alarm1

ds3231: 0000-00-07 16:28:20 0.
```

```shell
ds3231 -c alarm -set -alarm1 1612686500 -m ONCE_A_SECOND

ds3231: set alarm1 timestamp 1612715300 mode ONCE_A_SECOND.
```

```shell
ds3231 -c alarm -set -alarm1 enable

ds3231: set alarm1 enable.
ds3231: irq alarm1.
```

```shell
ds3231 -c alarm -get -alarm2

ds3231: 0000-00-07 16:28:00 0.
```

```shell
ds3231 -c alarm -set -alarm2 1612686500 -m ONCE_A_MINUTE

ds3231: set alarm2 timestamp 1612715300 mode ONCE_A_MINUTE.
```

```shell
ds3231 -c alarm -set -alarm2 enable

ds3231: set alarm2 enable.
ds3231: irq alarm2.
```

```shell
ds3231 -c output -set -time 1612686500

ds3231: set timestamp 1612686500.
```

```shell
ds3231 -c output -get -time 

ds3231: 2021-02-07 16:29:44 7.
```

```shell
ds3231 -c output -get -temperature

ds3231: temperature is 26.25C.
```

```shell
ds3231 -c output -set -1Hz enable

ds3231: set 1Hz output enable.
```

```shell
ds3231 -c output -get -1Hz

ds3231: get 1Hz output disable.
```

```shell
ds3231 -c output -set -32KHz enable

ds3231: set 32KHz output enable.
```

```shell
ds3231 -c output -get -32KHz

ds3231: get 32KHz output enable.
```

```shell
ds3231 -h

ds3231 -i
	show ds3231 chip and driver information.
ds3231 -h
	show ds3231 help.
ds3231 -p
	show ds3231 pin connections of the current board.
ds3231 -t reg
	run ds3231 register test.
ds3231 -t readwrite <times>
	run ds3231 readwrite test.times means test times.
ds3231 -t output <times>
	run ds3231 output test.times means test times.
ds3231 -c basic -set -time <timestamp>
	run ds3231 basic set time function.timestamp is the unix timestamp.
ds3231 -c basic -get -time
	run ds3231 basic get time function.
ds3231 -c basic -get -temperature
	run ds3231 basic get temperature function.
ds3231 -c alarm -set -time <timestamp>
	run ds3231 alarm set time function.timestamp is the unix timestamp.
ds3231 -c alarm -set -alarm1 <timestamp>
	run ds3231 alarm set alarm1 function.timestamp is the unix timestamp.
ds3231 -c alarm -set -alarm2 <timestamp>
	run ds3231 alarm set alarm2 function.timestamp is the unix timestamp.
ds3231 -c alarm -set -alarm1 -m (ONCE_A_SECOND | SECOND_MATCH | MINUTE_SECOND_MATCH | HOUR_MINUTE_SECOND_MATCH | DATE_HOUR_MINUTE_SECOND_MATCH | WEEK_HOUR_MINUTE_SECOND_MATCH)
	run ds3231 alarm set alarm1 function.
ds3231 -c alarm -set -alarm2 -m (ONCE_A_MINUTE | MINUTE_MATCH | HOUR_MINUTE_MATCH | DATE_HOUR_MINUTE_MATCH | WEEK_HOUR_MINUTE_MATCH)
	run ds3231 alarm set alarm2 function.
ds3231 -c alarm -get -time
	run ds3231 alarm get time function.
ds3231 -c alarm -get -temperature
	run ds3231 alarm get temperature function.
ds3231 -c alarm -get -alarm1
	run ds3231 alarm get alarm1 function.
ds3231 -c alarm -get -alarm2
	run ds3231 alarm get alarm2 function.
ds3231 -c output -set -time <timestamp>
	run ds3231 output set time function.timestamp is the unix timestamp.
ds3231 -c output -set -1Hz (enable | disable)
	run ds3231 output set 1Hz function.
ds3231 -c output -set -32KHz (enable | disable)
	run ds3231 output set 32KHz function.
ds3231 -c output -get -time
	run ds3231 output get time function.
ds3231 -c output -get -1Hz
	run ds3231 output get 1Hz function.
ds3231 -c output -get -32KHz
	run ds3231 output get 32KHz function.
ds3231 -c output -get -temperature
	run ds3231 output get temperature function.
```

