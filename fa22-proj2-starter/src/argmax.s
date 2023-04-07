.globl argmax

.text
# =================================================================
# FUNCTION: Given a int array, return the index of the largest
#   element. If there are multiple, return the one
#   with the smallest index.
# Arguments:
#   a0 (int*) is the pointer to the start of the array
#   a1 (int)  is the # of elements in the array
# Returns:
#   a0 (int)  is the first index of the largest element
# Exceptions:
#   - If the length of the array is less than 1,
#     this function terminates the program with error code 36
# =================================================================
argmax:
    # Prologue
    addi t3, x0, 1
    blt a1, t3, invalid_arg

    addi sp, sp, -8
    sw s0, 4(sp)
    sw s1, 0(sp)

loop_start:
    # t0: array_start, t1: number of elements in array, t2: i
    # s0: max_element, s1: max_element_index
    add t0, x0, a0
    add t1, x0, a1
    addi t2, x0, 1
    lw s0, 0(t0)
    li s1, 0

loop_continue:
    # t4: address of the element, t5: element
    beq t2, t1, loop_end
    slli t3, t2, 2
    add t4, t3, t0
    lw t5, 0(t4)
    addi t2, t2, 1
    bge s0, t5, loop_continue
    add s0, t5, x0
    addi s1, t2, -1
    j loop_continue

loop_end:
    # Epilogue
    add a0, s1, x0
    lw s0, 4(sp)
    lw s1, 0(sp)
    addi sp, sp, 8
    jr ra

invalid_arg:
    li a0 36
    j exit