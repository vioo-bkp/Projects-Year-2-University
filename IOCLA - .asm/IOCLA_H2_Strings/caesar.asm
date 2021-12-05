%include "io.mac"

section .text
    global caesar
    extern printf

caesar:
    ;; DO NOT MODIFY
    push    ebp
    mov     ebp, esp
    pusha

    mov     edx, [ebp + 8]      ; ciphertext
    mov     esi, [ebp + 12]     ; plaintext
    mov     edi, [ebp + 16]     ; key
    mov     ecx, [ebp + 20]     ; length
    ;; DO NOT MODIFY
    
    ; Clear register ebx.
    xor ebx, ebx

; Do for each character from the plain text.
caesar_cipher:
    ; Move a character from the plaintext to al.
    xor eax, eax
    mov al, byte [esi + ecx - 1]

    ; Check if the character is uppercase, lowercase or neither.
    ; If it's lowercase or uppercase => It has to be encrypted.
    ; Else leave it alone.
    
    cmp al, 'A'
    jl next_char

    cmp al, 'Z'
    jle uppercase

    cmp al, 'a'
    jl next_char

    cmp al, 'z'
    jg next_char


; If the character is lowercase/uppercase we'll keep 'a'/'A' in bl and 
; we'll use it later in the encryption process as a base value.
lowercase:
    mov bl, 'a'
    jmp encrypt

uppercase:
    mov bl, 'A'

; The encryption formula: (reduced_character + key_value) mod 26.
; Considering 'a', 'A' = 0; 'b', 'B' = 1 etc.
; Encrypt the letter.
encrypt:
    ; Reduce the character's value to the range [0, 25] and add the key to it.
    sub al, bl
    add eax, edi
    
    ; Keep ebx's value(base value) for later and clear the register. 
    push ebx
    xor ebx, ebx

    ; Move 26 to ebx and do the encryption using the formula.
    mov bl, 'z' - 'a' + 1
    div bl
    mov bh, ah
    
    ; Move the value we pushed on stack earlier('A'/'a' for uppercase/lowercase)
    ; to eax and add the remainder from the divion we did above to it.
    xor eax, eax
    pop eax
    add al, bh
    
; Move the character from al to its corresponding position in ciphertext 
; (edx + ecx - 1).
next_char:
    mov byte [edx + ecx - 1], al
    
    loop caesar_cipher

    ;; DO NOT MODIFY
    popa
    leave
    ret
    ;; DO NOT MODIFY