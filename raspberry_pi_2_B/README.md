the kernel I compiled is 	https://github.com/raspberrypi/linux commit 240d0482ad9341f7d98b06705d0a55e298034121


adding debugs as in 0001-debugs-in-amba-uart-driver.patch , there is:

```
[    0.812811] pl011_probe: dafna: start
[    0.812870] pl011_probe: dafna: vendor=80c54034, vendor_arm=80c54034, portn=0
[    0.812930] uart-pl011 3f201000.serial: cts_event_workaround enabled
[    0.812968] pl011_probe: dafna: uap->reg_offset=80c540c0, pl011_std_offsets=80c540c0
[    0.813007] pl011_setup_port: dafna: mmiobase->start=3f201000, mmiobase->end=3f201fff, name=/soc/serial@7e201000
[    0.813145] 3f201000.serial: ttyAMA0 at MMIO 0x3f201000 (irq = 87, base_baud = 0) is a PL011 rev2
```

