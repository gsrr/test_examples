#include<unistd.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<error.h>
#include<sys/ioctl.h>
#include<sys/types.h>
#include<sys/mman.h>
#include<scsi/scsi_ioctl.h>
#include<scsi/sg.h>
/* reads the LBA entered from the terminal */
/* compile: gcc file.c*/
/* execute: sudo ./a.out /dev/sg<> */

#define DISPLAY
#define LBA_SIZE 512
#define CMD_LEN 12
#define BLOCK_MAX 255
#define LBA_MAX (1<<24)

int main(int argc, char* argv[]){
	int fd,i;
	int lba;
	int no_of_blocks = 1;

	printf("enter the LBA you want to read: ");
	scanf("%d",&lba);

	if(lba > LBA_MAX){
		printf("LBA larger than %d not allowed with this version\n",LBA_MAX);
		return 1;
	}

	printf("Enter the NUMBER OF BLOCK you want to read: ");
	scanf("%d",&no_of_blocks);

	if((no_of_blocks > BLOCK_MAX)||(no_of_blocks <= 0) ){
		printf("it is not possible to read more than %d BLOCKS\n",BLOCK_MAX);
		printf("we are reading 1 BLOCK for you\n");
		no_of_blocks = 1;
	}

	unsigned char cmd_blk[CMD_LEN] =
	{0xa1,0x0D,0x2E,0,no_of_blocks,lba, (lba >> 8), (lba>>16),0x40,0x25,0, 0};
	sg_io_hdr_t io_hdr;
	char* file_name = 0;
	unsigned char buffer[LBA_SIZE * no_of_blocks];
	unsigned char sense_buffer[32];

	if(argc < 2){
		printf("please enter a device file\n");
		return 1;
	}else{
		file_name = argv[1];
	}

	/////////opening the device file/////////////

	if((fd = open(file_name,O_RDWR))<0){
		printf("device file opening failed\n");
		return 1;
	}

	/////////// data buffer ///////////
	// printf("********data buffer initially***********\n");
	for(i=0 ; i< (LBA_SIZE * no_of_blocks) ; i++){
		buffer[i] = 0;
		// printf("%hx ",buffer[i]);
	}
	printf("\n");

	////////////////prepare sg header for read//////
	memset(&io_hdr,0,sizeof(sg_io_hdr_t));
	io_hdr.interface_id = 'S';
	io_hdr.cmd_len = sizeof(cmd_blk);
	io_hdr.mx_sb_len = sizeof(sense_buffer);
	io_hdr.dxfer_direction = SG_DXFER_FROM_DEV;
	io_hdr.dxfer_len = (LBA_SIZE * no_of_blocks);
	io_hdr.dxferp = buffer;
	io_hdr.cmdp = cmd_blk;
	io_hdr.sbp = sense_buffer;
	io_hdr.timeout = 20000;

	if(ioctl(fd,SG_IO,&io_hdr)<0){
		printf("ioctl failed\n");
		for(i = 0 ; i<32;i++)
			printf("%hx ",sense_buffer[i]);
		return 1;
	}
	close(fd);

#ifdef DISPLAY
	printf("********data buffer after ioctl***********\n");
	for(i=0; i<(LBA_SIZE * no_of_blocks) ; i++){
		if(i%512 == 0) printf("\n NEW BLOCK \n");
		printf("%hx ",buffer[i]);
	}
#endif
	printf("\n");

	printf("\n*********duration = %d\n", io_hdr.duration);

	return 1;
}
