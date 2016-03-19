
# Senior Design Project

## Overview


## BeagleBone Device Tree

This is device-tree overlay for GPIOs used in Blaze



```
/*
* This is device-tree overlay for GPIOs used in Blaze
*/

/dts-v1/;
/plugin/;

/{
compatible = "ti,beaglebone", "ti,beaglebone-black";
part_number = "BLAZE_GPIOs";

fragment@0 {
target = <&am33xx_pinmux>;
__overlay__ {
blaze_gpios: BLAZE_GPIOs {
pinctrl-single,pins = <
0x034 0x7       /* P8_11 0x7 Output Mode 7 Pulldown - PWM 2 Converter Enable/Disable */
0x028 0x7       /* P8_14 0x7 Output Mode 7 Pulldown - M1 - Big Easy Driver */
0x038 0x7       /* P8_16 0x7 Output Mode 7 Pulldown - EN - Big Easy Driver */
0x0b8 0x7       /* P8_39 LED3 */
0x0bc 0x7       /* P8_40 LED4 */
0x0b0 0x7       /* P8_41 LED1 */
0x0b4 0x7       /* P8_42 LED2 */
0x0a8 0x7       /* P8_43 0x7 Output Mode 7 Pulldown - DIR 2 - PWM Dual Motor Controller */
0x0ac 0x7       /* P8_44 0x7 Output Mode 7 Pulldown - DIR 1 - PWM Dual Motor Controller */
0x1ac 0x37      /* P9_25 0x7 Input Mode 7 Pullup - Switch 1 */
0x1a4 0x37      /* P9_27 0x7 Input Mode 7 Pullup - Switch 2 */
0x164 0x37      /* P9_42 0x7 Output Mode 7 Pulldown - M2 - Big Easy */
>;
};
};
};

fragment@1 {
target = <&ocp>;
__overlay__ {
blaze_gpios_pinmux {
compatible = "bone-pinmux-helper";
status = "okay";
pinctrl-names = "default";
pinctrl-0 = <&blaze_gpios>;
};
};
};
};

```




### Compile ON BBB:
```
sudo dtc -O dtb -o /lib/firmware/blaze_gpios-00A0.dtbo -b 0 -@ /lib/firmware/blaze_gpios-00A0.dts
```

put in /etc/rc.local:

echo blaze_gpios > /sys/devices/bone_capemgr.9/slots
echo 72 > /sys/class/gpio/export







