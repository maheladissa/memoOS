#include "Input_Output/framebuffer.h"
#include "Input_Output/serial.h"
#include "Segments/gdt.h"
#include "Input_Output/keyboard.h"
#include "Interupts/interrupt_handler.h"
#include "multiboot.h"

/* Function to initialize */
void init() {
  /* Initialize segment descriptor tables */
  init_gdt();

  /* Initialize IDT */
  init_idt();

  /* Initialize serial port */
  serial_configure(SERIAL_COM1_BASE, 1);
 
}

int kmain(unsigned int ebx)
{

	init();
	
	
  	multiboot_info_t *mbinfo = (multiboot_info_t *) ebx;
  	multiboot_module_t* modules = (multiboot_module_t*) mbinfo->mods_addr; 
  	unsigned int address_of_module = modules->mod_start;
  	
  	
  	if((mbinfo->mods_count) == 1){
  		char buffer[] = "Module loaded successfully!";
  		serial_write(SERIAL_COM1_BASE,buffer,sizeof(buffer));
  		
  		typedef void (*call_module_t)(void);
        	/* ... */
        	call_module_t start_program = (call_module_t) address_of_module;
        	start_program();
        	/* we'll never get here, unless the module code returns */

  	}else{
  		char buffer[] = "Module Loading Error!";
  		serial_write(SERIAL_COM1_BASE,buffer,sizeof(buffer));
  	}
  	return 0;
    
}
