sys_exit:       equ		60

	        section         .text
		global          _start

buf_size:       equ		8192
_start:
		xor		rbx, rbx
		sub		rsp, buf_size
		mov		rsi, rsp
		xor		r9, r9
		inc		r9

read_again:
		xor		rax, rax
		xor		rdi, rdi
		mov		rdx, buf_size
		syscall

		test		rax, rax
		jz		quit
		js		read_error

		xor		rcx, rcx

check_char:
		cmp		rcx, rax
		je		read_again

		cmp		byte [rsi + rcx], 0x20
		je		is_true

		cmp		byte [rsi + rcx], 0x09
		jb		is_false

		cmp		byte [rsi + rcx], 0x0d
		jna		is_true

is_false:
		cmp		r9, 0
		je		skip
		inc		rbx
		xor		r9, r9
		jmp		skip

is_true:
		inc		r9

skip:
		inc		rcx
		jmp		check_char

	        jmp		quit
quit:
		mov		rax, rbx
		call		print_int

		mov		rax, sys_exit
		xor		rdi, rdi
		syscall

print_int:
		mov		rsi, rsp
		mov		rbx, 10

		dec		rsi
		mov		byte [rsi], 0x0a

next_char:
		xor		rdx, rdx
		div		rbx
		add		dl, '0'
		dec		rsi
		mov		[rsi], dl
		test		rax, rax
		jnz		next_char

		mov		rax, 1
		mov		rdi, 1
		mov		rdx, rsp
		sub		rdx, rsi
		syscall

		ret

read_error:
		mov		rax, 1
		mov		rdi, 2
		mov		rsi, read_error_msg
		mov		rdx, read_error_len
		syscall

		mov		rax, sys_exit
		mov		rdi, 1
		syscall

		section         .rodata

read_error_msg: db		 "read failure", 0x0a
read_error_len: equ		$ - read_error_msg
