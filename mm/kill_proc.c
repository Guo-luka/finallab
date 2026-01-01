#include "type.h"
#include "stdio.h"
#include "const.h"
#include "protect.h"
#include "string.h"
#include "fs.h"
#include "proc.h"
#include "tty.h"
#include "console.h"
#include "global.h"
#include "keyboard.h"
#include "proto.h"
#include "hd.h"

PRIVATE void cleanup(struct proc* proc) {
    MESSAGE msg2parent;
    msg2parent.type = SYSCALL_RET;
    msg2parent.PID = proc2pid(proc);
    msg2parent.STATUS = proc->exit_status;
    send_recv(SEND, proc->p_parent, &msg2parent);

    proc->p_flags = FREE_SLOT;
}

PUBLIC int do_kill_proc(int pid) {
    // struct proc* p = proc_table;
    // int i;
    // for (i = NR_TASKS; i < NR_TASKS + NR_PROCS; i++, p++) {
    //     if (p->pid == pid) {
    //         p->p_flags = FREE_SLOT;
    //         return 0;  // success
    //     }
    // }
    // return -1;  // fail
    int status = 1;
    if (pid < NR_TASKS + NR_NATIVE_PROCS || pid >= (NR_TASKS + NR_PROCS)) {
        return -1;  // invalid pid
    }
    if (proc_table[pid].p_flags == FREE_SLOT) {
        return -1;  // process already free
    }

    int i;
    int parent_pid = proc_table[pid].p_parent;
    struct proc* p = &proc_table[pid];

    /* tell FS, see fs_exit() */
    MESSAGE msg2fs;
    msg2fs.type = EXIT;
    msg2fs.PID = pid;
    send_recv(BOTH, TASK_FS, &msg2fs);

    free_mem(pid);

    p->exit_status = status;

    if (proc_table[parent_pid].p_flags & WAITING) { /* parent is waiting */
        proc_table[parent_pid].p_flags &= ~WAITING;
        cleanup(&proc_table[pid]);
    } else { /* parent is not waiting */
        proc_table[pid].p_flags |= HANGING;
    }

    /* if the proc has any child, make INIT the new parent */
    for (i = 0; i < NR_TASKS + NR_PROCS; i++) {
        if (proc_table[i].p_parent == pid) { /* is a child */
            proc_table[i].p_parent = INIT;
            if ((proc_table[INIT].p_flags & WAITING) &&
                (proc_table[i].p_flags & HANGING)) {
                proc_table[INIT].p_flags &= ~WAITING;
                cleanup(&proc_table[i]);
            }
        }
    }


    return 0;  // success

}
