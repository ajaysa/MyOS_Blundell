mov al, 'A'
call my_print_funct

;jmp my_print_funct
;return_here:
jmp $

my_print_funct:
mov ah, 0x0e
int 0x10
;jmp return_here
ret

times 510-($-$$) db 0

dw 0xaa55
