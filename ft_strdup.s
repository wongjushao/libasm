default rel
extern ft_strlen, ft_strcpy, malloc

section .text
    global ft_strdup

ft_strdup:
    test rdi, rdi
    jz .return_null

    push rdi
    call ft_strlen

    inc rax
    mov rdi, rax
    call malloc wrt ..plt

    mov rdi, rax
    pop rsi
    call ft_strcpy

    ret

.return_null:
    xor rax, rax
    ret

section .note.GNU-stack noalloc noexec nowrite progbits