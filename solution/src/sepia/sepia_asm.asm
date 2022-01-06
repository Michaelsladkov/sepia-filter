global transform_chunk

%define matrix_shuffle_pattern 0b01111001

section .rodata
align 16
initial_s1: dd 0.42, 0.3, 0.222, 0.42
align 16
initial_s2: dd 0.7, 0.6, 0.3, 0.7
align 16
initial_s3: dd 0.3, 0.208, 0.461, 0.3

section .bss
align 16
pixel_buffer: resd 24

section .text
; rdi <- pixel chunk pointer 
preload:
    ;возьмём из чанка 6 чисел: b1, g1, r1, b2, g2, r2
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
        mov al, [r9 + rsi]
        mov [r8 + rdi], al
        inc rdi
        cmp rdi, 4
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
    call preload
    mov r9, pixel_buffer
    movdqa xmm0, [r9] ; b1|g1|r1|b2 -> xmm0
    movdqa xmm1, [r9 + 16]; g2|r2|0|0 -> xmm1
    movdqa xmm2, xmm1
    shufps xmm2, xmm2, 0b01010101; r2|r2|r2|r2 -> xmm2
    shufps xmm2, xmm0, 0b10100101; r2|r2|r1|r1 -> xmm2
    shufps xmm2, xmm2, 0b00111111; r1|r1|r1|r2 -> xmm2
    shufps xmm1, xmm0, 0b01010000; g2|g2|g1|g1 -> xmm1
    shufps xmm1, xmm1, 0b00111111; g1|g1|g1|g2 -> xmm1
    shufps xmm0, xmm0, 0b11000000; b1|b1|b1|b2 -> xmm0
    mulps xmm0, xmm3
    mulps xmm1, xmm4
    mulps xmm2, xmm5
    addps xmm0, xmm1
    addps xmm0, xmm2
    check:
    movdqa [r9], xmm0 ; b1'|g1'|r1'|b2' -> buffer
    call save_changed