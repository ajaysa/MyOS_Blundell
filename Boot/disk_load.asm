disk_load:
	push dx
	
	mov ah, 0x02	; BIOS read sector function
	mov al, dh	; READ dh sectors
	mov ch, 0x00	; select cylinder 0
	mov dh, 0x00	; select head 0
	
	mov cl, 0x02	; start reading from 2nd sector (after boot sector)

	int 0x13	; BIOS interrupt
	jc disk_error

	pop dx
	cmp dh, al
	jne disk_error

	ret
disk_error:
	mov bx, [DISK_ERROR_MSG]
	call print_string
	jmp $

DISK_ERROR_MSG:
	db 'Disk read Error!', 0
