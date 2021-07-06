%include "io.mac"
section .text
    global otp
    extern printf

otp:
    ;; DO NOT MODIFY
    push    ebp
    mov     ebp, esp
    pusha

    mov     edx, [ebp + 8]      ; ciphertext
    mov     esi, [ebp + 12]     ; plaintext
    mov     edi, [ebp + 16]     ; key
    mov     ecx, [ebp + 20]     ; length
    ;; DO NOT MODIFY

    ;; TODO: Implement the One Time Pad cipher
    dec ecx
_loop:
    mov ah, [edi +ecx]  ;folosesc ah si al pentru ca sunt
    mov ah, [esi +ecx]  ;primii 2 bytes respectiv 1 byte din eax
    mov al, [edi +ecx]
    xor ah, al
    mov [edx +ecx], ah
    dec ecx             ; il decrementez pe ecx cu fiecare loop, 
    cmp ecx, 0          ; deoarece iesea cand ecx devenea 0
    jnl _loop

    ;; DO NOT MODIFY
    popa
    leave
    ret
    ;; DO NOT MODIFY