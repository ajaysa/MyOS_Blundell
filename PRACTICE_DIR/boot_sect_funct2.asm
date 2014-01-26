mov al, 'A'
call print_string
jmp $

%include "print_string.asm"

times 510-($-$$) db 0

dw 0xaa55
