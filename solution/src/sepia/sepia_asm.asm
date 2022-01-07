global transform_chunk

%define matrix_shuffle_pattern 0b01111001

%macro shift_matrix 0
    shufps xmm3, xmm3, matrix_shuffle_pattern
    shufps xmm4, xmm4, matrix_shuffle_pattern
    shufps xmm5, xmm5, matrix_shuffle_pattern
%endmacro

%macro multiplicate_and_add 0
    cvtdq2ps xmm0, xmm0
    cvtdq2ps xmm1, xmm1
    cvtdq2ps xmm2, xmm2
    mulps xmm0, xmm3
    mulps xmm1, xmm4
    mulps xmm2, xmm5
    addps xmm0, xmm1
    addps xmm0, xmm2
    cvtps2dq xmm0, xmm0
%endmacro

section .rodata
align 16
initial_s1: dd 0.1554, 0.224, 0.2653, 0.1554
align 16
initial_s2: dd 0.21, 0.3717, 0.4011, 0.21
align 16
initial_s3: dd 0.3227, 0.21, 0.31626, 0.3227

section .bss
align 16
pixel_buffer: resd 24

section .text
; chunk be like b1, g1, r1, b2, r2, g2, b3, g3, r3, b4, g4, r4
; rdi <- pixel chunk pointer 
; rsi <- target chunk pointer
preload:
    ;возьмём из чанка 6 чисел: b1, g1, r1, b2, g2, r2 на первой иттерации; b2, g2, r2, b3, g3, r3 на второй и b3, g3, r3, b4, g4, r4 на третьей
    ;переложим их в буфер так, чтобы потом взять их в xmm
    xor r8, r8
    mov r9, pixel_buffer; сохраняем адрес буфера в регистре
    .preload_loop:
        mov al, byte [r8 + rdi]
        mov rsi, r8
        shl rsi, 2
        mov [r9 + rsi], al
        inc r8
        cmp r8, 6
        jl .preload_loop
    ret

; rdi <- pixel chunk pointer 
save_changed:
    xor r8, r8
    mov r9, pixel_buffer
    .save_loop:
        mov rsi, r8
        shl rsi, 2
        xor rax, rax
        mov al, byte [r9 + rsi]
        cmp rax, 255
        jb .write
        mov rax, 255
        .write:
        mov [r8 + rdi], al
        inc r8
        cmp r8, 4
        jl .save_loop
    ret

; rdi <- pixel chunk pointer 
transform_chunk:
    ;загрузим в xmm3..5 столбцы матрицы
    mov r9, initial_s1
    movdqa xmm3, [r9]
    mov r9, initial_s2
    movdqa xmm4, [r9]
    mov r9, initial_s3
    movdqa xmm5, [r9] 
    push rdi
    push rsi
    call preload
    pop rsi
    pop rdi
    mov r9, pixel_buffer
    movdqa xmm0, [r9] ; b1|g1|r1|b2 -> xmm0
    movdqa xmm1, [r9 + 16]; g2|r2|0|0 -> xmm1
    movdqa xmm2, xmm1
    shufps xmm2, xmm0, 0b10100101; r2|r2|r1|r1 -> xmm2
    shufps xmm2, xmm2, 0b00111111; r1|r1|r1|r2 -> xmm2
    shufps xmm1, xmm0, 0b01010000; g2|g2|g1|g1 -> xmm1
    shufps xmm1, xmm1, 0b00111111; g1|g1|g1|g2 -> xmm1
    shufps xmm0, xmm0, 0b11000000; b1|b1|b1|b2 -> xmm0
    multiplicate_and_add
    movdqa [r9], xmm0 ; b1'|g1'|r1'|b2' -> buffer
    push rdi
    push rsi
    mov rdi, rsi
    call save_changed
    pop rsi
    pop rdi
    
    shift_matrix
    add rdi, 3
    add rsi, 4
    push rdi
    push rsi
    call preload
    pop rsi
    pop rdi
    mov r9, pixel_buffer
    movdqa xmm0, [r9] ; b2|g2|r2|b3 -> xmm0
    movdqa xmm1, [r9 + 16]; g3|r3|0|0 -> xmm1
    movdqa xmm2, xmm1
    shufps xmm2, xmm0, 0b10100101; r3|r3|r2|r2 -> xmm2
    shufps xmm2, xmm2, 0b00001111; r2|r2|r3|r3 -> xmm2
    shufps xmm1, xmm0, 0b01010000; g3|g3|g2|g2 -> xmm1
    shufps xmm1, xmm1, 0b00001111; g2|g2|g3|g3 -> xmm1
    shufps xmm0, xmm0, 0b11110000; b2|b2|b3|b3 -> xmm0
    check:
    multiplicate_and_add
    movdqa [r9], xmm0 ; b1'|g1'|r1'|b2' -> buffer
    push rdi
    push rsi
    mov rdi, rsi
    call save_changed
    pop rsi
    pop rdi

    shift_matrix
    add rdi, 3
    add rsi, 4
    push rdi
    push rsi
    call preload
    pop rsi
    pop rdi
    mov r9, pixel_buffer
    movdqa xmm0, [r9] ; b3|g3|r3|b4 -> xmm0
    movdqa xmm1, [r9 + 16]; g4|r4|0|0 -> xmm1
    movdqa xmm2, xmm1
    shufps xmm2, xmm0, 0b10100101; r4|r4|r3|r3 -> xmm2
    shufps xmm2, xmm2, 0b00000011; r3|r4|r4|r4 -> xmm2
    shufps xmm1, xmm0, 0b01010000; g4|g4|g3|g3 -> xmm1
    shufps xmm1, xmm1, 0b00000011; g3|g4|g4|g4 -> xmm1
    shufps xmm0, xmm0, 0b11111100; b3|b4|b4|b4 -> xmm0
    multiplicate_and_add
    movdqa [r9], xmm0 ; b1'|g1'|r1'|b2' -> buffer
    mov rdi, rsi
    call save_changed
ret