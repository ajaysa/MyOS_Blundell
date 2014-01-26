[bits 32]

VIDEO_MEMORY equ 0xb8000
WHITE_ON_BLACK equ 0x0f

; data in ebx register
print_string_pm:
	pusha
	mov edx, VIDEO_MEMORY

	print_string_pm_loop:
		mov al, [ebx]
		mov ah, WHITE_ON_BLACK

		cmp al, 0
		je done

		mov [edx], ax

		add edx, 2
		add ebx, 1
		jmp print_string_pm_loop

	done:
		popa
		ret
