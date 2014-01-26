print_hex:
	mov ax, dx
	mov bx, HEX_OUT
	add bx, 2
	mov cl, 12

	loopover:
		mov ax, dx
		shr ax, cl
		and ax, 0x000f

		cmp ax, 10
		jl less_then_ten

		cmp ax, 10
		je TEN
		cmp ax, 11
		je ELEVEN
		cmp ax, 12
		je TWELVE
		cmp ax, 13
		je THIRTEEN
		cmp ax, 14
		je FOURTEEN
		cmp ax, 15
		je FIFTEEN

		TEN:
			mov [bx], byte 'a'
			jmp adjust
		ELEVEN:
			mov [bx], byte 'b'
			jmp adjust
		TWELVE:
			mov [bx], byte 'c'
			jmp adjust
		THIRTEEN:
			mov [bx], byte 'd'
			jmp adjust
		FOURTEEN:
			mov [bx], byte 'e'
			jmp adjust
		FIFTEEN:
			mov [bx], byte 'f'
			jmp adjust

		less_then_ten:
			add al, 48
			mov [bx], al

		adjust:
		add bx, 1
		sub cl, 4
		cmp cl, 0
		jge loopover
	
	mov bx, HEX_OUT
	call print_string
	ret

HEX_OUT:
	db '0x0000', 0
