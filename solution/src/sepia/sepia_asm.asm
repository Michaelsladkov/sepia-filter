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
    mov r9, maximums
    pminsd xmm0, [r9]
%endmacro

%macro clear_xmm1to3 0
    pxor xmm0, xmm0
    pxor xmm1, xmm1
    pxor xmm2, xmm2
%endmacro

%macro load_pixels 0
    pinsrb xmm0, [rdi], 0
    pinsrb xmm0, [rdi+3], 12
    pinsrb xmm1, [rdi+1], 0
    pinsrb xmm1, [rdi+1+3], 12
    pinsrb xmm2, [rdi+2], 0
    pinsrb xmm2, [rdi+2+3], 12
%endmacro

%macro save_changed 0
    pextrb [rsi], xmm0, 0
    pextrb [rsi+1], xmm0, 4
    pextrb [rsi+2], xmm0, 8
    pextrb [rsi+3], xmm0, 12
%endmacro

section .rodata
align 16
initial_s1: dd 0.1554, 0.224, 0.2653, 0.1554
align 16
initial_s2: dd 0.21, 0.3717, 0.4011, 0.21
align 16
initial_s3: dd 0.3227, 0.21, 0.31626, 0.3227
align 16
maximums: dd 0xFF, 0xFF, 0xFF, 0xFF

section .text
; chunk be like b1, g1, r1, b2, r2, g2, b3, g3, r3, b4, g4, r4
; rdi <- pixel chunk pointer 
; rsi <- target chunk pointer
transform_chunk:
    ;загрузим в xmm3..5 столбцы матрицы
    mov r9, initial_s1
    movdqa xmm3, [r9]
    mov r9, initial_s2
    movdqa xmm4, [r9]
    mov r9, initial_s3
    movdqa xmm5, [r9] 
    clear_xmm1to3
    load_pixels
    shufps xmm0, xmm0, 0b11000000
    shufps xmm1, xmm1, 0b11000000
    shufps xmm2, xmm2, 0b11000000
    multiplicate_and_add
    save_changed
    
    shift_matrix
    add rdi, 3
    add rsi, 4
    clear_xmm1to3
    load_pixels
    shufps xmm0, xmm0, 0b11110000
    shufps xmm1, xmm1, 0b11110000
    shufps xmm2, xmm2, 0b11110000
    multiplicate_and_add
    save_changed

    shift_matrix
    add rdi, 3
    add rsi, 4
    clear_xmm1to3
    load_pixels
    shufps xmm0, xmm0, 0b11111100
    shufps xmm1, xmm1, 0b11111100
    shufps xmm2, xmm2, 0b11111100
    multiplicate_and_add
    save_changed
ret