#include "Input_Output/framebuffer.h"
#include "Input_Output/serial.h"
#include "Segments/gdt.h"
#include "Input_Output/keyboard.h"
#include "Interupts/interrupt_handler.h"
#include "multiboot.h"
#include "Paging/paging.h"

/* Function to initialize */
void init() {
  /* Initialize segment descriptor tables */
  init_gdt();

  /* Initialize IDT */
  init_idt();

  /* Initialize serial port */
  serial_configure(SERIAL_COM1_BASE, 1);
  
  /* Initialize paging */
  initialise_paging();
 
}

int kmain()
{

	init();
	
	char buffer[]= "Frame Allocation Complete";
	serial_write(SERIAL_COM1_BASE,buffer,sizeof(buffer));
	fb_write(buffer,sizeof(buffer));
  	
  	return 0;
    
}
