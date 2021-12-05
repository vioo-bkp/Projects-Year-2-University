; Tema 3 - IOCLA, Abstract Syntax Tree
; Mateescu Viorel-Cristian

section .data
    delim:  db " ", 0

section .bss
    root resd 1

section .text

extern check_atoi
extern print_tree_inorder
extern print_tree_preorder
extern evaluate_tree
extern strtok
extern strdup
extern calloc
extern strlen

global create_tree
global iocla_atoi

; Function that converts a numeric string to its corresponding number (int).
iocla_atoi:
    enter 0, 0
    
    ; Save the registers.
    push esi
    push ebx
    push ecx

    ; Clear registers ecx and eax.
    xor ecx, ecx
    xor eax, eax

    ; The register edx stores the number's sign (1 - positive, -1 - negative).
    ; First we suppose that it's positive.
    mov ecx, 1

    ; The string that it is going to be converted.
    mov esi, [ebp + 8]

    ; Check if it will be a negative number (if the first character in the 
    ; string is '-' then we'll have a negative number).
    movzx ebx, byte [esi]
    cmp ebx,  '-'
    jne get_digits

    ; If the first character in the string is a '-' ecx is set to -1 (negative 
    ; number) and we move to the next character.
    mov ecx, -1
    inc esi

; Loop the string's characters and compute the numbers digits.
get_digits:
    ; Move to ebx the next character in the string.
    movzx ebx, byte [esi]

    ; Check if the string's end has been reached.
    cmp ebx, 0
    je no_digits_left

    ; Subtract from ebx '0' in order to convert it from char to unsigned int.
    sub ebx, '0'

    ; Build the number by appending at its end the new digit.
    ; In order to do this we have to multiply the current version of the number
    ; by 10 and add the new digit to it.
    imul eax, 10
    add eax, ebx

    ; Move to the next character in the string.
    inc esi
    jmp get_digits

; Multiply the final version of the number with -1 or 1 to make it negative, 
; or keep it positive (multiplying by 1 doesn't affect the number in any way).
no_digits_left:
    imul eax, ecx

    ; Restore the registers.
    pop ecx
    pop ebx
    pop esi
    
    leave
    ret 

; Function that builds an abstract syntax tree from a prefix expression. 
; First the root node is created and after that its left and right subtrees are
; built using the "build_subtree" function.
create_tree:
    enter 0, 0

    ; Save the registers.
    push esi
    push ebx
    push edx

    ; The string that contains the prefix expression.
    mov esi, [ebp + 8]

    ; Get the first element in the expression. Because it's a prefix 
    ; expression it is going to be an operator. 
    push delim
    push esi
    call strtok
    add esp, 8
    
    ; Create a node that will store the first element (token) in its 'data' 
    ; field. 
    push eax
    call create_node
    add esp, 4

    ; Set the tree's root and the ebx register to the node we created earlier.
    mov [root], eax
    mov ebx, eax

    ; Build the left subtree of the root node. 
    call build_subtree
    mov [ebx + 4], eax
    
    ; Build the right subtree of the root node. 
    call build_subtree
    mov [ebx + 8], eax

    ; Set register eax to the tree's root.
    mov eax, [root]

    ; Restore the registers.    
    pop edx
    pop ebx
    pop esi

    leave
    ret

; The prefix expression is tokenized and each token is checked whether it's a 
; number or an operator. A node is created for each token. Recursion is used 
; for building the left and right subtrees of each node that contains an 
; operator. The new node is returned.
; So the strategy is:
;   - if the token is a number create a node and return it.
;   - if it's an operator create a node and create it's left and right subtrees
;     using recursion before returning it.

build_subtree:
    enter 0, 0

    ; Save the registers.
    push ebx
    push edx
    push esi

    ; Get the next element in the prefix expression.
    push delim
    push 0
    call strtok
    add esp, 8

    ; Check if the expression's end has been reached.
    cmp eax, 0
    je subtree_is_done
    
    ; Create a node that will store the token in its 'data' field.
    ; This token is a string that can either be an operator or a number.
    mov esi, eax
    push esi
    call create_node
    add esp, 4

    ; Set ebx to the current node.
    mov ebx, eax

    ; Compute the token's length by calling "strlen" function on it.
    push esi
    call strlen
    add esp, 4
    
    ; If its length is greater than 1 then we have a number and the node that 
    ; stores it should be returned.
    cmp eax, 1
    jg subtree_is_done

    ; Else we may have an operator or a number. In order to find out which is 
    ; the case we compare the character with '0'.
    movzx eax, byte [esi]
    cmp eax, '0'
    jge subtree_is_done

; In this case the element(token) is an operator and we have to build its left 
; and right subtrees.
operator:
    ; Build the left subtree for the current node and assign it to its left 
    ; child.
    call build_subtree
    mov [ebx + 4], eax

    ; Build the right subtree for the current node and assign it to its right 
    ; child.
    call build_subtree
    mov [ebx + 8], eax

subtree_is_done:
    ; Set eax to the node we created earlier before returning it.
    mov eax, ebx

    ; Restore the registers.
    pop esi
    pop edx
    pop ebx

    leave
    ret

; Function used for creating and returning a new node.
create_node:
    enter 0, 0
    
    ; Save the registers.
    push ebx
    push edx
    push esi

    ; The string that will be stored in the data field of the node that will 
    ; be created.
    mov esi, [ebp + 8]
    
    ; Allocate memory for the string and copy it to the new memory zone.
    push esi
    call strdup
    add esp, 4
    
    ; Set ebx to the string's address.
    mov ebx, eax

    ; Allocate memory for the node.
    push 1
    push 12
    call calloc
    add esp, 8

    ; Set its data field to the string we duplicated earlier.
    mov [eax], ebx

    ; Restore the registers.
    pop esi
    pop edx
    pop ebx
    
    leave
    ret