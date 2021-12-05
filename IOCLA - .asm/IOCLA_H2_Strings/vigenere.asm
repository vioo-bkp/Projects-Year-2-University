%include "io.mac"

section .data
    key_len dd 0
    plaintext_len dd 0
    temp db 0

section .text
    global vigenere
    extern printf

vigenere:
    ;; DO NOT MODIFY
    push    ebp
    mov     ebp, esp
    pusha

    mov     edx, [ebp + 8]      ; ciphertext
    mov     esi, [ebp + 12]     ; plaintext
    mov     ecx, [ebp + 16]     ; plaintext_len
    mov     edi, [ebp + 20]     ; key
    mov     ebx, [ebp + 24]     ; key_len
    ;; DO NOT MODIFY

    ; Keep lengths of plain text and key for later.
    mov dword[key_len], ebx
    mov dword[plaintext_len], ecx

; Reduce the key's characters values to the range [0, 25] by subtracting 'A'
; from each one.
convert_key:
    sub byte [edi + ebx - 1], 'A'
    dec ebx
    cmp ebx, 0x0
    jne convert_key
    
    ; Clear the registers.
    xor ecx, ecx
    xor ebx, ebx

; Do for each character from the plain text.
vigenere_cipher:
    xor eax, eax

    ; Move a character from the plaintext to al.
    mov al, byte [esi + ecx]

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

; If the character is lowercase/uppercase we'll push 'a'/'A' to stack and
; we'll use it later in the encryption process as a base value.
lowercase:
    sub al, 'a'
    push 'a'
    jmp encrypt

uppercase:
    sub al, 'A'
    push 'A'

; The encryption formula: (reduced_character + reduced_character_in_key) mod 26.
; Considering 'a', 'A' = 0; 'b', 'B' = 1 etc.
; Encrypt the letter.
encrypt:
    ; Add to the character from al its corresponding character from the key.
    add al, byte [edi + ebx]

    ; Move 26 to ebx and do the encryption using the formula.
    mov byte [temp], 'z' - 'a' + 1
    div byte [temp]
    mov [temp], ah
    
    ; Move the value we pushed on stack earlier, the base value
    ; ('A'/'a' for uppercase/lowercase) to eax and add the remainder from the
    ; divion we did above to it.
    xor eax, eax
    pop eax
    add al, [temp]
    
    ; Increment the character index for key.
    inc ebx
    jmp next_char

; Set the character index for key to 0.
reset_key:
    xor ebx, ebx

next_char:
    ; Check if the character index for key is bigger than key's length and 
    ; reset it if necessary.
    cmp dword[key_len], ebx
    je reset_key

    ; Move the character from al to its corresponding position in ciphertext
    ; (edx + ecx).    
    mov byte [edx + ecx], al
    inc ecx

    ; Check if the character index for plain text is bigger than text's length.
    cmp dword[plaintext_len], ecx
    jne vigenere_cipher

    ;; DO NOT MODIFY
    popa
    leave
    ret
    ;; DO NOT MODIFY