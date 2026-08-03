default rel

section .text
    global ft_strcpy

ft_strcpy:
	mov rax, rdi
    xor rcx, rcx

    test rdi, rdi
    jz .return_null
    test rsi, rsi
    jz .return_null

.loop:

	mov dl, byte [rsi, rcx]
    mov byte [rdi, rcx], dl

    cmp dl, 0
    je .done

    inc rcx
	jmp .loop

.return_null:
    xor rax, rax

.done:
	ret

section .note.GNU-stack noalloc noexec nowrite progbits
