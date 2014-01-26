print_string:
pusha
mov ah, 0x0e
int 0x10
popa
ret
