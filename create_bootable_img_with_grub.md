## Create a bootable img file with a grub and a linux kernel

Here is how I did it with my ubuntu 16.04 and my grub version: `grub-install (GRUB) 2.02~beta2-36ubuntu3.17`

The steps are as follow:

start with a 1.3G img as the kernel and the initrd are big:

`$ sudo dd if=/dev/sda1 of=d.img count=2560000`

Then follow [This answer](https://superuser.com/a/1213967/695175) to create a gpt table with a boot, a swap, and a file system partitions.
The only differece is that I put 1M+ for both the boot and the swap partitions, so that the last partition is big enough to hold the kernel and the initrd.
So I have:
```
$ fdisk -l d.img
Disk d.img: 1.2 GiB, 1310720000 bytes, 2560000 sectors
Units: sectors of 1 * 512 = 512 bytes
Sector size (logical/physical): 512 bytes / 512 bytes
I/O size (minimum/optimal): 512 bytes / 512 bytes
Disklabel type: gpt
Disk identifier: 4350B4E8-92B0-421A-89E4-BC8BFDAFB28B
 
Device       Start     End Sectors  Size Type
d.img1        2048 2099199 2097152    1G BIOS boot
d.img2     2099200 2119679   20480   10M Linux swap
d.img3     2119680 2559966  440287  215M Linux filesystem
``` 

So now, following different tutorials, I did:
```
$ sudo losetup  /dev/loop7 d.img
$ sudo kpartx -a /dev/loop7
$ sudo mkfs.ext2 /dev/mapper/loop7p3 
$ sudo mount /dev/mapper/loop7p3 /mnt #note! the 'Linux filesystem' partition should be mounted
$ sudo mkdir -p /mnt/boot/grub
$ sudo cp /boot/initrd.img-4.16.0-rc1+ /mnt/boot #copy my own kernel and initrd
$ sudo cp /boot/vmlinuz-4.16.0-rc1+ /mnt/boot
```

Now the grub should be configured and installed.
Thanks to these sites:

explanation of how grub works: https://www.dedoimedo.com/computers/grub-2.html

redhat article: https://access.redhat.com/documentation/en-us/red_hat_enterprise_linux/7/html/system_administrators_guide/sec-customizing_the_grub_2_configuration_file

The redhat article explains how to create a custom grub.cfg - this is done by adding your own menuentry in the `/etc/grub.d/40_custom`, then leaving only `/etc/grub.d/00_header` and `/etc/grub.d/40_custom` as executables - running `chmod a-x <file>` for each other
executable in `/etc/grub.d`

I copyed the entry from `/boot/grub/grub.cfg` that relates to the kernel I added to /mnt/boot. Then changed it a bit.
Notice that the entry should be added after the 
```
#!/bin/sh
exec tail -n +3 $0
```

So it is still executable

So this is how mine looks like:

```
$ cat /etc/grub.d/40_custom
#!/bin/sh
exec tail -n +3 $0
# This file provides an easy way to add custom menu entries.  Simply type the
# menu entries you want to add after this comment.  Be careful not to change
# the 'exec tail' line above.

menuentry 'Ubuntu, with Linux 4.16.0-rc1+' --class ubuntu --class gnu-linux --class gnu --class os $menuentry_id_option 'gnulinux-4.16.0-rc1+-advanced-3bb43708-f989-403d-bfde-b9e83d480832' {
                recordfail
                load_video
                gfxmode $linux_gfx_mode
                insmod gzio
                if [ x$grub_platform = xxen ]; then insmod xzio; insmod lzopio; fi
	        insmod part_gpt
	        insmod ext2
	        set root='hd0,gpt1'
		search --no-floppy --fs-uuid --set=root 3bb43708-f989-403d-bfde-b9e83d480832
		echo    'Loading Linux 4.16.0-rc1+ ...'
		linux   /boot/vmlinuz-4.16.0-rc1+ root=UUID=3bb43708-f989-403d-bfde-b9e83d480832 ro  splash $vt_handoff
		echo "loading the initrd"
		initrd	/boot/initrd.img-4.16.0-rc1+
}

```
The long UUID that appears 3 times in that file should match the uuid of the mounted partition, can get it with `sudo blkid`:

<pre>
$ sudo blkid
...
/dev/mapper/loop7p1: PARTLABEL="BIOS boot partition" PARTUUID="1366838c-e30e-42ed-8513-940eef2d0d31"
/dev/mapper/loop7p2: PARTLABEL="Linux swap" PARTUUID="2b2c9144-e556-43bf-a953-c5898079a04a"
/dev/mapper/loop7p3: <b>UUID="3bb43708-f989-403d-bfde-b9e83d480832"</b> TYPE="ext2" PARTLABEL="Linux filesystem" PARTUUID="5125035b-5cf0-4ea6-9340-fd80c3ba8f4d"
</pre>
  

now, do the following (**careful with grub commands - avoid accidentally running them on your real /boot/grub **)
```
$ sudo grub-mkconfig -o /mnt/boot/grub/grub.cfg
$ sudo grub-install -d /usr/lib/grub/i386-pc/ --boot-directory=/mnt/boot/ -v d.img 2> grub_install.log
```
finally undo the mounting stuff:
```
$ sudo umount /mnt
$ sudo kpartx -d /dev/loop7
$ sudo losetup -d /dev/loop7
```

That's it for now, I did not add a real filesystem to the image so when the kernel boots, it will try to run `/sbin/init`, it won't find it and will enter the initramfs shell.

Also, It takes it some time to load the initrd, better run qemu with a 4G ram:

```
$ sudo qemu-system-x86_64 -hda d.img -m 4G
```

**update:** it is very important to remove `/etc/grub.d/40_custom` after created and change the other scripts back to executables so that any command that upgrade grub will not ruin your `/boot/grub/grub.cfg` 



