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

    ; Clear registers eax and ebx.
    xor eax, eax
    xor ebx, ebx

; Do for each character from the plain text and its corresponding
; character from the key.
otp_cipher:
    ; Move a character from the plaintext to al and its corresponding character
    ; from the key to bl.
    mov al, byte[esi + ecx - 1]
    mov bl, byte[edi + ecx - 1]
    
    ; XOR the two characters.
    xor al, bl

    ; Move the resulted character from al to its corresponding position in 
    ; ciphertext(edx + ecx - 1).
    mov byte [edx + ecx - 1], al
    
    loop otp_cipher


    ;; DO NOT MODIFY
    popa
    leave
    ret
    ;; DO NOT MODIFY