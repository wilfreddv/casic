section .data
    SYS_WRITE: db 4
    SYS_STDIO: db 1


section .text
    global _start
    global print
    extern main

_start:
    call main
    mov rbx, rax; retval from `main`
    mov rax, 1  ; syscall exit
    int 0x80


print:
    call __strlen
    mov rdx, rax    ; move string length into proper register
    mov al, [SYS_WRITE]
    mov bl, [SYS_STDIO]
    mov rcx, rdi    ; pointer to string
    int 0x80        ; syscall
    ret


__strlen:
    xor rax, rax        ; counter, also return value
__strlen_L1:
    mov bl, [rax+rdi]   ; fetch character
    cmp bl, 0x0         ; test for end of string
    je __strlen_END
    inc rax             ; next character
    jmp __strlen_L1
__strlen_END:
    ret
