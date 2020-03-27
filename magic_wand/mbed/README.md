## Deploy to Omnichip2
The following instructions will help you build and deploy this sample to [Omnichip2 development kits](https://www.zglue.com/products/devkit).

### Generate project 
Run command from /tensorflow/
```
make -f tensorflow/lite/micro/tools/make/Makefile TARGET=mbed TAGS="omnichip2" generate_magic_wand_mbed_project
```

Your project will be found here: /tensorflow/tensorflow/lite/micro/tools/make/gen/

### Copy over files 
From: /tensorflow/tensorflow/lite/micro/examples/magic_wand/omnichip2/replace_files/

- mbed_app.json goes to /../gen/mbed_cortex-m4/prj/magic_wand/mbed/
- mc3672_driver, constants.h, accelerometer_handler.h go to /../gen/mbed_cortex-m4/prj/magic_wand/mbed/tensorflow/lite/micro/examples/magic_wand/

### Build project
Run commands from /../gen/mbed_cortex-m4/prj/magic_wand/mbed/
```
mbed config root .
mbed deploy
mbed compile -m NRF52_DK -t GCC_ARM
```
### Program firmware
Install nrfjprog from [here](https://www.nordicsemi.com/Software-and-tools/Development-Tools/nRF-Command-Line-Tools/Download)
Run commands from /../gen/mbed_cortex-m4/prj/magic_wand/mbed/BUILD/NRF52_DK/GCC_ARM/
```
nrfjprog --program mbed.hex --sectorerase
nrfjprog -r
```

Open a serial terminal (e.g. Tera Term) with the following settings:
- Speed: 115200
- Data: 8 bit
- Parity: None
- Stop bits: 1 bit
- Flow Control: none

You will then see the following message:

```
Magic starts！
```

### Wave it around
Hold the board with its components facing upwards and the USB cable to your left. 
The following gestures are supported:

```
WING:
*         *         *
 *       * *       *
  *     *   *     *
   *   *     *   *
    * *       * *
     *         *
```

```
RING:
          *
       *     *
     *         *
    *           *
     *         *
       *     *
          *
```

```
SLOPE:
        *
       *
      *
     *
    *
   *
  *
 * * * * * * * *
```

```
LETTER 'A':
      *
     * *
    *   *
   * * * *
  *       *
 *         *
*           *
```


