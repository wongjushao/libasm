default rel
extern __errno_location

section .text
    global ft_write

ft_write:
    mov rax, 1
    syscall
    cmp rax, 0
    jge .done

    neg rax
    mov rdi, rax
    call __errno_location wrt ..plt
    mov [rax], edi
    mov rax, -1

.done:
    ret

section .note.GNU-stack noalloc noexec nowrite progbits