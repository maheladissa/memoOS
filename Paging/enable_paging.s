lobal load_page_directory
global enable_paging
global boot_page_directory

PAGING_PRESENT equ 1b
PAGING_WRITABLE equ 10b
PAGING_USER_ACCESSIBLE equ 100b
PAGING_SIZE_4MB equ 10000000b
; identity map 0x00000000 - 0x00400000 (first 4MB) which includes kernel
; and paging data structures

section .data
align 4096
boot_page_directory:
  pde_frame_addr equ 0x0
  dd (pde_frame_addr & 0xfff00000) + (PAGING_PRESENT | PAGING_WRITABLE | PAGING_SIZE_4MB)
  times 0x3ff dd 0       ; allocate remaining page directory entries

; align 4096
; boot_page_table:
; %assign frame_addr 0
; %rep 0x300
;   dd frame_addr | (PAGING_PRESENT | PAGING_WRITABLE | PAGING_USER_ACCESSIBLE)
;   %assign frame_addr frame_addr+0x1000
; %endrep
;   times 0x100 dd 0

section .text 
.globl loadPageDirectory
loadPageDirectory:	; put &boot_page_directory in high 20 bits of cr3 register
	push %ebp
	mov %esp, %ebp
	mov 8(%esp), %eax
	mov %eax, %cr3
	mov %ebp, %esp
	pop %ebp
	ret

.globl enablePaging
enablePaging:
	push %ebp
	mov %esp, %ebp
	mov %cr0, %eax
	or $0x80000000, %eax
	mov %eax, %cr0
	mov %ebp, %esp
	pop %ebp
	ret
