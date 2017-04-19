#include "lab_miscdev.h"
#include <asm/io.h>
#include <linux/memblock.h>
#include <asm/fixmap.h>
#include <asm/memory.h>
#include "print_things.h"

int my_global;
static long printing_add_zone(void){

  //void * vp;
  int my_local;
  unsigned char *p = phys_to_virt(0); //#define __phys_to_virt(x)       ((unsigned long)((x) - PHYS_OFFSET) | PAGE_OFFSET)
  char *gfp_kern = kmalloc(128, GFP_KERNEL);
  char *gfp_atomic = kmalloc(128, GFP_ATOMIC);
  char *gfp_dma = kmalloc(128, GFP_DMA);
  char *gfp_dma32 = kmalloc(128, GFP_DMA32);
  char *gfp_noio = kmalloc(128, GFP_NOIO);
  char *gfp_user = kmalloc(128, GFP_USER);
  char *gfp_highuser = kmalloc(128, GFP_HIGHUSER);
  char *vmalloced = vmalloc(128);

  print_all_kinds();
  print_mem_regs();
  printk(KERN_INFO "checking range for address of local module variable:\n");
  print_virtual_reg(&my_local);
  printk(KERN_INFO "checking range for address of global module variable:\n");
  print_virtual_reg(&my_global);
  printk(KERN_INFO "checking range for address allocated with GFP_KERNEL:\n");
  print_virtual_reg(gfp_kern);
  printk(KERN_INFO "checking range for address allocated with GFP_ATOMIC:\n");
  print_virtual_reg(gfp_atomic);

  printk(KERN_INFO "checking range for address allocated with GFP_DMA:\n");
  print_virtual_reg(gfp_dma);
  printk(KERN_INFO "checking range for address allocated with GFP_DMA32:\n");
  print_virtual_reg(gfp_dma32);
  printk(KERN_INFO "checking ange for address allocated with GFP_NOIO:\n");
  print_virtual_reg(gfp_noio);
  printk(KERN_INFO "checking ange for address allocated with GFP_USER:\n");
  print_virtual_reg(gfp_user);
  printk(KERN_INFO "checking ange for address allocated with GFP_HIGHUSER:\n");
  print_virtual_reg(gfp_highuser);
  printk(KERN_INFO "checking ange for address allocated with vmalloc:\n");
  print_virtual_reg(vmalloced);

  printk(KERN_INFO "checking ange for address phys_to_virt(0):\n");
  print_virtual_reg((void*)p);
  
  return 0;
}



static const struct file_operations mycdrv_fops = {
  .owner = THIS_MODULE,
  .read = mycdrv_generic_read,
  .write = mycdrv_generic_write,
};

static int __init my_printing_init(void)
{
  ramdisk1 = kmalloc(ramdisk_size, GFP_KERNEL);
  if (misc_register(&my_misc_device)) {
    pr_err("Couldn't register device misc, "
	   "%d.\n", my_misc_device.minor);
    kfree(ramdisk1);
    return -EBUSY;
  }
  my_dev = my_misc_device.this_device;

  printing_add_zone();

  dev_info(my_dev, "Succeeded in registering device %s\n", MYDEV_NAME);
  return 0;
}


module_init(my_printing_init);
module_exit(my_generic_exit);
MODULE_AUTHOR("Dafna");
MODULE_DESCRIPTION("bla bla");
MODULE_LICENSE("GPL");