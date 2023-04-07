.globl dot

.text
# =======================================================
# FUNCTION: Dot product of 2 int arrays
# Arguments:
#   a0 (int*) is the pointer to the start of arr0
#   a1 (int*) is the pointer to the start of arr1
#   a2 (int)  is the number of elements to use
#   a3 (int)  is the stride of arr0
#   a4 (int)  is the stride of arr1
# Returns:
#   a0 (int)  is the dot product of arr0 and arr1
# Exceptions:
#   - If the length of the array is less than 1,
#     this function terminates the program with error code 36
#   - If the stride of either array is less than 1,
#     this function terminates the program with error code 37
# =======================================================
dot:
    ebreak
    # Prologue
    addi t3, x0, 1
    blt a2, t3, invalid_number_elements
    blt a3, t3, invalid_stride
    blt a4, t3, invalid_stride

    addi sp, sp, -8
    sw s0, 4(sp)
    sw s1, 0(sp)

loop_start:
    # t0: i, t1: offset_array1, t2: offset_array2
    # t3: address of the element in array1
    # t4: address of the element in array2
    # t5: element in array1
    # t6: element in array2
    # s0: sum
    # s1: intermediate multiplication
    add t0, x0, x0
    slli t1, a3, 2
    slli t2, a4, 2
    add t3, a0, x0
    add t4, a1, x0
    add s0, x0, x0

loop_continue:
    beq t0, a2, loop_end
    lw t5, 0(t3)
    lw t6, 0(t4)
    mul s1, t5, t6
    add s0, s0, s1
    addi t0, t0, 1
    add t3, t3, t1
    add t4, t4, t2
    j loop_continue

loop_end:

    # Epilogue
    mv a0, s0
    lw s0, 4(sp)
    lw s1, 0(sp)
    addi sp, sp, 8
    jr ra

invalid_number_elements:
    li a0 36
    j exit

invalid_stride:
    li a0 37
    j exit