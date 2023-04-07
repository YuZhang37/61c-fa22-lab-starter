.globl relu

.text
# ==============================================================================
# FUNCTION: Performs an inplace element-wise ReLU on an array of ints
# Arguments:
#   a0 (int*) is the pointer to the array
#   a1 (int)  is the # of elements in the array
# Returns:
#   None
# Exceptions:
#   - If the length of the array is less than 1,
#     this function terminates the program with error code 36
# ==============================================================================
relu:
    # Prologue
    addi t3, x0, 1
    blt a1, t3, invalid_arg
    
loop_start:
    # t0: array_start, t1: number of elements in array, t2: i
    add t0, x0, a0
    add t1, x0, a1
    add t2, x0, x0

loop_continue:
    # t4: address of the element, t5: element
    beq t2, t1, loop_end
    slli t3, t2, 2
    add t4, t3, t0
    lw t5, 0(t4)
    addi t2, t2, 1
    bge t5, zero, loop_continue
    sw x0, 0(t4)
    j loop_continue


loop_end:

    # Epilogue
    jr ra

invalid_arg:
    li a0 36
    j exit
