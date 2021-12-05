%include "io.mac"

section .data
    len_needle dd 0
    len_haystack dd 0

section .text
    global my_strstr
    extern printf

my_strstr:
    ;; DO NOT MODIFY
    push    ebp
    mov     ebp, esp
    pusha

    mov     edi, [ebp + 8]      ; substr_index
    mov     esi, [ebp + 12]     ; haystack
    mov     ebx, [ebp + 16]     ; needle
    mov     ecx, [ebp + 20]     ; haystack_len
    mov     edx, [ebp + 24]     ; needle_len
    ;; DO NOT MODIFY

    ; Make ecx = haystack_len - needle_len + 1.
    ; This is done in order not to search for the needle in the haystack if 
    ; there aren't as many characters left in the haystack as in the needle.
    sub ecx, edx
    inc ecx

    ; Keep the length of the needle and (haystack_legth - needle_length + 1) 
    ; for later.
  	mov dword[len_needle], edx
    mov dword[len_haystack], ecx

    ; Set ecx to 0.
	xor ecx, ecx

; Loop through the haystack.
loop_haystack:
    ; Check if the end if the haystack has been reached( => no match was found).
	cmp ecx, dword [len_haystack]
	je no_needle_in_haystack

    ; Set edx to 0.
	xor edx, edx

; Loop through the needle.
loop_needle:
    ; If the end of the needle has been reached then it's a match.
	cmp edx, dword [len_needle]
	je done

    ; Move (ecx + edx)'th character from the haystack to al. 
    push ecx
	add ecx, edx
	mov al, byte [esi + ecx]
	pop ecx

    ; Check if the character from al matches the edx'th character from the 
    ; needle.
	cmp al, byte [ebx + edx]
	jne no_match
	
    ; Move to the next character from the needle.
	inc edx

	jmp loop_needle

; Move to the next character from the haystack.
no_match:
	inc ecx
	jmp loop_haystack

; If there wasn't any match set ecx to be haystack_length + 1 by adding the 
; needle's length to it.
no_needle_in_haystack:
	add ecx, dword [len_needle]

; Move to [edi] the index of the first match of the needle in the haystack or 
; haystack's length if there isn't any match.
done:
	mov [edi], ecx

    ;; DO NOT MODIFY
    popa
    leave
    ret
    ;; DO NOT MODIFY
