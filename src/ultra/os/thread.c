#include "PR/os_internal.h"
#include "PRinternal/osint.h"

void __osDequeueThread(register OSThread **queue, register OSThread *t) {
    register OSThread *pred;
    register OSThread *succ;

    pred = (OSThread *)queue;
    succ = pred->next;

    while (succ != NULL) {
        if (succ == t) {
            pred->next = t->next;
#ifdef _DEBUG
            t->next = NULL;
#endif
            return;
        }
        pred = succ;
        succ = pred->next;
    }
}
