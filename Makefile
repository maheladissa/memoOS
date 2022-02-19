    OBJECTS = loader.o kmain.o Input_Output/io.o Segments/gdt.o Interupts/interrupt_handler.o Interupts/idt.o Paging/enable_paging.o usermode.o Interupts/hardware_intrupt_handler.o start.o userprograme.c
    CC = gcc
    CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
             -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -c
    CUSEFLAGS= -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs
    LDFLAGS = -T link.ld -melf_i386
    AS = nasm
    ASFLAGS = -f elf

    all: kernel.elf

    kernel.elf: $(OBJECTS)
	ld $(LDFLAGS) $(OBJECTS) -o kernel.elf

    userprograme.o: userprograme.c
	$(GCC) $(CUSEFLAGS) $< -o $@	

    userprograme.bin: userprograme.o start.o
	$(LD) -T usermode_linker.ld -melf_i386 $^ -o $@

    os.iso: kernel.elf userprograme.bin menu.lst
	cp kernel.elf iso/boot/kernel.elf
	cp userprograme.bin iso/modules
	genisoimage -R                              \
                    -b boot/grub/stage2_eltorito    \
                    -no-emul-boot                   \
                    -boot-load-size 4               \
                    -A os                           \
                    -input-charset utf8             \
                    -quiet                          \
                    -boot-info-table                \
                    -o os.iso                       \
                    iso

    run: os.iso
	bochs -f bochsrc.txt -q

    %.o: %.c
	$(CC) $(CFLAGS)  $< -o $@

    %.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

    clean:
	rm -rf *.o kernel.elf os.iso
