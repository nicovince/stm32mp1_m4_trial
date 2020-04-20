# Compilation Instructions
## Download and compile
 1. `git clone --recurse-submodules https://github.com/nicovince/stm32mp1_m4_trial.git`
 2. `cd stm32mp1_m4_trial`
 3. `make -C libopencm3 # (Only needed once)`
 4. `make -C <project>`

If you have an older git, or got ahead of yourself and skipped the ```--recurse-submodules```
you can fix things by running ```git submodule update --init``` (This is only needed once)


# Board Setup Instructions
## Serial console
You need to connect a micro-usb cable from your workstation to the board, this micro-usb is a serial console configured at 115200 baud, 8 databits, 1 stopbits, no parity :
```screen /dev/ttyACM0 115200 8N1```

You can then power up the board, by connecting the board with the USB-C connector which is between the micro-SD slot and the ethernet.

## Device tree
During the boot, you are prompted to select the device tree, you have 3 seconds to select the one where peripherals are assigned to the m4 (option 3) :
```
Select the boot mode
1:      stm32mp157c-dk2-sdcard-optee
2:      stm32mp157c-dk2-a7-examples-sdcard-optee
3:      stm32mp157c-dk2-m4-examples-sdcard-optee
Enter choice: 3
```

## Run a m4 firmware
Once your firmwares are on the m4 (scp, installed through yocto, ...) you need to copy them in the `/lib/firmware` folder, select the firmware to run and start the m4 with the selected one:
```
cp program.elf /lib/firmware/
echo program.elf > /sys/class/remoteproc/remoteproc0/firmware
echo start > /sys/class/remoteproc/remoteproc0/state
```

## Stop the m4 firmware
Issue the following command:
```
echo stop > /sys/class/remoteproc/remoteproc0/state
```

## M4 uart
[STM32MP1-DK2](https://wiki.st.com/stm32mpu/wiki/STM32MP157X-DKX_-_hardware_description#GPIO_expansion_connector) indicates USART3 pinout on the 40 pins connector:
| function | Pin number |
| -------- |:---------- |
| GND      | pin 6      |
| Tx       | pin 8      |
| Rx       | Pin 10     |

