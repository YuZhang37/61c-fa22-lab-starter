.globl f # this allows other files to find the function f

# f takes in two arguments:
# a0 is the value we want to evaluate f at
# a1 is the address of the "output" array (defined above).
# The return value should be stored in a0
f:
    addi t0, a0, 3  # index = index + 2
    slli t2, t0, 2  # index = index * 4
    add t2, t2, a1  # addr = index + &output
    lw t3 0(t2)     # value = *addr
    add a0, t3, x0  # store value in a0  
    # This is how you return from a function. You'll learn more about this later.
    # This should be the last line in your program.
    jr ra  