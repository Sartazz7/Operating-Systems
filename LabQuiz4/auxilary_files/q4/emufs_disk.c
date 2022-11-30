#include "emufs_disk.h"
#include "emufs.h"

struct mount_t mounts[MAX_MOUNT_POINTS];


/*-----------DEVICE------------*/

int readblock(int dev_fd, int block, char * buf)
{
	/*
		* Writes a block in the device to the memory buffer

		* Return value: -1, error
						 1, success
	*/

	int ret;
	int offset;

	if(dev_fd < 0)
	{
		printf("Devices not found\n");
		return -1;
	}
	offset = block * BLOCKSIZE;
	lseek(dev_fd, offset, SEEK_SET);
	ret = read(dev_fd, buf, BLOCKSIZE);
	if(ret != BLOCKSIZE)
	{
		printf("Error: Disk read error. fd: %d. block: %d. buf: %p. ret: %d \n", dev_fd, block, buf, ret);
		return -1;
	}

	return 1;
}


/*----------MOUNT-------*/
int add_new_mount_point(int fd, char *device_name, int fs_number)
{
	/*
		* Creates a mount for the device
		* Assigns an entry in the mount devices array

		* Return value: -1,									error
						array entry index (mount point)		success
	*/

	struct mount_t* mount_point = NULL;

	for(int i=0; i<MAX_MOUNT_POINTS; i++)
		if(mounts[i].device_fd <= 0 )
		{
			mount_point = &mounts[i];
			mount_point->device_fd = fd;
			strcpy(mount_point->device_name, device_name);
			mount_point->fs_number = fs_number;

			return i;
		}

	return -1;
}


int opendevice(char* device_name)
{
	/*
		* Open an existing device and do some consistency checks
		* Assigns a mount point

		* Return value: -1, 			error
						 mount point,	success	
	*/			

	int fd;
	FILE* fp;
	char tempBuf[BLOCKSIZE];
	struct superblock_t* superblock;
	int mount_point;
	int key;

	if(!device_name || strlen(device_name) == 0)
	{
		printf("Error: Invalid device name \n");
		return -1;
	}

	superblock = (struct superblock_t*)malloc(sizeof(struct superblock_t));
	fp = fopen(device_name, "r");
	if(!fp)
	{
		//	Creating the device
		printf("[%s] Disk does not exist \n", device_name);
		return -1;
	}
	else
	{
		fclose(fp);
		fd = open(device_name, O_RDWR);

		readblock(fd, 0, tempBuf);
		memcpy(superblock, tempBuf, sizeof(struct superblock_t));

		if(superblock->magic_number != MAGIC_NUMBER || superblock->disk_size < 3 || superblock->disk_size > MAX_BLOCKS)
		{
			printf("%d,%d,%d",superblock->magic_number,superblock->disk_size,superblock->disk_size);
			printf("Error: Inconsistent super block on device. \n");
			free(superblock);
			return -1;
		}
		printf("[%s] Disk opened \n", device_name);

		if(superblock->fs_number == -1)
			printf("[%s] File system found in the disk \n", device_name);
		else
			printf("[%s] File system found.\n", device_name);
		
	}	

	mount_point = add_new_mount_point(fd, device_name, superblock->fs_number);
	if(superblock->fs_number==1)
		mounts[mount_point].key=key;

	printf("[%s] Disk successfully mounted \n", device_name);
	free(superblock);

	return mount_point;	
}


int closedevice_(int mount_point)
{
	/*
		* Closes a device

		* Return value: -1, error
						 1, success
	*/

	char device_name[20];

	if(mounts[mount_point].device_fd < 0)
	{
		printf("Error: Devices not found\n");
		return -1;
	}

	strcpy(device_name, mounts[mount_point].device_name);
	close(mounts[mount_point].device_fd);

	mounts[mount_point].device_fd = -1;
	strcpy(mounts[mount_point].device_name, "\0");
	mounts[mount_point].fs_number = -1;

	printf("[%s] Device closed \n", device_name);
	return 1;
}

void read_superblock(int mount_point, struct superblock_t *superblock){
	/*	
		* Reads the superblock of the device
		* If its an encrypted system, decrypts the magic number after reading
	*/

	char tempBuf[BLOCKSIZE];
	readblock(mounts[mount_point].device_fd, 0, tempBuf);
	memcpy(superblock, tempBuf, sizeof(struct superblock_t));
}


void read_inode(int mount_point, int inodenum, struct inode_t *inodeptr){
	/*
		* Reads the metadata in block 2 or 3
		* Decrypts the metadata block if its an encrypted system
		* Reads the inode entry from the metadata block into the memory buffer
	*/

	struct metadata_t metadata;
	int d=inodenum/16, r=inodenum%16;
	readblock(mounts[mount_point].device_fd, 1+d, (char*)&metadata);
	*inodeptr = metadata.inodes[r];
}

void read_datablock(int mount_point, int blocknum, char *buf){
	/*
		* Read the block into the memory buffer
		* Decrypt the block if its an encrypted system
	*/

	readblock(mounts[mount_point].device_fd, blocknum, buf);
}