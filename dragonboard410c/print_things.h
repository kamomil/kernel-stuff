#include <linux/ioctl.h>
#include <asm/io.h>
#include <linux/memblock.h>
#include <asm/fixmap.h>
#include <asm/memory.h>

//#include <linux/mm.h>
//#include <linux/bootmem.h>

//#include <asm-arm64/page.h>
//#include <asm/pgtable_types.h>
//#include <asm/pgtable.h>

/*

vmalloc : 0xffffff8000000000 - 0xffffffbdbfff0000   (   246 )
vmemmap : 0xffffffbdc0000000 - 0xffffffbfc0000000   (     8 )
          0xffffffbdc0000000 - 0xffffffbdc1000000   (    16 )
fixed   : 0xffffffbffa7fd000 - 0xffffffbffac00000   (  4108 )
PCI I/O : 0xffffffbffae00000 - 0xffffffbffbe00000   (    16 )
modules : 0xffffffbffc000000 - 0xffffffc000000000   (    64 )
memory  : 0xffffffc000000000 - 0xffffffc040000000   (  1024 )
.init : 0xffffffc000e49000 - 0xffffffc000f43000   (  1000 )
.text : 0xffffffc000080000 - 0xffffffc000e483e4   ( 14113 )

 */
/*
		  MLG(VMEMMAP_START,
		      VMEMMAP_START + VMEMMAP_SIZE),
		  MLM((unsigned long)virt_to_page(PAGE_OFFSET),
		      (unsigned long)virt_to_page(high_memory)),

 */
int print_all_kinds(void){
  //phys_addr_t addr1;
  //phys_addr_t addr2;
  
  printk( KERN_INFO "PAGE_OFFSET = 0x%lx\n",PAGE_OFFSET);//#define __phys_to_virt(x)       ((unsigned long)((x) - PHYS_OFFSET) | PAGE_OFFSET)
  printk( KERN_INFO "PAGE_SHIFT = %d\n",PAGE_SHIFT);
  printk( KERN_INFO "high_memory = 0x%p\n",high_memory);
  printk( KERN_INFO "(unsigned long)virt_to_page(PAGE_OFFSET) = 0x%lx\n",(unsigned long)virt_to_page(PAGE_OFFSET));
  printk( KERN_INFO "(unsigned long)virt_to_page(high_memory) = 0x%lx\n",(unsigned long)virt_to_page(high_memory));
  printk( KERN_INFO "PHYS_OFFSET = 0x%llx\n",PHYS_OFFSET);
  //printk( KERN_INFO "TEXT_OFFSET = 0x%llx\n",TEXT_OFFSET); //undeclared
  
  //printk( KERN_INFO "kimage_vaddr = 0x%lx\n",kimage_vaddr);
  //  addr2 = get_allocated_memblock_reserved_regions_info(&addr1);
  //printk( KERN_INFO "get_allocated_memblock_reserved_regions_info = 0x%llx, 0x%llx",addr1,addr2);
  return 0;
}
int print_mem_regs(void){

  unsigned long long pfn = 
  printk( KERN_INFO "vmalloc: starts: 0x%lx ends 0x%lx\n",VMALLOC_START,VMALLOC_END);
  printk( KERN_INFO "vmemmap: starts: 0x%lx ends 0x%lx\n",VMEMMAP_START,VMEMMAP_START+VMEMMAP_SIZE);
  printk( KERN_INFO "fixed:   starts: 0x%lx ends 0x%lx\n",FIXADDR_START,FIXADDR_TOP);
  printk( KERN_INFO "PCI I/O: starts: 0x%lx ends 0x%lx\n",PCI_IO_START, PCI_IO_END);
  printk( KERN_INFO "modules: starts: 0x%lx ends 0x%lx\n",MODULES_VADDR, MODULES_END);
  printk( KERN_INFO "memory:  starts: 0x%lx ends 0x%lx\n",PAGE_OFFSET,(unsigned long)high_memory);

  printk( KERN_INFO "physical range corresponding to memory:  starts: 0x%llx ends 0x%llx\n",virt_to_phys(PAGE_OFFSET),virt_to_phys(high_memory));

  printk( KERN_INFO "__pa(PAGE_OFFSET) = 0x%llx\n",__pa(PAGE_OFFSET));
  printk( KERN_INFO "__pa(PAGE_OFFSET) >> PAGE_SHIFT = 0x%llx\n",__pa(PAGE_OFFSET) >> PAGE_SHIFT);
  printk( KERN_INFO "pfn_to_page(__pa(PAGE_OFFSET) >> PAGE_SHIFT) = 0x%lx\n",pfn_to_page(__pa(PAGE_OFFSET) >> PAGE_SHIFT));
  printk( KERN_INFO "virt_to_page(PAGE_OFFSET) = %p\n",virt_to_page(PAGE_OFFSET));

  /* #define __pfn_to_page(pfn)      (vmemmap + (pfn)) */
  printk( KERN_INFO "vmemmap = 0x%llx, vmemmap+(__pa(PAGE_OFFSET) >> PAGE_SHIFT) = 0x%llx\n",vmemmap,(vmemmap+(__pa(PAGE_OFFSET) >> PAGE_SHIFT)));
  printk( KERN_INFO "sizeof(struct page) = 0x%lx",sizeof(struct page));
  
  printk(KERN_INFO "\n");
  return 0;
}

int print_virtual_reg(void* p){

  printk( KERN_INFO "p is %p \n",p);

  if(p>=VMALLOC_START && p< VMALLOC_END){
    printk( KERN_INFO "p is in the vmalloc region, starts: 0x%lx ends 0x%lx\n",VMALLOC_START,VMALLOC_END);
  }
  if(p >= VMEMMAP_START && p < VMEMMAP_START + VMEMMAP_SIZE){
    printk( KERN_INFO "p is in the vmemmap region, starts: 0x%lx ends 0x%lx\n",VMEMMAP_START,VMEMMAP_START+VMEMMAP_SIZE);
  }
  if(p >= FIXADDR_START && p < FIXADDR_TOP){
    printk( KERN_INFO "p is in the fixed region, starts: 0x%lx ends 0x%lx\n",FIXADDR_START,FIXADDR_TOP);
  }

  if(p >= PCI_IO_START && p < PCI_IO_END){
    printk( KERN_INFO "p is in the PCI I/O region, starts: 0x%lx ends 0x%lx\n",PCI_IO_START, PCI_IO_END);
  }

  if(p >= MODULES_VADDR && p < MODULES_END){
    printk( KERN_INFO "p is in the modules region, starts: 0x%lx ends 0x%lx\n",MODULES_VADDR, MODULES_END);
  }

  if(p >= PAGE_OFFSET && p < (unsigned long)high_memory){
    printk( KERN_INFO "p is in the memory, starts: 0x%lx ends 0x%lx\n",PAGE_OFFSET,(unsigned long)high_memory);
  }
  printk(KERN_INFO "\n");
  return 0;
}
