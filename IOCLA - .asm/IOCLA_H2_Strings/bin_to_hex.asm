%include "io.mac"

section .data
    hex_index dd 0
    length dd 0

section .text
    global bin_to_hex
    extern printf

bin_to_hex:
    ;; DO NOT MODIFY
    push    ebp
    mov     ebp, esp
    pusha

    mov     edx, [ebp + 8]      ; hexa_value
    mov     esi, [ebp + 12]     ; bin_sequence
    mov     ecx, [ebp + 16]     ; length
    ;; DO NOT MODIFY

    ; The strategy is:
    ; - Take the binary sequence;
    ; - Divide the binary digits into groups of 4(starting from right to left);
    ; - Convert each group of 4 binary digits(bits) to one hexadecimal digit. 

    ; Clear registers ebx and eax.
    xor ebx, ebx
    xor eax, eax
    
    ; We'll find how many characters we'll have by grouping the bits in pairs
    ; of 4 starting from right to left.
    ; In order to do this we divide the sequence's length by 4. 
    mov eax, ecx
    mov bl, 0x4
    div bl
    
    mov byte[length], al
    
    ; Check if there were any bits that were left apart(this happens only when
    ; the sequence's length isn't a multiple of 4).
    cmp ah, 0x0
    je continue
    
    ; Increment the number of characters if there were any bits left apart.
    inc dword [length]
 
continue:
    ; Set eax to 0 and ebx to 1.
    ; ebx will hold the powers of 2 corresponding to each bit in a pair
    ; (2^0 = 1 -> 2^3 = 8).
    xor eax, eax
    xor ebx, ebx
    mov bl, 0x1
    
    ; Set hex_index to have the same value as length.
    push dword [length]
    pop dword [hex_index]

make_pair:
    ; Check if the bit is '0' or '1'.
    ; If it's '1' we use it to build the pair's value.
    cmp byte[esi + ecx - 1], '0'
    je check_pair

    ; The bit is '1' so we add ebx's value to the pair's value(eax).
    add al, bl

; Check if a pair of 4 bits was made.
check_pair:
    ; Shift ebx to left with 1 position to increase it's value(ebx is the power
    ; of 2 corresponding to the current bit in the pair).
    shl bl, 0x1

    ; If ebx is 16, then the pair is complete and we have to move the next one.
    cmp bl, 0x10
    jne next_bit

; If al is lower than 10 then the corresponding hexadecimal digit for this
; pair is a number, else it's a letter between 'A' and 'F'.
get_pair_value:
    cmp al, 0xA
    jl number

letter:
    add al, 'A' - 10
    jmp set_hexa_digit

number:
    add al, '0'

set_hexa_digit:
    ; Move in ebx the index for the current character(hexadecimal digit) in 
    ; hexa_value. 
    mov ebx, dword[hex_index]

    ; Move to the current position in hexa_value the corresponding hexadecimal 
    ; digit and decrement the index.
    mov byte [edx + ebx - 1], al
    dec dword [hex_index]
    
    ; Set eax to 0 and ebx to 1 because a new pair is starting.
    xor eax, eax
    xor ebx, ebx
    mov bl, 0x1

; Move to the next bit in binary sequence.
next_bit:
    loop make_pair

    ; Check if there was any incomplete pair of digits and add it to hexa_value.
    inc ecx
    cmp al, 0x0
    jne get_pair_value

    ; Add '\n' at the end of hexa_value.
    mov ecx, dword[length]
    mov byte [edx + ecx], 0x0A

    ;; DO NOT MODIFY
    popa
    leave
    ret
    ;; DO NOT MODIFY