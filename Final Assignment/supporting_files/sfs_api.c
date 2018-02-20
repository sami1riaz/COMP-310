

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fuse.h>
#include <strings.h>

#include "sfs_api.h"
#include "bitmap.h"
#include "disk_emu.h"

#define NUM_INODES 100
#define LASTNAME_FIRSTNAME_DISK "sfs_disk.disk"
#define NUM_BLOCKS 1024                             //maximum number of data blocks on the disk.
#define BITMAP_ROW_SIZE (NUM_BLOCKS/8)              // this essentially mimcs the number of rows we have in the bitmap. we will have 128 rows. 

/* macros */
#define FREE_BIT(_data, _which_bit) \
    _data = _data | (1 << _which_bit)

#define USE_BIT(_data, _which_bit) \
    _data = _data & ~(1 << _which_bit)


//initialize all bits to high
uint8_t free_bit_map[BITMAP_ROW_SIZE] = { [0 ... BITMAP_ROW_SIZE - 1] = UINT8_MAX };

directory_entry rootDir[NUM_INODES];    //directory table to keep track of names and i-nodes of entries
int index;                              // root directory tracker

inode_t iNode_Table[NUM_INODES];              // i-node table
int empty_iNodes[NUM_INODES];           // to keep track of empty slots in i-node table

file_descriptor fd_Table[NUM_INODES];    // fd table


/* 
Initialize i-Node table
*/
void init_iNode_Table()
{
    for int(i = 0, i < NUM_INODES; i++)
    {
        iNode_Table() 
    }

}

void init_fd_Table()
{
    for(int i = 0, i < NUM_INODES, i++)
    {
        fd_Table[i].inodeIndex = -1;
    }
}
/*
The super block defines the file system geometry. 
It is also the first block in SFS.
*/
void create_SuperBlock(superblock_t sb)
{
    sb.magic = 0xACBD0005;
    sb.block_size = 1024;
    sb.fs_size = NUM_BLOCKS;
    sb.inode_table_len = NUM_BLOCKS;
    sb.root_dir_inode = 0;              
}

/*
Formats the virtual disk implemented by the disk emulator and creates an instance of the simple file system 
on top of it. 
The mksfs() has a fresh flag to signal that the file system should be created from scratch. 
If flag is false, the file system is opened from the disk (i.e. we assume that a valid file system is 
already there in the file system). 
The support for persistence is important so you can reuse existing data or create a new file system.
*/
void mksfs(int fresh) {
	
    if (fresh)                // flag to signal that no file system exists therefore create one from scratch
    {            
        printf("Creating File System from scratch\n");
        int superBlock_size = 1;
        create_SuperBlock();


        init_fresh_disk(LASTNAME_FIRSTNAME_DISK, BLOCK_SIZE, NUM_BLOCKS);   // initialize a disk file with 0's
        
    }      

    else                        // reopen existing file system
    {
        printf("Reopening existing File System!\n");
        read_blocks(0,1,&sb);
    


    }


}

/*
Copies the name of the next file in the directory into fname and returns non zero if there is a new file. 
Once all the files have been returned, this function returns 0. 
So, you should be able to use this function to loop through the directory. 
In implementing this function, you need to ensure that the function remembers the 
current position in the directory at each call. 
Remember we have a single-level directory.
*/
int sfs_getnextfilename(char *fname)
{

    strcpy(fname, rootDirectory.entry[])

}

/*
Returns the size of a given file.
*/
int sfs_getfilesize(const char* path){

    if((int i = 0; i < ))

}
int sfs_fopen(char *name){

}
int sfs_fclose(int fileID) {

}
int sfs_fread(int fileID, char *buf, int length) {
	
}
int sfs_fwrite(int fileID, const char *buf, int length) {

}
int sfs_fseek(int fileID, int loc) {

}
int sfs_remove(char *file) {


}

