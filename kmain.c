#include "Headers/framebuffer.h"
#include "Headers/serial.h"
#include "Segments/gdt.h"

/* Function to initialize */
void init() {
  /* Initialize segment descriptor tables */
  init_gdt();

  /* Initialize serial port */
  serial_configure(SERIAL_COM1_BASE, 1);
}

void kmain()
{

    init();
    //fb_write_cell(0, 'A', FB_LIGHT_CYAN, FB_BLACK);
    //fb_move_cursor(00);
    char buffer[25] = "--- Welcome to MemoOS ---";
    fb_write(buffer, 26);
    serial_write(SERIAL_COM1_BASE, buffer, 25);
    
}
