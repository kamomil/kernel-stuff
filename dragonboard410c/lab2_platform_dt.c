#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/device.h>
#include <linux/of.h>
#include "lab_miscdev.h"
#include <linux/interrupt.h>
#include <linux/of_address.h>
#include <linux/mm.h>
#include <linux/pagemap.h>
#include <linux/vmalloc.h>
#include "print_things.h"

#define TX_IDX 0
#define RX_IDX 1


struct resource* resources [2];
const char* resources_types[2]  = {"tx","rx"};
const char* resources_types_dev[2]  = {"lab2_interuppts_tx","lab2_interuppts_rx"};


int mycdrv_mymmap (struct file *file, struct vm_area_struct *vma)
{
  //int remap_pfn_range(struct vm_area_struct *, unsigned long addr,unsigned long pfn, unsigned long size, pgprot_t);
  unsigned long mmap_size;
  phys_addr_t ramdisk =  0x0000000090000000;
  //unsigned long  range_size = 300000;

  if(!file && !vma){
    dev_info(my_dev,"MMAP internaly called\n");
    return 0;
  }    
  mmap_size = vma->vm_end - vma->vm_start;

  print_virtual_reg(__va(ramdisk));

  dev_info(my_dev,"MMAP: PAGE_SIZE %lu PAGE_SHIFT %d ramdisk (virtual) = %p phisical (0x%lx), mmap_size = %ld\n", PAGE_SIZE,PAGE_SHIFT, __va(ramdisk),ramdisk, mmap_size);
  
  dev_info(my_dev,"MMAP: before: vma = %p, vma->vm_start = %p, __pa(ramdisk) = 0x%llx, mmap_size = %ld, vma->vm_page_prot = 0x%llx\n",vma, (void *)vma->vm_start, __pa(ramdisk), mmap_size, vma->vm_page_prot);

 
  if(remap_pfn_range(vma, vma->vm_start, ramdisk >> PAGE_SHIFT , mmap_size, vma->vm_page_prot)){
    dev_warn(my_dev,"remap_pfn_range failed\n");
    return -EAGAIN;
  }
  vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);

  dev_info(my_dev,"MMAP: after: vma = %p, vma->vm_start = %p,  vma->vm_page_prot = 0x%llx\n",vma, (void *)vma->vm_start, vma->vm_page_prot);
    
  return 0;
}

static void reserved_mem_range(void){

  struct device_node *parent, *node;
    
  parent = of_find_node_by_path("/reserved-memory");
  if (!parent)
    return;
    
  for_each_child_of_node(parent, node) {
    uint64_t range_addr, range_size, range_end;
    const __be32 *addrp;
    const char *label;
    
    addrp = of_get_address(node, 0, &range_size, NULL);
    
    range_addr = of_read_number(addrp, 2);
    range_end = range_addr + range_size;
    
    label = of_get_property(node, "rmtfs", NULL);

    pr_info("\nreserved_mem_range: addrp = %p\n",addrp);
    pr_info("reserved_mem_range: range_addr = %p\n",(void*)range_addr);
    pr_info("reserved_mem_range: range_addr = %llx\n",range_addr);

    pr_info("reserved_mem_range: range_size = %p\n",(void*)range_size);
    pr_info("reserved_mem_range: range_size = %llx\n",range_size);
    
    /* PRD ranges need a label */
    if (!label)
      continue;

    return;
  }
}

