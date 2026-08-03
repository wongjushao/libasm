default rel

section .text
    global ft_strcmp

ft_strcmp:

    xor rcx, rcx

.loop:
    movzx eax, byte [rdi + rcx]
    movzx edx, byte [rsi + rcx]

    cmp eax, edx
    jne .done

    test eax, eax
    je .done

    inc rcx
    jmp .loop

.done:
    sub eax, edx
    ret

section .note.GNU-stack noalloc noexec nowrite progbits