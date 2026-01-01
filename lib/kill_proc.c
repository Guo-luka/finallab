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
#include "proto.h"

PUBLIC int kill(int pid) {
    MESSAGE msg;
    
    msg.type = KILL_PROC;
    
    msg.PID = pid;
    send_recv(BOTH, TASK_MM, &msg);
    return msg.RETVAL;
}