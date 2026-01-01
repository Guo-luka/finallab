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
#include "fs.h"

PUBLIC int do_search_dir() {
    struct inode* dir_inode;
    char filename[MAX_PATH];
    char* dir = fs_msg.pBUF;
    int pointer = 0;

    printl("here : %s\n", dir);
    memset(filename, 0, MAX_FILENAME_LEN);
    if (strip_path(filename, dir, &dir_inode) != 0) {
        return 0;
    }

    // printl("dir:%s\n", dir);
    // printl("buf:%s\n", fs_msg.pBUF);
    // printl("dir_node:%d\n", dir_inode);

    int dir_blk0_nr = dir_inode->i_start_sect;
    int nr_dir_blks = (dir_inode->i_size + SECTOR_SIZE - 1) / SECTOR_SIZE;
    int nr_dir_entries =
        dir_inode->i_size / DIR_ENTRY_SIZE; /**
                                             * including unused slots
                                             * (the file has been deleted
                                             * but the slot is still there)
                                             */
    struct dir_entry* pde;
    int i, j;
	char temp_buf[SECTOR_SIZE];
    for (i = 0; i < nr_dir_blks; i++) {
        RD_SECT(dir_inode->i_dev, dir_blk0_nr + i);
        //pde = (struct dir_entry*)fsbuf;
	memcpy(temp_buf, fsbuf, SECTOR_SIZE);
	pde = (struct dir_entry*)temp_buf;        
	for (j = 0; j < SECTOR_SIZE / DIR_ENTRY_SIZE; j++, pde++) {
             printl("%s  ", pde->name);
		//printl("%d ", pde);
		if (pde->name[0] == 0) {
			continue;		
		}            
		dir[pointer] = '\n';
            pointer += 1;
		printl("here");
            memcpy(dir + pointer, pde->name, strlen(pde->name));
            pointer += strlen(pde->name);
	    int k;
	    //for (k = 0; k < 15 - strlen(pde->name); k++) {
	    	dir[pointer] = ' ';
            	pointer += 1;
	    //}
	    char temp[11];
    	    int i = 0;
	    u32 num = 0;
	//	printl("%d ", num);
	    struct inode* inode = get_inode(dir_inode->i_dev, pde->inode_nr);
		num = pde->inode_nr;
		printl("%d ", num);
    	    if (inode) {
	    	num = inode->i_size;
		printl("%d ",inode->i_num);		
		if (inode->i_cnt > 0) {
            		put_inode(inode);
		}    
	    }
		struct inode* p;

	printl("\n");
//   for (p = &inode_table[0]; p < &inode_table[10]; p++) {
  //     		printl("%d ",p->i_num);

            if (num == 0) {
		temp[i++] = '0';
	    }
    	    while (num > 0) {
    	        temp[i++] = (num % 10) + '0'; 
    	        num /= 10;
    	    }
    
	    int j;
	    for (j = 0; j < i; j++) {
        	dir[pointer] = temp[i - j - 1];
		pointer += 1;
	    }
        }
    }
    // printl("after for : %s\n", dir);
    return (void*)0;
}
