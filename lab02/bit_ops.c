#include <stdio.h>
#include "bit_ops.h"

/* Returns the Nth bit of X. Assumes 0 <= N <= 31. */
unsigned get_bit(unsigned x, unsigned n) {
    unsigned result = 1;
    result = result & (x >> n);
    return result;
}

/* Set the nth bit of the value of x to v. Assumes 0 <= N <= 31, and V is 0 or 1 */
void set_bit(unsigned *x, unsigned n, unsigned v) {
    unsigned base = 1 << n;
    if (v == 0) {
        *x = *x & ~base;
    }
    if (v == 1) {
        *x = *x | base;
    }
}

/* Flips the Nth bit in X. Assumes 0 <= N <= 31.*/
void flip_bit(unsigned *x, unsigned n) {
    unsigned base = 1 << n;
    *x = *x ^ base;
}

