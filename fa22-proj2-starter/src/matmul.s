.globl matmul

.text
# =======================================================
# FUNCTION: Matrix Multiplication of 2 integer matrices
#   d = matmul(m0, m1)
# Arguments:
#   a0 (int*)  is the pointer to the start of m0
#   a1 (int)   is the # of rows (height) of m0
#   a2 (int)   is the # of columns (width) of m0
#   a3 (int*)  is the pointer to the start of m1
#   a4 (int)   is the # of rows (height) of m1
#   a5 (int)   is the # of columns (width) of m1
#   a6 (int*)  is the pointer to the the start of d
# Returns:
#   None (void), sets d = matmul(m0, m1)
# Exceptions:
#   Make sure to check in top to bottom order!
#   - If the dimensions of m0 do not make sense,
#     this function terminates the program with exit code 38
#   - If the dimensions of m1 do not make sense,
#     this function terminates the program with exit code 38
#   - If the dimensions of m0 and m1 don't match,
#     this function terminates the program with exit code 38
# =======================================================
matmul:

    # Error checks
    li t3, 1
    blt a1, t3, invalid_arg
    blt a2, t3, invalid_arg
    blt a4, t3, invalid_arg
    blt a5, t3, invalid_arg
    bne a2, a4, invalid_arg

    # Prologue
    # save ra, s0, s1, s2
    addi sp, sp, -16
    sw ra, 12(sp)
    sw s0, 8(sp)
    sw s1, 4(sp)
    sw s2, 0(sp)

    # s0: row, s1: col, s2: address of result element
    # a7: m1 * n2, t0: i
    li s0, 0
    li s1, 0
    add s2, x0, a6
    mul a7, a1, a5
    li t0, 0

loop_start:
    beq t0, a7, loop_end
    # save t0, a0..a7
    addi sp, sp, -36
    sw t0, 32(sp)
    sw a0, 28(sp)
    sw a1, 24(sp)
    sw a2, 20(sp)
    sw a3, 16(sp)
    sw a4, 12(sp)
    sw a5, 8(sp)
    sw a6, 4(sp)
    sw a7, 0(sp)

    mul t1, s0, a2
    slli t1, t1, 2
    add a0, a0, t1
    slli t2, s1, 2
    add a1, a3, t2
    # a2 is not changed
    li a3, 1
    add a4, x0, a5
    jal ra, dot
    sw a0, 0(s2)

    # load t0, a0..a7
    lw t0, 32(sp)
    lw a0, 28(sp)
    lw a1, 24(sp)
    lw a2, 20(sp)
    lw a3, 16(sp)
    lw a4, 12(sp)
    lw a5, 8(sp)
    lw a6, 4(sp)
    lw a7, 0(sp)
    addi sp, sp, 36

    addi t0, t0, 1
    slli t1, t0, 2
    add s2, a6, t1
    addi s1, s1, 1
    bne s1, a5, loop_start
    add s1, x0, x0
    addi s0, s0, 1
    j loop_start

loop_end:

    # Epilogue
    # load ra, s0, s1, s2
    lw ra, 12(sp)
    lw s0, 8(sp)
    lw s1, 4(sp)
    lw s2, 0(sp)
    addi sp, sp, 16

    jr ra

invalid_arg:
    li a0, 38
    j exit