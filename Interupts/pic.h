#ifndef INCLUDE_PIC
#define INCLUDE_PIC


#include "../Input_Output/io.h"

#define PIC1_PORT_A 0x20
#define PIC2_PORT_A 0xA0

/* The PIC interrupts have been remapped */
#define PIC1_START_INTERRUPT 0x20
#define PIC2_START_INTERRUPT 0x28
#define PIC2_END_INTERRUPT   PIC2_START_INTERRUPT + 7

#define PIC_ACK     0x20

/*                      I/O port */
#define PIC_1		0x20		/* IO base address for master PIC */
#define PIC_2		0xA0		/* IO base address for slave PIC */
#define PIC_1_COMMAND	PIC_1
#define PIC_1_DATA	(PIC_1+1)
#define PIC_2_COMMAND	PIC_2
#define PIC_2_DATA	(PIC_2+1)

#define PIC_1_OFFSET 0x20
#define PIC_2_OFFSET 0x28
#define PIC_2_END PIC_2_OFFSET + 7

#define PIC_1_COMMAND_PORT 0x20
#define PIC_2_COMMAND_PORT 0xA0
#define PIC_ACKNOWLEDGE 0x20

#define PIC_ICW1_ICW4            0x01	/* ICW4 (not) needed */
#define PIC_ICW1_SINGLE          0x02	/* Single (cascade) mode */
#define PIC_ICW1_INTERVAL4       0x04	/* Call address interval 4 (8) */
#define PIC_ICW1_LEVEL           0x08	/* Level triggered (edge) mode */
#define PIC_ICW1_INIT            0x10	/* Initialization - required! */

#define PIC_ICW4_8086            0x01	/* 8086/88 (MCS-80/85) mode */
#define PIC_ICW4_AUTO            0x02	/* Auto (normal) EOI */
#define PIC_ICW4_BUF_SLAVE       0x08	/* Buffered mode/slave */
#define PIC_ICW4_BUF_MASTER      0x0C	/* Buffered mode/master */
#define PIC_ICW4_SFNM            0x10	/* Special fully nested (not) */

void pic_remap(int offset1, int offset2);
void pic_acknowledge(unsigned int interrupt);

/** pic_acknowledge:
 *  Acknowledges an interrupt from either PIC 1 or PIC 2.
 *
 *  @param num The number of the interrupt
 */
void pic_acknowledge(unsigned int interrupt)
{
    if (interrupt < PIC1_START_INTERRUPT || interrupt > PIC2_END_INTERRUPT) {
      return;
    }

    if (interrupt < PIC2_START_INTERRUPT) {
      outb(PIC1_PORT_A, PIC_ACK);
    } else {
      outb(PIC2_PORT_A, PIC_ACK);
    }
}


void pic_remap(int offset1, int offset2)
{
	outb(PIC_1_COMMAND, PIC_ICW1_INIT + PIC_ICW1_ICW4);	// starts the initialization sequence (in cascade mode)
	outb(PIC_2_COMMAND, PIC_ICW1_INIT + PIC_ICW1_ICW4);
	outb(PIC_1_DATA, offset1);				// ICW2: Master PIC vector offset
	outb(PIC_2_DATA, offset2);				// ICW2: Slave PIC vector offset
	outb(PIC_1_DATA, 4);					// ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	outb(PIC_2_DATA, 2);					// ICW3: tell Slave PIC its cascade identity (0000 0010)

	outb(PIC_1_DATA, PIC_ICW4_8086);
	outb(PIC_2_DATA, PIC_ICW4_8086);

        // Setup Interrupt Mask Register (IMR)
	outb(PIC_1_DATA, 0xFD); // 1111 1101 - Enable IRQ 1 only (keyboard).
	outb(PIC_2_DATA, 0xFF);

	asm("sti"); // Enable interrupts.
}

#endif
