    section .text
        ; push argv
        ; push argc
        extern main
        call main
        ; main has returned, eax is return value
        jmp  $    ; loop forever