static int my_probe(struct platform_device *pdev)
{
  struct device_node* dn;
  struct  property *p = NULL;
  void *virt_addr_ptr = NULL;
  struct page **my_page = NULL;
  unsigned long phys_addr_ptr =  0x0000000090000000; //0x0000000086000000; 
  
  pr_info("MY_PROBE: pdev = %p\n",pdev);
  memset(resources,0,sizeof(resources));

  /* platform device is defined in linux/platform_device.h */
  /* struct device_node is defined in include/linux/of.h */
  /* struct device  is defined in include/device.h*/

   pr_info("MY_PROBE: pdev->name = %s\n",pdev->name);
   pr_info("MY_PROBE: pdev->id = %d\n",pdev->id);
   pr_info("MY_PROBE: pdev->id_auto = %d\n",pdev->id_auto);

   pr_info("MY_PROBE: pdev->num_resources = %d\n",pdev->num_resources);
   pr_info("MY_PROBE: pdev->*driver_override = %s\n",pdev->driver_override); /* Driver name to force a match */

   dn = pdev->dev.of_node;
   pr_info("MY_PROBE: pdev->dev.of_node = %p\n",dn); 

   pr_info("MY_PROBE: dn->name = %p\n",dn->name);
   pr_info("MY_PROBE: dn->type = %p\n",dn->type);

   for(p = dn->properties;p != NULL;p = p->next){
     pr_info("/nMY_PROBE: p->name = %s\n",p->name);
     pr_info("MY_PROBE: p->length = %d\n",p->length);
     pr_info("MY_PROBE: p->value = %p\n",p->value);
     pr_info("MY_PROBE: p->value = %s\n",(char*)(p->value));
     pr_info("MY_PROBE: p->_flags = %lx\n",p->_flags);
     pr_info("MY_PROBE: p->unique_id = %x\n",p->unique_id);
	       
   }
   reserved_mem_range();

   //my_page = (struct page**)kmalloc(sizeof (*my_page), GFP_KERNEL);
   //*my_page = phys_to_page(phys_addr_ptr);
   //virt_addr_ptr = vmap(my_page, 1, VM_MAP, PAGE_KERNEL);
   //pr_info("/nMY_PROBE: p->unique_id = %x\n",p->unique_id);
   
   /* 26         struct device   dev;*/
   /* 28         struct resource *resource;*/
   
   /* 30         const struct platform_device_id *id_entry;*/
   
   /* MFD cell pointer */
   /*        struct mfd_cell *mfd_cell;*/
   
   /* arch specific additions */
   /*         struct pdev_archdata    archdata;*/
   
  
  return 0;
}

static int my_remove(struct platform_device *pdev)
{
  
  pr_info("MY_REMOVE: pdev = %p",pdev);
  return 0;
}


static const struct of_device_id lab2_of_match[] = {
	{ .compatible = "stam,devdev" },
	{}
};
MODULE_DEVICE_TABLE(of, lab2_of_match);

static struct platform_driver dafna_driver = {
  //struct device_driver driver;
  .driver= {
    .name = "lab2_platform_dt",//any device identifying itself as "lab2_platform_dt" will be bound to this driver
    
    .of_match_table = lab2_of_match,
  },
  .probe = my_probe,
  .remove = my_remove,
  //.id_table = pwm_id_table,
};

/*
tx irq: 208 
rx irq: 209
 */
static int __init interrupt_init(void)
{
  int err = platform_driver_register(&dafna_driver);
  if (err){
    pr_err("lab2 in init: platform_driver_register failed\n");
    return err;
  }
  
  ramdisk1 = kmalloc(ramdisk_size, GFP_KERNEL);
  if(!ramdisk1){
      pr_err("lab2 interupts (in init): could not allocate ramdisk\n");
      return -1;
  }
  
  if (misc_register(&my_misc_device)) {
    pr_err("Couldn't register device misc, "
	   "%d.\n", my_misc_device.minor);
    kfree(ramdisk1);
    platform_driver_unregister(&dafna_driver);
    return -EBUSY;
  }
  my_dev = my_misc_device.this_device;
  dev_info(my_dev, "interrupt_init: Succeeded in registering device %s\n", MYDEV_NAME);

  return 0;
}

static void __exit  interrupt_exit(void)
{
  dev_info(my_dev, "Unregistering Device\n");
  platform_driver_unregister(&dafna_driver);
  misc_deregister(&my_misc_device);
  kfree(ramdisk1);
}

static const struct file_operations mycdrv_fops = {
  .owner = THIS_MODULE,
  .read = mycdrv_generic_read,
  .write = mycdrv_generic_write,
  .open = mycdrv_generic_open,
  .release = mycdrv_generic_release,
    .mmap = mycdrv_mymmap
};

module_init(interrupt_init);
module_exit(interrupt_exit);

MODULE_AUTHOR("Dafna");
MODULE_DESCRIPTION("dummy stam");
MODULE_LICENSE("GPL v2");
//MODULE_ALIAS("of:N*T*Cqcom,wcnss-wlan*");
