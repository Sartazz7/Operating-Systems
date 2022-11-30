#include "emufs_disk.h"
#include "emufs.h"

int closedevice(int mount_point){
    /*
        * Unmount the device
        
        * Return value: -1,     error
                         1,     success
    */

    return closedevice_(mount_point);
}

#define MAX_MAPPINGS 4

void update_bitmap(char* inode_bitmap, char* block_bitmap, int inodenum, int mount_point){
    struct inode_t inode;
    read_inode(mount_point, inodenum, &inode);
    // file
    if(inode.type==0){
        int num_data_blocks = 0; // inode.size/BLOCKSIZE
        // update data bitmap.
        for(int i=0; i<MAX_MAPPINGS; i++)
        {
            // printf("up %d\n",inode.mappings[i]);
            if(inode.mappings[i]==-1)
                continue;
            // printf("down %d\n",inode.mappings[i]);
            // printf(inode.mappings[i]);
            block_bitmap[inode.mappings[i]] = 1;
            num_data_blocks++;
        }
        int partially_filled_blocks = 0;
        if(inode.size%BLOCKSIZE!=0){
            partially_filled_blocks++;
        }
        if((int)(inode.size/BLOCKSIZE) + partially_filled_blocks != num_data_blocks)
            printf("Error: Size of file inconsistent with number of valid data blocks mappings. inode = %d, name = %s, inode.size=%d, num_data_blocks=%d\n", inodenum, inode.name, inode.size, num_data_blocks);
        return;
    }
    // Size check... Is size == number of mappings that are not -1?
    int num_mappings = 0;
    for(int i=0; i<MAX_MAPPINGS; i++){
        if(inode.mappings[i]==-1)
            continue;
        
        // printf("here %d\n", inode.mappings[i]);
        inode_bitmap[inode.mappings[i]] = 1;
        num_mappings++;
        update_bitmap(inode_bitmap, block_bitmap, inode.mappings[i], mount_point);
    }
    if(num_mappings!=inode.size){
        printf("Error: Size of directory does not match number of valid mappings. inode = %d, name = %s, num_mappings=%d, inode.size=%d\n", inodenum, inode.name, num_mappings, inode.size);
        return;
    }
    return;
}

// inode 0 is root directory. Should always be 1.
void filesystem_check(int mount_point){
    printf("Checking file system...\n");
    struct superblock_t superblock;
    read_superblock(mount_point, &superblock);
    char* inode_bitmap = superblock.inode_bitmap;
    char* block_bitmap = superblock.block_bitmap;

    // for(int i=0;i<MAX_INODES;i++)
    //     {inode_bitmap[i] = 0;}

    // Create bitmap by traversing the file system
    char inode_bitmap2[MAX_INODES];
    char block_bitmap2[MAX_BLOCKS];
    struct inode_t inode;

    // Start with inode_num (=0) for root directory.
    
    for(int i=0;i<MAX_INODES;i++) 
        inode_bitmap2[i]=0;
    for(int i=0;i<MAX_BLOCKS;i++) 
        inode_bitmap2[i]=0;
    block_bitmap2[0] = 1;
    block_bitmap2[1] = 1;
    block_bitmap2[2] = 1;
    inode_bitmap2[0] = 1;
    update_bitmap(inode_bitmap2, block_bitmap2, 0, mount_point);
    
    for(int ii=0; ii<MAX_INODES; ii++){
        if(inode_bitmap[ii]!=inode_bitmap2[ii]){
            printf("Inode bitmap error at inode %d\n", ii);
            // printf("in superblock  = %d\n",inode_bitmap[ii]);
            // printf("by recursion  = %d\n",inode_bitmap2[ii]);
            // return;
        }
    }

    for(int ii=0; ii<MAX_INODES; ii++){
        if(block_bitmap[ii]!=block_bitmap2[ii]){
            printf("Data block bitmap error at block num = %d\n", ii);
            // printf("in superblock  = %d\n",block_bitmap[ii]);
            // printf("by recursion  = %d\n",block_bitmap2[ii]);
            // return;
        }
    }

    printf("File system check passed.\n\n");
}