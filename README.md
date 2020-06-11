## Deploy to Omnichip2
The following instructions will help you build and deploy this sample to [Omnichip2 development kits](https://www.zglue.com/products/devkit).

### Pre-requisites
Install Mbed CLI from [here](https://os.mbed.com/docs/mbed-os/v5.15/tools/installation-and-setup.html)

Install nRF Command Line Tools from [here](https://www.nordicsemi.com/Software-and-tools/Development-Tools/nRF-Command-Line-Tools)

### Build project
Run commands from Tensorflow_Lite/magic_wand/mbed/
```
mbed config root .
mbed deploy
mbed compile -m NRF52_DK -t GCC_ARM
```

### Program firmware


Run commands from Tensorflow_Lite/magic_wand/mbed/BUILD/NRF52_DK/GCC_ARM/
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


