# RPIPicoW-OTA-Exp
OTA (Over the Air) Update Experiment on the Raspberry Pi Pico-W

Making use of [Jakub Zimnol](https://github.com/JZimnol) excellent [Firmware Over the Air](https://github.com/JZimnol/pico_fota_bootloader) bootloader to firmware upgrade my Pico-W.

The Firmware loader handles the update of a preloaded firmware held in flash to be the active application. It does that be reworking flash duing a reboot of the processor. The client applications needs to load the firmware from it's source.

## Bootloader
I included the [pico_fota_bootloader](https://github.com/JZimnol/pico_fota_bootloader) largely as Jakub suggests into a simple project. Building the project then creates two binaries the bootloader and the application firmware.  Both must be loaded onto the Pico the first time.

Jakub talks about doing this through Bootsel, actually I uploaded through openocd and Debug Probe which worked flawlessly.

## Downloading the binary
For test purposes I download the firmware over HTTP. Due to memory size limits on the Pico I decided to download it in seperate 10KB chunks. I could have made this smaller or larger. I chose to make each chunk a seperate HTTP Get request, because the write of each chunk into firmware is going to halt my IP stack, so holding open a GET request mid transfer seemed like a risky idea. Especially as I am not doing any CRC checking of the downloaded payload.

You will see the application is doing MD5 calculations on each segment and the total file using Caled Stewart [MD5 library](https://github.com/calebstewart/md5). This was purely for visual inspection during development. More serious validation is required for a production ready version of this.

HTTP GET is implimented using [FreeRTOS coreHTTP](https://github.com/FreeRTOS/coreHTTP) library.  This is therefore using LWIP and [FreeRTOS Kernel](https://github.com/FreeRTOS/FreeRTOS-Kernel) to support Sockets.

## Test Server
I have a simple Python Flask server as the delivery mechanism for the firmware. See the py folder. This just send segments and prints out the MD5 of each segment sent.  It is picking up the binaries directly from the build folders.

## Blue and Green Test Firmware
The test switches between two firmware releases Green (GRN) and Blue (BLU). Both are technically the same code. The CMakeLists.txt file within the grn/src and blu/src inserts three definitions to distinguish the releases in behaviour:

```
RELEASE="GRN"
OTA_URL="http://vmu22a.local.jondurrant.com:5000/blu"
LED_GP=14
```

+ RELEASE is a string printed at boot up
+ OTA_URL is the firmware release to load next
+ LED_GP is the LED GPIO pad to flash for 20 seconds before trying to update


## Clone
This project makes use of subprojects. Clone with the --recurse-submodules switch.

## Build and flash
Both the grn and blu firmware need to be build seperately using the standard build process.
```
mkdir build
cd build
cmake ..
make
```

You need to the flash pico_fota_bootloader/pico_fota_bootloader.elf or pico_fota_bootloader/pico_fota_bootloader.uf2 on the Pico.
Then flash either blu.elf (or blu.uf2) or grn.elf (or grn.uf2) to start the process.





