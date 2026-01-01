#include <stdio.h>
int main(int args, char* argv[]) {
    //struct proc* p = proc_table;
    //int i;
    //for (i = 0; i < NR_TASKS + NR_PROCS; i++, p++) {
        //printf("%s ", p->name);
    //}
	char* result;
    result = search_proc();
    printf("%s\n", result);
    return 0;
}
