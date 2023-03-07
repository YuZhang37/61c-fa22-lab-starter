#include <stddef.h>
#include "ll_cycle.h"

int ll_has_cycle(node *head) {
    /* TODO: Implement ll_has_cycle */
    node dummyHead;
    dummyHead.value = 0;
    dummyHead.next = head;
    node *slow_ptr = &dummyHead, *fast_ptr = &dummyHead;
    if (fast_ptr->next == 0 || fast_ptr->next->next == 0) {
        return 0;
    } else {
        fast_ptr = fast_ptr->next->next;
    }
    while (slow_ptr != fast_ptr) {
        if (fast_ptr->next == 0 || fast_ptr->next->next == 0) {
            return 0;
        } else {
            fast_ptr = fast_ptr->next->next;
        }
        slow_ptr = slow_ptr->next;
    }
    return 1;
}
